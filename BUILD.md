# Building OpenGameDevEngine

## Automated Build (Recommended for Windows)

### First Time Setup

Run the setup script to verify prerequisites:
```batch
scripts\setup.bat
```

### Building with Scripts

Use the automated build script:
```batch
scripts\build.bat
```

#### Build Options

- `--clean` - Clean build directory before building
- `--debug` - Build in Debug mode
- `--release` - Build in Release mode (default)
- `--run-examples` - Run examples after building
- `--run-tests` - Run unit tests after building
- `--help` - Show all options

#### Examples

```batch
# Clean release build
scripts\build.bat --clean --release

# Debug build with tests
scripts\build.bat --debug --run-tests

# Build and run examples
scripts\build.bat --run-examples
```

### Build Logs

All build logs are automatically saved to the `logs/` directory with timestamps for troubleshooting.

## Manual Build

## Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Git

## Building on Linux/macOS

```bash
# Clone the repository
git clone https://github.com/shifty81/OpenGameDevEngine.git
cd OpenGameDevEngine

# Create a build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# Run tests (optional)
ctest

# Install (optional)
sudo cmake --install .
```

## Building on Windows

```bash
# Clone the repository
git clone https://github.com/shifty81/OpenGameDevEngine.git
cd OpenGameDevEngine

# Create a build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . --config Release

# Run tests (optional)
ctest -C Release
```

## CMake Options

- `OGDE_BUILD_EXAMPLES` - Build example projects (default: ON)
- `OGDE_BUILD_TESTS` - Build tests (default: ON)
- `OGDE_BUILD_DOCS` - Build documentation (default: OFF)
- `OGDE_BUILD_TOOLS` - Build tools (default: ON)

Example:
```bash
cmake -DOGDE_BUILD_EXAMPLES=OFF -DOGDE_BUILD_TESTS=ON ..
```

## Project Structure

```
OpenGameDevEngine/
├── src/              # Engine source code
│   ├── core/         # Core functionality
│   ├── graphics/     # Graphics rendering
│   ├── audio/        # Audio system
│   ├── input/        # Input handling
│   ├── physics/      # Physics engine
│   ├── scripting/    # Scripting support
│   └── networking/   # Network communication
├── include/          # Public header files
│   └── ogde/         # Engine API headers
├── examples/         # Example projects
├── tests/            # Unit tests
├── tools/            # Development tools
├── docs/             # Documentation
├── assets/           # Game assets
└── external/         # Third-party dependencies
```

## Troubleshooting

### CMake not found
Ensure CMake is installed and in your PATH.

### Compiler errors
Make sure you have a C++17 compatible compiler installed.

### Build errors
Try cleaning the build directory and rebuilding:
```bash
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```
