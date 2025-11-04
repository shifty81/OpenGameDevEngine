# Asset Pipeline Tools

add_executable(AssetConverter asset_converter.cpp)

target_link_libraries(AssetConverter PRIVATE OGDE::Core)

install(TARGETS AssetConverter DESTINATION bin/tools)
