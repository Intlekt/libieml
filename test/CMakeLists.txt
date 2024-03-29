set(TEST_IEML_GRAMMAR_FILES_FOLDER ${CMAKE_SOURCE_DIR}/assets/ieml_files)
set(TEST_IEML_ALL_SCRIPTS_FILE ${CMAKE_SOURCE_DIR}/assets/dictionary_scripts.txt)
set(TEST_IEML_GRAMMAR_EXAMPLES_FOLDER ${CMAKE_SOURCE_DIR}/assets/examples)
set(TEST_IEML_ERRONEOUS_FILES_FOLDER ${CMAKE_SOURCE_DIR}/assets/erroneous_files)

configure_file(TestConfig.h.in TestConfig.h)

if (DEFINED TEST_LIGHT)
    set(TEST_SRC grammar/test_declaration.cpp)
else()
    set(TEST_SRC utils_testing.cpp
                grammar/test_parser.cpp
                grammar/test_declaration.cpp
                grammar/test_json_serialization.cpp
                grammar/test_syntax_error.cpp
                grammar/test_paradigm.cpp
                grammar/test_phrase_word.cpp
                structure/test_path.cpp
                structure/test_path_node.cpp
                structure/test_namespace.cpp
                structure/test_word.cpp
                structure/test_script.cpp
                structure/test_table.cpp
                structure/test_link.cpp
                relation/test_composition.cpp
                relation/test_inclusion.cpp
                relation/test_graph.cpp)
    set(TEST_ADDITIONAL_LIB stdc++fs)
endif()

add_executable(libieml_test ${TEST_SRC})

set_target_properties(libieml_test PROPERTIES LINK_FLAGS ${LINK_FLAGS})

add_dependencies(libieml_test gtest gtest_main IEMLParser_shared)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(libieml_test gtest gtest_main IEMLParser_shared ${TEST_ADDITIONAL_LIB})

enable_testing()

add_test(NAME libieml_test 
         COMMAND libieml_test)
