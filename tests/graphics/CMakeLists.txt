# Graphics module tests

add_executable(GraphicsTests test_renderer.cpp)

target_link_libraries(GraphicsTests PRIVATE OGDE::Graphics)

add_test(NAME GraphicsTests COMMAND GraphicsTests)
