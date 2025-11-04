# PR Testing Quick Reference

This guide provides quick examples of using the PR testing tool.

## Quick Start

### Windows
```batch
# Test PR #42 (replaces current directory)
scripts\test-pr.bat --pr 42

# Test PR #42 in separate directory
scripts\test-pr.bat --pr 42 --multi-version
```

### Linux/macOS
```bash
# Test PR #42 (replaces current directory)
./scripts/test-pr.sh --pr 42

# Test PR #42 in separate directory
./scripts/test-pr.sh --pr 42 --multi-version
```

## Use Cases

### Scenario 1: Quick Test of a Single PR
**Goal:** Test PR #42 quickly in the current directory

**Windows:**
```batch
scripts\test-pr.bat --pr 42
```

**Linux/macOS:**
```bash
./scripts/test-pr.sh --pr 42
```

**What happens:**
1. Your current directory is updated with PR #42
2. The PR is fetched and checked out
3. The project is automatically built
4. You can immediately test the changes

### Scenario 2: Compare Multiple PRs Side-by-Side
**Goal:** Test and compare PRs #42, #43, and #44

**Windows:**
```batch
scripts\test-pr.bat --pr 42 --multi-version
scripts\test-pr.bat --pr 43 --multi-version
scripts\test-pr.bat --pr 44 --multi-version
```

**Linux/macOS:**
```bash
./scripts/test-pr.sh --pr 42 --multi-version
./scripts/test-pr.sh --pr 43 --multi-version
./scripts/test-pr.sh --pr 44 --multi-version
```

**Result:**
```
pr-workspace/
  pr-42/    # PR #42 - Feature A
  pr-43/    # PR #43 - Feature B
  pr-44/    # PR #44 - Bug fix
```

Each PR is in its own directory with a complete build!

### Scenario 3: Test Without Building
**Goal:** Just checkout the PR to review code, don't build

**Windows:**
```batch
scripts\test-pr.bat --pr 42 --multi-version --no-build
```

**Linux/macOS:**
```bash
./scripts/test-pr.sh --pr 42 --multi-version --no-build
```

### Scenario 4: Auto-Detect Next Open PR
**Goal:** Automatically find and test the next open PR

**Requirement:** GitHub CLI (gh) must be installed

**Windows:**
```batch
scripts\test-pr.bat
```

**Linux/macOS:**
```bash
./scripts/test-pr.sh
```

## Options Reference

| Option | Description |
|--------|-------------|
| `--pr NUMBER` | Specify which PR to test |
| `--multi-version` | Create separate directory for each PR |
| `--in-place` | Update current directory (default) |
| `--no-build` | Skip automatic build |
| `--workspace PATH` | Custom workspace location |
| `--help` | Show help message |

## Tips

1. **Use multi-version mode when:**
   - Testing multiple PRs
   - Comparing different implementations
   - You don't want to modify your current working directory

2. **Use in-place mode when:**
   - Quick testing of a single PR
   - You don't need to preserve your current state
   - Disk space is limited

3. **Install GitHub CLI (gh) for:**
   - Auto-detection of open PRs
   - Easier workflow without looking up PR numbers
   - Download: https://cli.github.com/

## Troubleshooting

### "Git not found"
- Install Git from https://git-scm.com/

### "Failed to fetch PR"
- Check that the PR number is valid
- Ensure you have internet connectivity
- Verify the repository URL is correct

### "Could not automatically fetch PR number"
- Install GitHub CLI: https://cli.github.com/
- Or specify PR manually: `--pr <number>`

### "Build failed"
- Check the log file in the `logs/` directory
- Build dependencies may be missing
- Try running the build manually

## Examples with Real Workflow

### Example 1: Test Today's PRs
```bash
# Morning: Test PR #42
./scripts/test-pr.sh --pr 42 --multi-version

# Afternoon: Test PR #43 for comparison
./scripts/test-pr.sh --pr 43 --multi-version

# Now you have both versions to compare!
cd pr-workspace/pr-42 && ./build/bin/Release/BasicExample
cd ../pr-43 && ./build/bin/Release/BasicExample
```

### Example 2: Sequential Testing
```bash
# Test PR #42
./scripts/test-pr.sh --pr 42

# Found an issue? Switch to PR #43
./scripts/test-pr.sh --pr 43

# All changes are in-place, no extra directories
```

### Example 3: Review Code Only
```bash
# Just checkout without building
./scripts/test-pr.sh --pr 42 --multi-version --no-build

# Review the code
cd pr-workspace/pr-42
code .  # or your favorite editor
```
