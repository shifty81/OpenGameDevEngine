# OpenGameDevEngine Architecture

## Overview

OpenGameDevEngine is a modular, Windows-native game engine built with C++17, designed for creating voxel-based games with a focus on mod-ability and extensibility. The engine is built around an Entity-Component-System (ECS) architecture and uses DirectX 11 for rendering.

## Design Principles

1. **Modular Architecture**: Independent subsystems that can be developed and tested separately
2. **Windows-Native**: Built specifically for Windows using Win32 API and DirectX
3. **Data-Driven Design**: Separates game logic from engine code
4. **Performance-Oriented**: Optimized for real-time 3D rendering and physics simulation
5. **Extensible**: Easy to add new features through plugins and scripting
6. **Open Source**: MIT licensed for community contributions

## Core Architecture

### High-Level System Overview

```
┌─────────────────────────────────────────────────────────────┐
│                      Application Layer                       │
│            (Game-specific code, Editor, Tools)              │
└─────────────────────────────────────────────────────────────┘
                              │
┌─────────────────────────────────────────────────────────────┐
│                      Engine Core Layer                       │
├─────────────────────────────────────────────────────────────┤
│  Engine  │  Logger  │  Timer  │  Resource Manager  │  ECS  │
└─────────────────────────────────────────────────────────────┘
                              │
┌─────────────────────────────────────────────────────────────┐
│                    Subsystem Layer                           │
├──────────┬──────────┬──────────┬──────────┬─────────────────┤
│ Graphics │  Audio   │  Input   │ Physics  │   Scripting    │
│ (DirectX)│ (OpenAL) │ (Win32)  │ (PhysX)  │    (Lua)       │
└──────────┴──────────┴──────────┴──────────┴─────────────────┘
                              │
┌─────────────────────────────────────────────────────────────┐
│                    Platform Layer                            │
├─────────────────────────────────────────────────────────────┤
│   Windows (Win32 API)  │  DirectX  │  File System  │  I/O  │
└─────────────────────────────────────────────────────────────┘
```

## Core Systems

### 1. Platform Layer (`ogde::platform`)

The platform layer provides Windows-specific functionality and abstractions.

**Components:**
- `Platform` - System initialization, high-resolution timer, platform utilities
- `WindowWin32` - Native Win32 window management and message handling

**Responsibilities:**
- Window creation and management (Win32)
- High-precision timing (QueryPerformanceCounter)
- Input event handling
- Platform-specific utilities

**Key Files:**
- `include/ogde/platform/Platform.h`
- `include/ogde/platform/WindowWin32.h`
- `src/platform/Platform.cpp`
- `src/platform/WindowWin32.cpp`

### 2. Core Engine (`ogde::core`)

The core engine manages the main game loop and coordinates all subsystems.

**Components:**
- `Engine` - Main engine class with game loop, timing, and subsystem management
- `Logger` - Logging system with file output and multiple severity levels
- `Math` - Mathematical utilities and data structures

**Game Loop Flow:**
```
Initialize Engine
├─ Initialize Platform
├─ Create Window
├─ Initialize Graphics
├─ Initialize Audio
├─ Initialize Physics
└─ Initialize Other Subsystems

Main Loop:
┌─────────────────────────┐
│ Process Window Messages │
├─────────────────────────┤
│ Update Delta Time       │
├─────────────────────────┤
│ Update Callback         │ ← Game logic
├─────────────────────────┤
│ Render Callback         │ ← Rendering
├─────────────────────────┤
│ Update FPS Counter      │
├─────────────────────────┤
│ Frame Rate Limiting     │
└─────────────────────────┘
    │ Loop until quit
    
Shutdown Engine
├─ Shutdown Subsystems
├─ Destroy Window
└─ Shutdown Platform
```

**Key Features:**
- Delta time calculation for frame-independent updates
- FPS monitoring and display
- Configurable frame rate limiting
- Callback system for update and render logic

**Key Files:**
- `include/ogde/core/Engine.h`
- `include/ogde/core/Logger.h`
- `src/core/Engine.cpp`
- `src/core/Logger.cpp`

### 3. Graphics System (`ogde::graphics`)

The graphics system handles all visual rendering using DirectX 11.

**Planned Components:**
- `Renderer` - Main rendering interface
- `RenderDevice` - DirectX device abstraction
- `ShaderManager` - Shader compilation and management
- `MaterialSystem` - Material and texture management
- `Camera` - Camera transformations and projections

**DirectX 11 Pipeline:**
```
┌─────────────────────┐
│ DirectX Device Init │
├─────────────────────┤
│ Create Swap Chain   │
├─────────────────────┤
│ Render Target Setup │
└─────────────────────┘
         │
         ▼
┌─────────────────────┐
│  Vertex Processing  │ ← Vertex Shader
├─────────────────────┤
│ Rasterization       │
├─────────────────────┤
│  Pixel Processing   │ ← Pixel Shader
├─────────────────────┤
│  Output Merger      │
└─────────────────────┘
         │
         ▼
   [Swap Chain Present]
```

**Status:** Basic structure in place, DirectX integration pending

**Key Files:**
- `include/ogde/graphics/Renderer.h`
- `include/ogde/graphics/Window.h`
- `src/graphics/Renderer.cpp`

### 4. Audio System (`ogde::audio`)

Audio playback and spatial audio management.

**Planned Components:**
- `AudioEngine` - Audio system initialization and management
- `AudioSource` - 3D positioned audio sources
- `AudioListener` - Player audio receiver

**Status:** Placeholder implementation

**Key Files:**
- `include/ogde/audio/AudioEngine.h`
- `src/audio/AudioEngine.cpp`

### 5. Input System (`ogde::input`)

Handles keyboard, mouse, and game controller input.

**Planned Components:**
- `InputManager` - Input state management and event distribution
- `Keyboard` - Keyboard state tracking
- `Mouse` - Mouse position and button state
- `GameController` - Game controller/gamepad support

**Status:** Placeholder implementation, window callbacks ready

**Key Files:**
- `include/ogde/input/InputManager.h`
- `src/input/InputManager.cpp`

### 6. Physics System (`ogde::physics`)

3D physics simulation and collision detection.

**Planned Components:**
- `PhysicsEngine` - Physics world management
- `RigidBody` - Dynamic physics objects
- `Collider` - Collision shapes (box, sphere, mesh)
- `PhysicsMaterial` - Friction and bounce properties

**Status:** Placeholder implementation

**Key Files:**
- `include/ogde/physics/PhysicsEngine.h`
- `src/physics/PhysicsEngine.cpp`

### 7. Scripting System (`ogde::scripting`)

Lua scripting integration for game logic and modding.

**Planned Components:**
- `ScriptEngine` - Lua state management
- `ScriptBinding` - C++ to Lua bindings
- API exposure for game objects and systems

**Status:** Placeholder implementation

**Key Files:**
- `include/ogde/scripting/ScriptEngine.h`
- `src/scripting/ScriptEngine.cpp`

### 8. Networking System (`ogde::networking`)

Client-server networking for multiplayer.

**Planned Components:**
- `NetworkManager` - Network initialization
- `NetworkClient` - Client connection management
- `NetworkServer` - Server hosting and client management
- `NetworkPacket` - Message serialization

**Status:** Placeholder implementation

**Key Files:**
- `include/ogde/networking/NetworkManager.h`
- `src/networking/NetworkManager.cpp`

## Entity-Component-System (ECS)

The ECS architecture separates data (Components) from behavior (Systems) with Entities serving as unique identifiers.

### Planned ECS Design

```
Entity (ID)
    │
    ├─ TransformComponent (position, rotation, scale)
    ├─ MeshComponent (3D model data)
    ├─ PhysicsComponent (rigid body, collider)
    ├─ ScriptComponent (Lua script reference)
    └─ CustomComponents...

Systems:
    ├─ RenderSystem (processes MeshComponent + TransformComponent)
    ├─ PhysicsSystem (processes PhysicsComponent)
    ├─ ScriptSystem (processes ScriptComponent)
    └─ CustomSystems...
```

**Benefits:**
- Cache-friendly data layout
- Easy serialization
- Dynamic composition of game objects
- Efficient parallel processing
- Modding-friendly

## Voxel System (Game-Specific)

For voxel-based games like the described vintage story-inspired project:

**Planned Components:**
- `VoxelWorld` - World chunk management
- `VoxelChunk` - Individual chunk data (16x16x16 blocks)
- `VoxelBlock` - Block type definitions
- `VoxelRenderer` - Optimized voxel rendering (greedy meshing)
- `VoxelPhysics` - Block collision and physics

## Memory Management

**Strategy:**
- Custom allocators for frequently allocated objects
- Object pooling for entities and components
- Resource caching and reference counting
- Memory tracking in debug builds

## Threading Model

**Main Thread:**
- Window message processing
- Input handling
- Game logic updates
- Rendering command submission

**Worker Threads:**
- Asset loading and streaming
- Physics simulation
- Procedural generation
- Network I/O

## Build System

### CMake Structure

```
CMakeLists.txt (root)
├─ src/CMakeLists.txt
│  ├─ platform/CMakeLists.txt
│  ├─ core/CMakeLists.txt
│  ├─ graphics/CMakeLists.txt
│  ├─ audio/CMakeLists.txt
│  ├─ input/CMakeLists.txt
│  ├─ physics/CMakeLists.txt
│  ├─ scripting/CMakeLists.txt
│  └─ networking/CMakeLists.txt
├─ examples/CMakeLists.txt
├─ tests/CMakeLists.txt
└─ tools/CMakeLists.txt
```

### Build Automation

- **setup.bat** - Verify prerequisites (CMake, Visual Studio, Git)
- **build.bat** - One-click build with logging
- Extensive logging to `logs/` directory
- Support for Debug and Release configurations

## File Organization

```
OpenGameDevEngine/
├─ include/ogde/          # Public API headers
│  ├─ platform/
│  ├─ core/
│  ├─ graphics/
│  ├─ audio/
│  ├─ input/
│  ├─ physics/
│  ├─ scripting/
│  └─ networking/
├─ src/                   # Implementation files
│  ├─ platform/
│  ├─ core/
│  ├─ graphics/
│  └─ ...
├─ examples/              # Example projects
│  ├─ basic/
│  ├─ 2d-platformer/
│  └─ 3d-demo/
├─ tests/                 # Unit tests
├─ tools/                 # Development tools
├─ scripts/               # Build automation
├─ docs/                  # Documentation
├─ assets/                # Game assets
└─ external/              # Third-party libraries
```

## Dependencies

### Required:
- **Windows SDK** - Win32 API, DirectX 11
- **CMake 3.15+** - Build system
- **MSVC 2017+** - C++17 compiler

### Planned:
- **DirectX 11** - Graphics rendering (included in Windows SDK)
- **PhysX** - Physics simulation
- **Lua** - Scripting engine
- **OpenAL** or **XAudio2** - Audio engine
- **Dear ImGui** - Debug UI and editor interface
- **stb_image** - Image loading
- **Assimp** - 3D model loading

## Future Development

### Phase 1: Foundation (Current)
- ✅ Platform layer
- ✅ Core engine loop
- ✅ Logger system
- ✅ Build automation
- ⏳ DirectX 11 rendering
- ⏳ Basic ECS implementation

### Phase 2: Core Systems
- Input management
- Audio system
- Physics integration
- Resource management
- Asset pipeline

### Phase 3: Game Systems
- Voxel rendering
- Procedural generation
- Inventory system
- Save/load system

### Phase 4: Tools & Editor
- In-engine editor
- Debug console
- Performance profiler
- Asset browser
- Script editor

### Phase 5: Advanced Features
- Networking
- AI system
- Advanced graphics (PBR, shadows, post-processing)
- Modding support
- Documentation and tutorials

## Performance Targets

- **Frame Rate:** 60 FPS minimum on mid-range hardware
- **Memory:** < 512MB for engine core
- **Load Times:** < 3 seconds for typical scenes
- **Voxel Rendering:** 1M+ visible blocks at 60 FPS

## References

- [DirectX 11 Programming Guide](https://docs.microsoft.com/en-us/windows/win32/direct3d11/dx-graphics-overviews)
- [Win32 API Documentation](https://docs.microsoft.com/en-us/windows/win32/api/)
- [Entity-Component-System Architecture](https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/understanding-component-entity-systems-r3013/)
- [Game Engine Architecture by Jason Gregory](https://www.gameenginebook.com/)
