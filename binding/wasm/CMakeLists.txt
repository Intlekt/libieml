message(${CMAKE_TOOLCHAIN_FILE})
add_executable(parser_wasm main.cpp)
target_link_libraries(parser_wasm PUBLIC IEMLParser_static)


# -lembind : Embind is used to bind C++ functions and classes to JavaScript, so that the compiled code can be used in a natural way by “normal” JavaScript
# -s NO_DISABLE_EXCEPTION_CATCHING : https://emscripten.org/docs/porting/exceptions.html (to change execption catching)
# 
# -s MODULARIZE=1 : That puts all of the generated JavaScript into a factory function, which you can call to create an instance of your module. The factory function returns a Promise that resolves with the module instance. The promise is resolved once it’s safe to call the compiled code, i.e. after the compiled code has been downloaded and instantiated.
# -s EXPORT_ES6=1 : ES6 module, enable js import syntax
# --no-entry      : no main() function in main.cpp

set_target_properties(parser_wasm PROPERTIES LINK_FLAGS 
                        "-lembind \
                        -s NO_DISABLE_EXCEPTION_CATCHING \
                        -s STRICT=1 \
                        -s MODULARIZE=1 \
                        -s EXPORT_ES6=1 \
                        --no-entry \
                        -O3")
