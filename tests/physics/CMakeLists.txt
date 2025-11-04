# Physics module tests

add_executable(PhysicsTests test_physics.cpp)

target_link_libraries(PhysicsTests PRIVATE OGDE::Physics)

add_test(NAME PhysicsTests COMMAND PhysicsTests)
