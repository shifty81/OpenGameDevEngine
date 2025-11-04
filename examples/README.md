# Examples

This directory contains example projects demonstrating OpenGameDevEngine features.

## Available Examples

### Basic Example
Location: `basic/`

A minimal example showing how to initialize and run the engine with basic timing and FPS monitoring.

**Features Demonstrated:**
- Engine initialization
- Main loop execution
- FPS calculation
- Update and render callbacks
- Platform abstraction

**Running:**
```bash
./bin/BasicExample
```

### Triangle Demo (DirectX 11)
Location: `3d-demo/triangle_demo.cpp`

Demonstrates DirectX 11 rendering with a colored triangle on a color-cycling background.

**Features Demonstrated:**
- DirectX 11 device initialization
- Swap chain management
- Vertex buffer creation and management
- Shader compilation and binding (HLSL)
- Triangle rendering with per-vertex colors
- Dynamic background color animation

**Requirements:** Windows with DirectX 11 support

**Running:**
```bash
./bin/TriangleDemo
```

**What to Expect:** A window will open displaying a colored triangle (red, green, and blue vertices) on a slowly color-cycling background.

### 2D Platformer
Location: `2d-platformer/`

Example of a 2D platformer game (coming soon).

### 3D Demo
Location: `3d-demo/main.cpp`

Example of 3D graphics rendering (coming soon).

## Building Examples

Examples are built automatically when `OGDE_BUILD_EXAMPLES` is enabled (default):

```bash
cmake -DOGDE_BUILD_EXAMPLES=ON ..
cmake --build .
```

Executables will be output to `build/bin/` directory.

## Platform Compatibility

| Example | Windows | Linux | macOS |
|---------|---------|-------|-------|
| BasicExample | ✅ | ✅ | ✅ |
| TriangleDemo | ✅ | ❌* | ❌* |
| 2DPlatformer | 🚧 | 🚧 | 🚧 |
| 3DDemo | 🚧 | 🚧 | 🚧 |

*DirectX 11 examples require Windows. Linux/macOS builds will compile but show a platform message.

## Creating Your Own Example

1. Create a new directory under `examples/`
2. Add a `CMakeLists.txt` file
3. Add your source files
4. Update `examples/CMakeLists.txt` to include your example

Example CMakeLists.txt:
```cmake
add_executable(MyExample main.cpp)
target_link_libraries(MyExample PRIVATE OGDE::Engine)
install(TARGETS MyExample DESTINATION bin/examples)
```

## Learning Path

1. **Start with BasicExample** - Learn the fundamentals of engine initialization and the main loop
2. **Explore TriangleDemo** - Understand DirectX 11 rendering basics
3. **Study 2DPlatformer** (upcoming) - Learn about 2D game mechanics
4. **Dive into 3DDemo** (upcoming) - Master 3D graphics and rendering

## Additional Resources

- [Engine API Documentation](../docs/api/README.md)
- [Tutorials](../docs/tutorials/README.md)
- [Architecture Guide](../docs/design/ARCHITECTURE.md)
