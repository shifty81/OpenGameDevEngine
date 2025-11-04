#!/bin/bash
# OpenGameDevEngine PR Testing Script for Linux/macOS
# This script automates pulling and testing pull requests

set -e

# ============================================================================
# Configuration
# ============================================================================
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
LOG_DIR="$ROOT_DIR/logs"
TIMESTAMP=$(date +"%Y-%m-%d_%H-%M-%S")
LOG_FILE="$LOG_DIR/test-pr_$TIMESTAMP.log"
MODE="in-place"
PR_NUMBER=""
AUTO_BUILD=1
WORKSPACE_DIR="$ROOT_DIR/pr-workspace"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Create logs directory if it doesn't exist
mkdir -p "$LOG_DIR"

# ============================================================================
# Logging Functions
# ============================================================================
log() {
    local msg="[$(date '+%Y-%m-%d %H:%M:%S')] [INFO] $1"
    echo -e "${BLUE}$msg${NC}"
    echo "$msg" >> "$LOG_FILE"
}

log_success() {
    local msg="[$(date '+%Y-%m-%d %H:%M:%S')] [SUCCESS] $1"
    echo -e "${GREEN}$msg${NC}"
    echo "$msg" >> "$LOG_FILE"
}

log_warning() {
    local msg="[$(date '+%Y-%m-%d %H:%M:%S')] [WARNING] $1"
    echo -e "${YELLOW}$msg${NC}"
    echo "$msg" >> "$LOG_FILE"
}

log_error() {
    local msg="[$(date '+%Y-%m-%d %H:%M:%S')] [ERROR] $1"
    echo -e "${RED}$msg${NC}"
    echo "$msg" >> "$LOG_FILE"
}

# ============================================================================
# Help Function
# ============================================================================
show_help() {
    cat << EOF
Usage: test-pr.sh [options]

Description:
  Automate pulling and testing pull requests from the current repository.

Modes:
  --in-place        Update current directory with PR (default)
  --multi-version   Create separate directory for each PR

Options:
  --pr NUMBER       Specify PR number to test (required if gh CLI not installed)
  --no-build        Skip automatic build after checkout
  --workspace PATH  Custom workspace directory for multi-version mode
                    (default: ./pr-workspace)
  --help            Show this help message

Examples:
  ./test-pr.sh --pr 42
    Test PR #42 in current directory

  ./test-pr.sh --pr 42 --multi-version
    Test PR #42 in separate directory (./pr-workspace/pr-42)

  ./test-pr.sh --pr 42 --multi-version --no-build
    Checkout PR #42 without building

  ./test-pr.sh
    Automatically fetch and test next open PR (requires gh CLI)

Requirements:
  - Git must be installed
  - GitHub CLI (gh) is optional but recommended for auto-detection

Multi-Version Mode:
  Creates separate directories for each PR, allowing you to test
  multiple PRs side-by-side without switching branches:
    pr-workspace/
      pr-42/    (PR #42)
      pr-43/    (PR #43)
      pr-44/    (PR #44)
EOF
    exit 0
}

# ============================================================================
# Banner
# ============================================================================
echo ""
echo "==============================================================="
echo "         OpenGameDevEngine PR Testing Tool"
echo "         Automated Pull Request Testing"
echo "==============================================================="
echo ""

log "PR testing script started"

# ============================================================================
# Check Prerequisites
# ============================================================================
log "Checking prerequisites..."

# Check Git
if ! command -v git &> /dev/null; then
    log_error "Git not found! Please install Git."
    exit 1
fi
log_success "Git found"

# Check if we're in a git repository
cd "$ROOT_DIR"
if ! git rev-parse --git-dir &> /dev/null; then
    log_error "Not in a git repository!"
    exit 1
fi
log_success "Git repository detected"

# Get remote URL
REMOTE_URL=$(git remote get-url origin 2>/dev/null || echo "")
if [ -z "$REMOTE_URL" ]; then
    log_error "No git remote 'origin' found!"
    exit 1
fi
log "Repository: $REMOTE_URL"

# ============================================================================
# Parse Command Line Arguments
# ============================================================================
while [[ $# -gt 0 ]]; do
    case $1 in
        --multi-version)
            MODE="multi-version"
            shift
            ;;
        --in-place)
            MODE="in-place"
            shift
            ;;
        --pr)
            PR_NUMBER="$2"
            shift 2
            ;;
        --no-build)
            AUTO_BUILD=0
            shift
            ;;
        --workspace)
            WORKSPACE_DIR="$2"
            shift 2
            ;;
        --help)
            show_help
            ;;
        *)
            log_warning "Unknown option: $1"
            shift
            ;;
    esac
done

# ============================================================================
# Validate PR Number or Get Next PR
# ============================================================================
if [ -z "$PR_NUMBER" ]; then
    log "Fetching next open PR..."
    
    # Try to get PR list using gh CLI if available
    if command -v gh &> /dev/null; then
        PR_NUMBER=$(gh pr list --state open --limit 1 --json number --jq ".[0].number" 2>/dev/null || echo "")
    fi
    
    if [ -z "$PR_NUMBER" ]; then
        log_error "Could not automatically fetch PR number."
        log_error "Please specify a PR number using --pr <number>"
        log_error "Or install GitHub CLI (gh) for automatic PR detection."
        exit 1
    fi
    log_success "Found PR #$PR_NUMBER"
else
    log "Using specified PR #$PR_NUMBER"
fi

# ============================================================================
# Execute Based on Mode
# ============================================================================
in_place_mode() {
    log "Mode: In-Place (updating current directory)"
    log "============================================================"
    
    # Save current branch
    CURRENT_BRANCH=$(git rev-parse --abbrev-ref HEAD 2>/dev/null || echo "")
    log "Current branch: $CURRENT_BRANCH"
    
    # Check for uncommitted changes
    if ! git diff-index --quiet HEAD -- 2>/dev/null; then
        log_warning "You have uncommitted changes!"
        read -p "Continue and discard changes? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            log "Cancelled by user"
            exit 1
        fi
        log "Discarding changes..."
        git reset --hard HEAD
    fi
    
    # Fetch latest changes
    log "Fetching latest changes from remote..."
    git fetch origin >> "$LOG_FILE" 2>&1
    
    # Checkout the PR
    log "Checking out PR #$PR_NUMBER..."
    if ! git fetch origin pull/$PR_NUMBER/head:pr-$PR_NUMBER >> "$LOG_FILE" 2>&1; then
        log_error "Failed to fetch PR #$PR_NUMBER"
        log_error "Make sure the PR number is valid"
        exit 1
    fi
    
    if ! git checkout pr-$PR_NUMBER >> "$LOG_FILE" 2>&1; then
        log_error "Failed to checkout PR #$PR_NUMBER"
        exit 1
    fi
    
    log_success "Successfully checked out PR #$PR_NUMBER"
}

multi_version_mode() {
    log "Mode: Multi-Version (creating separate directory)"
    log "============================================================"
    
    # Create workspace directory if it doesn't exist
    if [ ! -d "$WORKSPACE_DIR" ]; then
        log "Creating workspace directory: $WORKSPACE_DIR"
        mkdir -p "$WORKSPACE_DIR"
    fi
    
    PR_DIR="$WORKSPACE_DIR/pr-$PR_NUMBER"
    
    # Check if PR directory already exists
    if [ -d "$PR_DIR" ]; then
        log_warning "Directory already exists: $PR_DIR"
        read -p "Overwrite existing directory? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            log "Using existing directory"
            return 0
        fi
        log "Removing existing directory..."
        rm -rf "$PR_DIR"
    fi
    
    # Clone the repository
    log "Cloning repository to $PR_DIR..."
    if ! git clone "$REMOTE_URL" "$PR_DIR" >> "$LOG_FILE" 2>&1; then
        log_error "Failed to clone repository"
        exit 1
    fi
    
    # Fetch and checkout the PR
    cd "$PR_DIR"
    log "Fetching PR #$PR_NUMBER..."
    if ! git fetch origin pull/$PR_NUMBER/head:pr-$PR_NUMBER >> "$LOG_FILE" 2>&1; then
        log_error "Failed to fetch PR #$PR_NUMBER"
        log_error "Make sure the PR number is valid"
        exit 1
    fi
    
    if ! git checkout pr-$PR_NUMBER >> "$LOG_FILE" 2>&1; then
        log_error "Failed to checkout PR #$PR_NUMBER"
        exit 1
    fi
    
    log_success "Successfully created PR workspace at $PR_DIR"
    cd "$SCRIPT_DIR"
}

if [ "$MODE" = "in-place" ]; then
    in_place_mode
else
    multi_version_mode
fi

# ============================================================================
# Build the Project (if requested)
# ============================================================================
if [ "$AUTO_BUILD" -eq 1 ]; then
    log "Building project..."
    
    if [ "$MODE" = "multi-version" ]; then
        BUILD_ROOT="$WORKSPACE_DIR/pr-$PR_NUMBER"
    else
        BUILD_ROOT="$ROOT_DIR"
    fi
    
    log "Build directory: $BUILD_ROOT"
    
    # Create build directory and run CMake
    cd "$BUILD_ROOT"
    if [ -d "build" ]; then
        log "Cleaning existing build directory..."
        rm -rf build
    fi
    
    mkdir -p build
    cd build
    
    log "Running CMake..."
    if cmake .. >> "$LOG_FILE" 2>&1; then
        log "Running build..."
        if cmake --build . >> "$LOG_FILE" 2>&1; then
            log_success "Build completed successfully"
        else
            log_warning "Build failed! Check log for details."
        fi
    else
        log_warning "CMake configuration failed! Check log for details."
    fi
fi

# ============================================================================
# Success Summary
# ============================================================================
log "============================================================"
log_success "PR #$PR_NUMBER ready for testing!"
log "============================================================"

if [ "$MODE" = "multi-version" ]; then
    log "PR Location: $WORKSPACE_DIR/pr-$PR_NUMBER"
    echo ""
    log "To test the PR, navigate to:"
    log_success "  cd $WORKSPACE_DIR/pr-$PR_NUMBER"
else
    log "PR checked out in current directory"
fi

echo ""
log "To test another PR, run:"
log_success "  $SCRIPT_DIR/test-pr.sh --pr <number>"
echo ""

log "PR testing script completed successfully"
exit 0
