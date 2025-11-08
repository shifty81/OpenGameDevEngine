# OpenGameDevEngine

A modular game development engine built with C++17, featuring DirectX 11 rendering.

This project provides a well-organized structure and build system foundation for building a game engine with a focus on Windows-native DirectX support.

## Features

### Implemented ✅
- **Modular Architecture**: Independent subsystems that can be used separately
- **Platform Layer**: Windows Win32 window management with cross-platform timer support
- **Core Engine**: Main game loop with delta time, FPS monitoring, and callback system
- **File I/O System**: Cross-platform file operations with comprehensive path utilities
- **Configuration System**: JSON-based configuration management with type-safe access
- **DirectX 11 Rendering**: 
  - Device and swap chain initialization
  - Render target and depth stencil management
  - Clear screen and color rendering
  - Shader compilation system (HLSL)
  - Window resize support
  - Vertex buffer management
  - Triangle and primitive rendering
- **Texture System**:
  - Image loading (PNG, JPG, BMP, TGA via stb_image)
  - DirectX 11 texture resource management
  - Automatic format conversion
- **Material System**:
  - Material properties (diffuse, ambient, specular)
  - Multiple texture map support
  - Opacity and shininess controls
- **Camera System**: 
  - Perspective and orthographic projections
  - View and projection matrix generation
  - Look-at camera positioning
  - Camera direction vectors
- **Logging System**: Multi-level logging with timestamps and file output
- **Build Automation**: CMake-based build system with Windows batch scripts

### Planned 🚧
- **Mesh Loading**: OBJ and other 3D model formats
- **Basic Lighting**: Directional and point lights
- **Audio System**: Multi-channel audio with support for music and sound effects
- **Input Handling**: Keyboard, mouse, and controller input support
- **Physics Engine**: 2D/3D physics simulation and collision detection
- **Scripting**: Embedded scripting support for game logic
- **Networking**: Built-in multiplayer networking capabilities
- **Advanced Graphics**: PBR materials, shadows, and post-processing effects

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
- **CameraDemo**: 3D camera system with a rotating cube

See [examples/README.md](examples/README.md) for more information.

## Documentation

- [API Documentation](docs/api/README.md)
- [Tutorials](docs/tutorials/README.md)
- [Architecture](docs/design/ARCHITECTURE.md)
- [Development Roadmap](docs/ROADMAP.md)
- [Implementation Summary](docs/IMPLEMENTATION_SUMMARY.md)
- [PR Testing Guide](docs/PR_TESTING.md) - Test pull requests automatically

## Testing Pull Requests

Want to test the next version before it's released? Use our automated PR testing tool:

```batch
# Windows: Test any PR
scripts\test-pr.bat --pr 42

# Linux/macOS: Test any PR
./scripts/test-pr.sh --pr 42
```

The tool supports two modes:
- **In-place mode**: Update your current directory with the PR
- **Multi-version mode**: Create separate directories for each PR (`pr-workspace/pr-42/`)

See the [PR Testing Guide](docs/PR_TESTING.md) for detailed instructions and examples.

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Status

🚀 **Phase 1: Foundation - 100% Complete** 🚀
🎨 **Phase 2: Core Rendering - 40% Complete** 🎨

The engine has successfully implemented:

**Phase 1 - Foundation:**
- ✅ Core engine loop and timing system
- ✅ Platform abstraction layer (Windows/Linux)
- ✅ DirectX 11 rendering foundation
- ✅ Shader compilation system
- ✅ Vertex buffer management
- ✅ Triangle rendering with colored vertices
- ✅ Camera system (perspective and orthographic)
- ✅ Comprehensive logging system
- ✅ Build automation
- ✅ File I/O system
- ✅ JSON-based configuration system

**Phase 2 - Core Rendering (In Progress):**
- ✅ Texture loading system (stb_image integration)
- ✅ Material system with texture mapping
- 🚧 Shader management (hot-reloading, parameter binding)
- 🚧 Mesh loading (OBJ format)
- 🚧 Transform system (MVP matrices)
- 🚧 Basic lighting (directional and point lights)

Next up: Mesh loading and basic lighting system!

See [ROADMAP.md](docs/ROADMAP.md) for the full development plan.
