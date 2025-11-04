# 3D Demo example

add_executable(3DDemoExample main.cpp)

target_link_libraries(3DDemoExample PRIVATE OGDE::Engine)

install(TARGETS 3DDemoExample DESTINATION bin/examples)
