# External Dependencies

This directory is for third-party libraries and dependencies.

## Managing Dependencies

You can use various methods to manage dependencies:

1. **Git Submodules**
   ```bash
   git submodule add <repository-url> external/<library-name>
   ```

2. **CMake FetchContent**
   Add to CMakeLists.txt:
   ```cmake
   include(FetchContent)
   FetchContent_Declare(
     library_name
     GIT_REPOSITORY https://github.com/user/repo.git
     GIT_TAG v1.0.0
   )
   FetchContent_MakeAvailable(library_name)
   ```

3. **Package Managers**
   - vcpkg
   - Conan
   - System package managers (apt, brew, etc.)

## Recommended Libraries

Suggested third-party libraries for future integration:

- **Graphics**: GLFW, SDL2
- **Audio**: OpenAL, FMOD, Wwise
- **Physics**: Bullet Physics, PhysX
- **Scripting**: Lua, ChaiScript
- **Networking**: ENet, RakNet
- **Math**: GLM
- **Testing**: Google Test, Catch2

## License Compliance

When adding dependencies, ensure:
- License compatibility with MIT License
- License files are included
- Attribution is provided where required
