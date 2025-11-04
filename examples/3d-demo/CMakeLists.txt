# 3D Demo example

add_executable(3DDemoExample main.cpp)
target_link_libraries(3DDemoExample PRIVATE OGDE::Engine)
install(TARGETS 3DDemoExample DESTINATION bin/examples)

# Triangle demo example (DirectX 11)
add_executable(TriangleDemo triangle_demo.cpp)
target_link_libraries(TriangleDemo PRIVATE OGDE::Engine)
install(TARGETS TriangleDemo DESTINATION bin/examples)

# Camera demo example (DirectX 11 with 3D camera)
add_executable(CameraDemo camera_demo.cpp)
target_link_libraries(CameraDemo PRIVATE OGDE::Engine)
install(TARGETS CameraDemo DESTINATION bin/examples)
