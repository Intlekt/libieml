LIST( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add external build for antlrcpp
include( ExternalAntlr4Cpp )

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)


message(STATUS "Found antlr4cpp includes: ${ANTLR4_INCLUDE_DIRS} ")
set(ANTLR_EXECUTABLE ${CMAKE_SOURCE_DIR}/thirdparty/antlr/antlr-4.9.2-complete.jar)

find_package(ANTLR REQUIRED)

antlr_target(IEMLParser ${CMAKE_CURRENT_LIST_DIR}/grammar/ieml.g4 
             PACKAGE ieml_generated VISITOR)

message(STATUS "Generated parser at ${ANTLR_IEMLParser_OUTPUT_DIR} ")

include_directories(${ANTLR4_INCLUDE_DIRS} 
                    ${ANTLR_IEMLParser_OUTPUT_DIR}
                    ${CMAKE_CURRENT_LIST_DIR})


add_library (IEMLParser 
             ${ANTLR_IEMLParser_CXX_OUTPUTS}
             ${CMAKE_CURRENT_LIST_DIR}/RecognitionError.cpp
             ${CMAKE_CURRENT_LIST_DIR}/IemlParser.cpp
             )


target_include_directories(IEMLParser 
                           INTERFACE ${ANTLR4_INCLUDE_DIRS} 
                                     ${ANTLR_IEMLParser_OUTPUT_DIR} 
                                     ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(IEMLParser antlr4_static)




