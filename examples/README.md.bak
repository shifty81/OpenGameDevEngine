# Examples

This directory contains example projects demonstrating OpenGameDevEngine features.

## Available Examples

### Basic Example
Location: `basic/`

A minimal example showing how to initialize and run the engine.

```bash
./bin/examples/BasicExample
```

### 2D Platformer
Location: `2d-platformer/`

Example of a 2D platformer game (coming soon).

### 3D Demo
Location: `3d-demo/`

Example of 3D graphics rendering (coming soon).

## Building Examples

Examples are built automatically when `OGDE_BUILD_EXAMPLES` is enabled (default):

```bash
cmake -DOGDE_BUILD_EXAMPLES=ON ..
cmake --build .
```

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
