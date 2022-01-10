
#########################
# ANTLR4
#########################
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


#########################
# BOOST GRAPH
#########################
include( ExternalProject )

set( boost_URL "https://sourceforge.net/projects/boost/files/boost/1.78.0/boost_1_78_0.tar.bz2" )
set( boost_SHA1 "7ccc47e82926be693810a687015ddc490b49296d" )
set( boost_INSTALL ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/boost )
set( BOOST_INCLUDE_DIR ${boost_INSTALL}/include )
set( BOOST_LIB_DIR ${boost_INSTALL}/lib )

ExternalProject_Add(external_boost
                    URL               ${boost_URL}
                    URL_HASH          SHA1=${boost_SHA1}
                    BUILD_IN_SOURCE 1
                    CONFIGURE_COMMAND ./bootstrap.sh
                                      --with-libraries=graph
                                      --prefix=<INSTALL_DIR>
                    BUILD_COMMAND     ./b2 install link=static variant=release threading=multi runtime-link=static
                    INSTALL_COMMAND ""
                    INSTALL_DIR ${boost_INSTALL} )
        

#########################
# IEMLParser
#########################

include_directories(${ANTLR4_INCLUDE_DIRS} 
                    ${ANTLR_IEMLParser_OUTPUT_DIR}
                    ${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/thirdparty/nlohmann_json
                    ${PROJECT_SOURCE_DIR}/thirdparty/better_enums)

set(IEMLParser_src ${ANTLR_IEMLParser_CXX_OUTPUTS}
                   ${CMAKE_CURRENT_LIST_DIR}/SyntaxError.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/IEMLGrammarVisitor.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/IemlParser.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ParserJsonSerializer.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/Program.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/Path.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/Namespace.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/HashElement.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/relation/Composition.cpp)
                   
set(IEMLParser_include ${PROJECT_SOURCE_DIR}/include
                       ${ANTLR4_INCLUDE_DIRS}
                       ${ANTLR_IEMLParser_OUTPUT_DIR}
                       ${PROJECT_SOURCE_DIR}/thirdparty/nlohmann_json
                       ${PROJECT_SOURCE_DIR}/thirdparty/better_enums
                       ${BOOST_INCLUDE_DIR})

# IEMLParser_object
add_library (IEMLParser_object OBJECT ${IEMLParser_src})
set_property(TARGET IEMLParser_object PROPERTY POSITION_INDEPENDENT_CODE True)
target_include_directories(IEMLParser_object PUBLIC ${IEMLParser_include})
add_dependencies(IEMLParser_object antlr4_static external_boost)

# IEMLParser_shared
add_library(IEMLParser_shared SHARED $<TARGET_OBJECTS:IEMLParser_object>)
target_link_libraries(IEMLParser_shared PUBLIC antlr4_static)
target_include_directories(IEMLParser_shared PUBLIC ${IEMLParser_include})

# IEMLParser_static
add_library(IEMLParser_static STATIC $<TARGET_OBJECTS:IEMLParser_object>)
target_link_libraries(IEMLParser_static PUBLIC antlr4_static)
target_include_directories(IEMLParser_static PUBLIC ${IEMLParser_include})
