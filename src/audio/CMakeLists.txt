# Audio module - sound and music system

add_library(OGDEAudio STATIC
    AudioEngine.cpp
)

target_include_directories(OGDEAudio
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(OGDEAudio PUBLIC OGDE::Core)

add_library(OGDE::Audio ALIAS OGDEAudio)

install(TARGETS OGDEAudio
        EXPORT OpenGameDevEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
