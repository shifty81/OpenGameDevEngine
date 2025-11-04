# OpenGameDevEngine API Documentation

This directory contains API documentation for the OpenGameDevEngine.

## Modules

### Core Module
The core module provides fundamental engine functionality including:
- Engine initialization and management
- Logging system
- Mathematical utilities (vectors, matrices)

See: `include/ogde/core/`

### Graphics Module
The graphics module handles rendering and visual components:
- Window management
- Rendering system
- Shader management (planned)
- Texture loading (planned)

See: `include/ogde/graphics/`

### Audio Module
The audio module manages sound and music:
- Audio playback
- Sound effects
- Background music
- Volume control

See: `include/ogde/audio/`

### Input Module
The input module handles user input:
- Keyboard input
- Mouse input
- Controller support (planned)

See: `include/ogde/input/`

### Physics Module
The physics module provides physics simulation:
- Collision detection
- Rigid body dynamics
- Physics world management

See: `include/ogde/physics/`

### Scripting Module
The scripting module enables game logic scripting:
- Script execution
- Lua integration (planned)
- API bindings (planned)

See: `include/ogde/scripting/`

### Networking Module
The networking module handles multiplayer communication:
- Client/server architecture
- Network messaging
- Connection management

See: `include/ogde/networking/`

## Building Documentation

Documentation will be generated using Doxygen (planned).

```bash
cmake -DOGDE_BUILD_DOCS=ON ..
cmake --build . --target docs
```
