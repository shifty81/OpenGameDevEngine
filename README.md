# OpenGameDevEngine

A modular game development engine built with C++17, featuring DirectX 11 rendering.

This project provides a well-organized structure and build system foundation for building a game engine with a focus on Windows-native DirectX support.

## Features

### Implemented ✅
- **Modular Architecture**: Independent subsystems that can be used separately
- **Platform Layer**: Windows Win32 window management with cross-platform timer support
- **Core Engine**: Main game loop with delta time, FPS monitoring, and callback system
- **DirectX 11 Rendering**: 
  - Device and swap chain initialization
  - Render target and depth stencil management
  - Clear screen and color rendering
  - Shader compilation system (HLSL)
  - Window resize support
  - Vertex buffer management
  - Triangle and primitive rendering
- **Logging System**: Multi-level logging with timestamps and file output
- **Build Automation**: CMake-based build system with Windows batch scripts

### Planned 🚧
- **Audio System**: Multi-channel audio with support for music and sound effects
- **Input Handling**: Keyboard, mouse, and controller input support
- **Physics Engine**: 2D/3D physics simulation and collision detection
- **Scripting**: Embedded scripting support for game logic
- **Networking**: Built-in multiplayer networking capabilities
- **Advanced Graphics**: Triangle/mesh rendering, textures, lighting, and effects

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

Check out the `examples/` directory for sample projects demonstrating engine features:

- **BasicExample**: Minimal engine initialization and main loop
- **TriangleDemo**: DirectX 11 rendering with color-cycling background

See [examples/README.md](examples/README.md) for more information.

## Documentation

- [API Documentation](docs/api/README.md)
- [Tutorials](docs/tutorials/README.md)
- [Architecture](docs/design/ARCHITECTURE.md)
- [Development Roadmap](docs/ROADMAP.md)
- [Implementation Summary](docs/IMPLEMENTATION_SUMMARY.md)

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Status

🚀 **Phase 1: Foundation - 85% Complete** 🚀

The engine has successfully implemented:
- ✅ Core engine loop and timing system
- ✅ Platform abstraction layer (Windows/Linux)
- ✅ DirectX 11 rendering foundation
- ✅ Shader compilation system
- ✅ Vertex buffer management
- ✅ Triangle rendering with colored vertices
- ✅ Comprehensive logging system
- ✅ Build automation

Next up: Camera system and mesh loading!

See [ROADMAP.md](docs/ROADMAP.md) for the full development plan.
