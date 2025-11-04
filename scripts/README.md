# Build Scripts

This directory contains automated build scripts for OpenGameDevEngine.

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
