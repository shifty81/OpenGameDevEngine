# Platform module - Windows-specific functionality

add_library(OGDEPlatform STATIC
    Platform.cpp
)

# Add Windows-specific files on Windows
if(WIN32)
    target_sources(OGDEPlatform PRIVATE
        WindowWin32.cpp
    )
endif()

target_include_directories(OGDEPlatform
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link Windows libraries
if(WIN32)
    target_link_libraries(OGDEPlatform PRIVATE
        user32
        gdi32
    )
endif()

add_library(OGDE::Platform ALIAS OGDEPlatform)

install(TARGETS OGDEPlatform
        EXPORT OpenGameDevEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
