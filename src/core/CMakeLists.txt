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

# Link to platform library
target_link_libraries(OGDECore PUBLIC OGDE::Platform)

add_library(OGDE::Core ALIAS OGDECore)

install(TARGETS OGDECore
        EXPORT OpenGameDevEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
