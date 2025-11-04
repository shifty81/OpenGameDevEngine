# Scripting module - scripting language support

add_library(OGDEScripting STATIC
    ScriptEngine.cpp
)

target_include_directories(OGDEScripting
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(OGDEScripting PUBLIC OGDE::Core)

add_library(OGDE::Scripting ALIAS OGDEScripting)

install(TARGETS OGDEScripting
        EXPORT OpenGameDevEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
