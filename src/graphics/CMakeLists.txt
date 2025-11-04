# Graphics module - rendering and visual components

set(GRAPHICS_SOURCES
    Renderer.cpp
)

# Add DirectX 11 renderer on Windows
if(WIN32)
    list(APPEND GRAPHICS_SOURCES 
        RendererD3D11.cpp
        Shader.cpp
    )
endif()

add_library(OGDEGraphics STATIC ${GRAPHICS_SOURCES})

target_include_directories(OGDEGraphics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Graphics depends on Core for Logger
target_link_libraries(OGDEGraphics PUBLIC OGDE::Core)

# Link DirectX 11 libraries on Windows
if(WIN32)
    target_link_libraries(OGDEGraphics PRIVATE d3d11 dxgi d3dcompiler)
endif()

add_library(OGDE::Graphics ALIAS OGDEGraphics)

install(TARGETS OGDEGraphics
        EXPORT OpenGameDevEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
