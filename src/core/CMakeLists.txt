# Core module - fundamental engine functionality

add_library(OGDECore STATIC
    Engine.cpp
    Logger.cpp
)

target_include_directories(OGDECore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Core links to platform and graphics libraries
# Note: This works because Graphics is header-only dependent on Core (Logger.h)
# and Core is source-level dependent on Graphics (Renderer.cpp)
target_link_libraries(OGDECore PUBLIC OGDE::Platform OGDE::Graphics)

add_library(OGDE::Core ALIAS OGDECore)

install(TARGETS OGDECore
        EXPORT OpenGameDevEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
