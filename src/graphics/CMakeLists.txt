# Graphics module - rendering and visual components

add_library(OGDEGraphics STATIC
    Renderer.cpp
)

target_include_directories(OGDEGraphics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(OGDEGraphics PUBLIC OGDE::Core)

add_library(OGDE::Graphics ALIAS OGDEGraphics)

install(TARGETS OGDEGraphics
        EXPORT OpenGameDevEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
