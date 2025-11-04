# Basic example demonstrating engine initialization

add_executable(BasicExample main.cpp)

target_link_libraries(BasicExample PRIVATE OGDE::Engine)

install(TARGETS BasicExample DESTINATION bin/examples)
