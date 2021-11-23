set(TEST_IEML_GRAMMAR_EXAMPLES_FOLDER ${CMAKE_SOURCE_DIR}/assets/ieml_files)

configure_file(TestConfig.h.in TestConfig.h)

add_executable(registerer_test grammar/test_parser.cpp
                               structure/test_path.cpp)

add_dependencies(registerer_test gtest gtest_main IEMLParser)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(registerer_test gtest gtest_main IEMLParser stdc++fs)

enable_testing()

add_test(NAME registerer_test 
         COMMAND registerer_test)
