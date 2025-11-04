cmake_minimum_required(VERSION 3.20)
project(OpenGameDevEngine VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(OGDE_BUILD_EXAMPLES "Build example projects" ON)
option(OGDE_BUILD_TESTS "Build tests" ON)
option(OGDE_BUILD_DOCS "Build documentation" OFF)
option(OGDE_BUILD_TOOLS "Build tools" ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(src)

if(OGDE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(OGDE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(OGDE_BUILD_TOOLS)
    add_subdirectory(tools)
endif()

# Installation rules
install(DIRECTORY include/ogde 
        DESTINATION include
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

# Package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/OpenGameDevEngineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Export targets
install(EXPORT OpenGameDevEngineTargets
        FILE OpenGameDevEngineTargets.cmake
        NAMESPACE OGDE::
        DESTINATION lib/cmake/OpenGameDevEngine)
