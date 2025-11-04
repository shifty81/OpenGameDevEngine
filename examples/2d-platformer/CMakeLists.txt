# 2D Platformer example

add_executable(2DPlatformerExample main.cpp)

target_link_libraries(2DPlatformerExample PRIVATE OGDE::Engine)

install(TARGETS 2DPlatformerExample DESTINATION bin/examples)
