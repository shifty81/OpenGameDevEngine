# Input module tests

add_executable(InputTests test_input.cpp)

target_link_libraries(InputTests PRIVATE OGDE::Input)

add_test(NAME InputTests COMMAND InputTests)
