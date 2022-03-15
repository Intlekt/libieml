message(${CMAKE_TOOLCHAIN_FILE})
add_executable(parser_wasm main.cpp)
target_link_libraries(parser_wasm PUBLIC IEMLParser_static)
# target_link_options(parser_wasm PRIVATE -sEXPORTED_FUNCTIONS=['_parse_project'])
# target_link_options(parser_wasm PRIVATE -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap'])


set_target_properties(parser_wasm PROPERTIES LINK_FLAGS "-lembind -s NO_DISABLE_EXCEPTION_CATCHING -O3")
