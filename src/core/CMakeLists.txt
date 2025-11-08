# Core module - fundamental engine functionality

add_library(OGDECore STATIC
    Engine.cpp
    Logger.cpp
    FileSystem.cpp
    Config.cpp
)

target_include_directories(OGDECore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external
)

# Core links to platform library
# Graphics is linked privately since Engine.cpp needs it, but consumers don't need to link it directly
target_link_libraries(OGDECore 
    PUBLIC OGDE::Platform
    PRIVATE OGDE::Graphics
)

add_library(OGDE::Core ALIAS OGDECore)

install(TARGETS OGDECore
        EXPORT OpenGameDevEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
