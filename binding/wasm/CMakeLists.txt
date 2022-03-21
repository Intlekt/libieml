message(${CMAKE_TOOLCHAIN_FILE})
add_executable(parser_wasm main.cpp)
target_link_libraries(parser_wasm PUBLIC IEMLParser_static)



# -lembind : Embind is used to bind C++ functions and classes to JavaScript, so that the compiled code can be used in a natural way by “normal” JavaScript
# --no-entry      : no main() function in main.cpp
set_target_properties(parser_wasm PROPERTIES LINK_FLAGS "-lembind --no-entry ${LINK_FLAGS}")
