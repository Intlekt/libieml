LIST( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

set (ANTLR4_TAG fb661e891d2db191af4369695651906865c4959f)
# add external build for antlrcpp
include( ExternalAntlr4Cpp )

# required if linking to static library
# add_definitions(-DANTLR4CPP_STATIC)


message(STATUS "Found antlr4cpp includes: ${ANTLR4_INCLUDE_DIRS} ")
set(ANTLR_EXECUTABLE ${CMAKE_SOURCE_DIR}/thirdparty/antlr/antlr-4.9.2-complete.jar)

find_package(ANTLR REQUIRED)

antlr_target(IEMLParser ${CMAKE_CURRENT_LIST_DIR}/grammar/ieml.g4 
             PACKAGE ieml_generated VISITOR)

message(STATUS "Generated parser at ${ANTLR_IEMLParser_OUTPUT_DIR} ")

include_directories(${ANTLR4_INCLUDE_DIRS} 
                    ${ANTLR_IEMLParser_OUTPUT_DIR}
                    ${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/thirdparty/nlohmann_json
                    ${PROJECT_SOURCE_DIR}/thirdparty/better_enums)


add_library (IEMLParser SHARED
             ${ANTLR_IEMLParser_CXX_OUTPUTS}
             ${CMAKE_CURRENT_LIST_DIR}/RecognitionError.cpp
             ${CMAKE_CURRENT_LIST_DIR}/IEMLGrammarVisitor.cpp
             ${CMAKE_CURRENT_LIST_DIR}/IemlParser.cpp
             ${CMAKE_CURRENT_LIST_DIR}/ast/Program.cpp
             ${CMAKE_CURRENT_LIST_DIR}/structure/Path.cpp
             ${CMAKE_CURRENT_LIST_DIR}/structure/Namespace.cpp
             ${CMAKE_CURRENT_LIST_DIR}/relation/Composition.cpp
             )


target_include_directories(IEMLParser 
                           INTERFACE ${PROJECT_SOURCE_DIR}/include
                                     ${ANTLR4_INCLUDE_DIRS}
                                     ${ANTLR_IEMLParser_OUTPUT_DIR}
                                     ${PROJECT_SOURCE_DIR}/thirdparty/nlohmann_json
                                     ${PROJECT_SOURCE_DIR}/thirdparty/better_enums)

# target_compile_options(IEMLParser PRIVATE -Wall -Wextra -Wpedantic)


target_link_libraries(IEMLParser antlr4_shared)




