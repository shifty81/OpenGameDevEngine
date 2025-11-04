# Input module - keyboard, mouse, and controller input

add_library(OGDEInput STATIC
    InputManager.cpp
)

target_include_directories(OGDEInput
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(OGDEInput PUBLIC OGDE::Core)

add_library(OGDE::Input ALIAS OGDEInput)

install(TARGETS OGDEInput
        EXPORT OpenGameDevEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
