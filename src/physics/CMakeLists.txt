# Physics module - collision detection and physics simulation

add_library(OGDEPhysics STATIC
    PhysicsEngine.cpp
)

target_include_directories(OGDEPhysics
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(OGDEPhysics PUBLIC OGDE::Core)

add_library(OGDE::Physics ALIAS OGDEPhysics)

install(TARGETS OGDEPhysics
        EXPORT OpenGameDevEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
