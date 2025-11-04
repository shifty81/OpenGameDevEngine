# Audio module tests

add_executable(AudioTests test_audio.cpp)

target_link_libraries(AudioTests PRIVATE OGDE::Audio)

add_test(NAME AudioTests COMMAND AudioTests)
