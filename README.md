# OpenGameDevEngine

A modular game development engine template built with C++17.

This project provides a well-organized structure and build system foundation for building a game engine.

## Features (Planned)

- **Modular Architecture**: Independent subsystems that can be used separately
- **Graphics Rendering**: Modern rendering pipeline with OpenGL/Vulkan support
- **Audio System**: Multi-channel audio with support for music and sound effects
- **Input Handling**: Keyboard, mouse, and controller input support
- **Physics Engine**: 2D/3D physics simulation and collision detection
- **Scripting**: Embedded scripting support for game logic
- **Networking**: Built-in multiplayer networking capabilities
- **Cross-platform**: Support for Windows, Linux, and macOS

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
│   ├── basic/        # Basic usage example
│   ├── 2d-platformer/# 2D game example
│   └── 3d-demo/      # 3D graphics demo
├── tests/            # Unit tests
├── tools/            # Development tools
├── docs/             # Documentation
├── assets/           # Game assets
└── external/         # Third-party dependencies
```

## Building

### Quick Start (Windows)

For first-time users on Windows:

```batch
# 1. Check prerequisites
scripts\setup.bat

# 2. Build the engine
scripts\build.bat

# 3. Run the example
build\bin\Release\BasicExample.exe
```

For more build options:
```batch
scripts\build.bat --help
```

### Manual Build

See [BUILD.md](BUILD.md) for detailed build instructions and manual build process.

Quick manual build:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Examples

Check out the `examples/` directory for sample projects demonstrating engine features.

## Documentation

- [API Documentation](docs/api/README.md)
- [Tutorials](docs/tutorials/README.md)
- [Architecture](docs/design/ARCHITECTURE.md)

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Status

🚧 **This project is in early development** 🚧

The current repository provides a feature layout and building structure template for future development.
