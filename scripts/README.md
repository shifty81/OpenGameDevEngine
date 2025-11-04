# Build Scripts

This directory contains automated build scripts and tools for OpenGameDevEngine.

## Available Scripts

- **setup.bat** - First-time setup and prerequisite checking (Windows)
- **build.bat** - Automated build script (Windows)
- **test-pr.bat** - Automated PR testing tool (Windows)
- **test-pr.sh** - Automated PR testing tool (Linux/macOS)

## For Windows Users

### First Time Setup

1. **Run setup.bat** to check if you have all required tools:
   ```batch
   scripts\setup.bat
   ```

2. This will check for:
   - CMake (required)
   - Visual Studio 2017+ (required)
   - Git (optional)

### Building the Engine

Run the build script:
```batch
scripts\build.bat
```

This will:
- Configure the project with CMake
- Build all libraries and examples
- Generate detailed logs in the `logs/` directory

### Build Options

```batch
scripts\build.bat --help           Show all available options
scripts\build.bat --clean          Clean build from scratch
scripts\build.bat --debug          Build in Debug mode
scripts\build.bat --release        Build in Release mode (default)
scripts\build.bat --run-examples   Build and run examples
scripts\build.bat --run-tests      Build and run tests
```

### Examples

```batch
REM Clean release build with examples
scripts\build.bat --clean --release --run-examples

REM Debug build with tests
scripts\build.bat --debug --run-tests

REM Quick rebuild
scripts\build.bat
```

## Build Logs

All build logs are saved to the `logs/` directory with timestamps.
If a build fails, check the latest log file for detailed error information.

## Requirements

### Windows
- **CMake 3.15+** - [Download](https://cmake.org/download/)
- **Visual Studio 2017+** with:
  - Desktop development with C++
  - Windows SDK
- **Git** (optional) - [Download](https://git-scm.com/download/win)

### DirectX
DirectX is included with Windows SDK (installed with Visual Studio).

## Troubleshooting

### CMake not found
- Install CMake from https://cmake.org/download/
- Make sure to check "Add CMake to system PATH" during installation
- Restart your terminal/command prompt

### Visual Studio not found
- Install Visual Studio 2017 or later
- During installation, select "Desktop development with C++"
- Make sure Windows SDK is included

### Build errors
- Check the log file in the `logs/` directory
- Make sure all prerequisites are installed
- Try running `scripts\build.bat --clean` to rebuild from scratch

## Testing Pull Requests

The PR testing tool allows you to easily test pull requests in two modes:

### Quick Start

#### Windows
```batch
REM Test a specific PR
scripts\test-pr.bat --pr 42

REM Test PR in separate directory (multi-version mode)
scripts\test-pr.bat --pr 42 --multi-version

REM Auto-detect next PR (requires gh CLI)
scripts\test-pr.bat
```

#### Linux/macOS
```bash
# Test a specific PR
./scripts/test-pr.sh --pr 42

# Test PR in separate directory (multi-version mode)
./scripts/test-pr.sh --pr 42 --multi-version

# Auto-detect next PR (requires gh CLI)
./scripts/test-pr.sh
```

### Modes

#### In-Place Mode (Default)
Updates your current directory with the PR code:
- Fetches the PR from the repository
- Checks out the PR branch locally
- Automatically builds the project
- Best for quick testing of a single PR

#### Multi-Version Mode
Creates separate directories for each PR:
- Clones the repository to a new directory (e.g., `pr-workspace/pr-42/`)
- Allows testing multiple PRs side-by-side
- Each PR is isolated in its own directory
- Perfect for comparing different PR versions

Example directory structure:
```
OpenGameDevEngine/
  pr-workspace/
    pr-42/    (PR #42 - Feature A)
    pr-43/    (PR #43 - Feature B)
    pr-44/    (PR #44 - Bug fix)
```

### Options

- `--pr NUMBER` - Specify PR number to test
- `--multi-version` - Create separate directory for each PR
- `--in-place` - Update current directory (default)
- `--no-build` - Skip automatic build after checkout
- `--workspace PATH` - Custom workspace directory for multi-version mode
- `--help` - Show detailed help message

### Requirements

- **Git** (required) - For fetching and checking out PRs
- **GitHub CLI (gh)** (optional) - For auto-detecting next open PR
  - Install from: https://cli.github.com/

### Examples

```batch
REM Windows: Test PR #42 in current directory and build
scripts\test-pr.bat --pr 42

REM Windows: Test PR #42 in separate directory without building
scripts\test-pr.bat --pr 42 --multi-version --no-build

REM Windows: Auto-detect and test next open PR
scripts\test-pr.bat
```

```bash
# Linux/macOS: Test PR #42 in current directory and build
./scripts/test-pr.sh --pr 42

# Linux/macOS: Test PR #42 in separate directory without building
./scripts/test-pr.sh --pr 42 --multi-version --no-build

# Linux/macOS: Auto-detect and test next open PR
./scripts/test-pr.sh
```

## Manual Build (Alternative)

If the scripts don't work, you can build manually:

```batch
REM Create build directory
mkdir build
cd build

REM Configure with CMake
cmake .. -G "Visual Studio 17 2022"

REM Build
cmake --build . --config Release

REM Run example
bin\Release\BasicExample.exe
```
