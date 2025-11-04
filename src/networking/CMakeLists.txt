# Networking module - multiplayer and network communication

add_library(OGDENetworking STATIC
    NetworkManager.cpp
)

target_include_directories(OGDENetworking
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(OGDENetworking PUBLIC OGDE::Core)

add_library(OGDE::Networking ALIAS OGDENetworking)

install(TARGETS OGDENetworking
        EXPORT OpenGameDevEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
