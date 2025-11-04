# Core module tests

add_executable(CoreTests test_engine.cpp)

target_link_libraries(CoreTests PRIVATE OGDE::Core)

add_test(NAME CoreTests COMMAND CoreTests)
