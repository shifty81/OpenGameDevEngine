# OpenGameDevEngine Source

# Platform library
add_subdirectory(platform)

# Core library (contains Logger which Graphics needs)
add_subdirectory(core)

# Graphics library (depends on Core for Logger)
add_subdirectory(graphics)

# Audio library
add_subdirectory(audio)

# Input library
add_subdirectory(input)

# Physics library
add_subdirectory(physics)

# Scripting library
add_subdirectory(scripting)

# Networking library
add_subdirectory(networking)

# Main engine library that combines all modules
add_library(OpenGameDevEngine INTERFACE)
target_link_libraries(OpenGameDevEngine INTERFACE
    OGDE::Platform
    OGDE::Core
    OGDE::Graphics
    OGDE::Audio
    OGDE::Input
    OGDE::Physics
    OGDE::Scripting
    OGDE::Networking
)

add_library(OGDE::Engine ALIAS OpenGameDevEngine)
