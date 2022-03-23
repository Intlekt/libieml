
#########################
# ANTLR4
#########################

set (ANTLR4_TAG fb661e891d2db191af4369695651906865c4959f)

# add external build for antlrcpp
include( ExternalAntlr4Cpp )

message(STATUS "Found antlr4cpp includes: ${ANTLR4_INCLUDE_DIRS} with flags ${COMPILER_FLAGS}")
set(ANTLR_EXECUTABLE ${CMAKE_SOURCE_DIR}/thirdparty/antlr/antlr-4.9.2-complete.jar)

find_package(ANTLR REQUIRED)

# Generate IEML Parser
antlr_target(IEMLLexerGrammar  ${CMAKE_CURRENT_LIST_DIR}/grammar/IEMLLexerGrammar.g4  LEXER
             PACKAGE ieml_generated)

antlr_target(IEMLParserGrammar ${CMAKE_CURRENT_LIST_DIR}/grammar/IEMLParserGrammar.g4 PARSER
             PACKAGE ieml_generated
             DEPENDS_ANTLR IEMLLexerGrammar
             COMPILE_FLAGS -lib ${ANTLR_IEMLLexerGrammar_OUTPUT_DIR} 
             VISITOR)

message(STATUS "Generated IEML parser at ${ANTLR_IEMLParserGrammar_OUTPUT_DIR} ")

# Generate Script Parser
antlr_target(ScriptLexerGrammar  ${CMAKE_CURRENT_LIST_DIR}/grammar/ScriptLexerGrammar.g4  LEXER
             PACKAGE script_generated)

antlr_target(ScriptParserGrammar ${CMAKE_CURRENT_LIST_DIR}/grammar/ScriptParserGrammar.g4 PARSER
             PACKAGE script_generated
             DEPENDS_ANTLR ScriptLexerGrammar
             COMPILE_FLAGS -lib ${ANTLR_ScriptLexerGrammar_OUTPUT_DIR}
             VISITOR)

message(STATUS "Generated Script parser at ${ANTLR_ScriptParserGrammar_OUTPUT_DIR} ")


#########################
# BOOST GRAPH
#########################
include( ExternalProject )

set( boost_URL "https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.bz2" )
set( boost_SHA256 "8681f175d4bdb26c52222665793eef08490d7758529330f98d3b29dd0735bccc" )
set( boost_INSTALL ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/boost )
set( BOOST_INCLUDE_DIR ${boost_INSTALL}/include )
set( BOOST_LIB_DIR ${boost_INSTALL}/lib )

set( boost_SOURCE_DIR ${boost_INSTALL}/src/external_boost )
set( boost_DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/build_cache/boost )

ExternalProject_Add(external_boost
                    PREFIX            ${boost_INSTALL}
                    # SOURCE_DIR        ${boost_SOURCE_DIR}
                    DOWNLOAD_DIR ${boost_DOWNLOAD_DIR}
                    
                    URL               ${boost_URL}
                    URL_HASH          SHA256=${boost_SHA256}
                    CONFIGURE_COMMAND ""
                    BUILD_COMMAND     ""
                    INSTALL_COMMAND   mkdir ${BOOST_INCLUDE_DIR} && cp -R ${boost_INSTALL}/src/external_boost/boost ${BOOST_INCLUDE_DIR}/boost
                    )

#########################
# IEMLParser
#########################

set(IEMLParser_src ${ANTLR_IEMLLexerGrammar_CXX_OUTPUTS}
                   ${ANTLR_IEMLParserGrammar_CXX_OUTPUTS}
                   ${ANTLR_ScriptLexerGrammar_CXX_OUTPUTS}
                   ${ANTLR_ScriptParserGrammar_CXX_OUTPUTS}
                   ${CMAKE_CURRENT_LIST_DIR}/SyntaxError.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/IemlParser.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/AbstractVisitor.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/IEMLGrammarVisitor.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ScriptParser.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ScriptGrammarVisitor.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ParserJsonSerializer.cpp


                   ${CMAKE_CURRENT_LIST_DIR}/ast/Program.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/DimensionDefinition.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/PartialPathTree.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/InvariantParanodeMapping.cpp

                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/CategoryDeclaration.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/AuxiliaryDeclaration.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/JunctionDeclaration.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/InflectionDeclaration.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/ParanodeDeclaration.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/LanguageDeclaration.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/SingularCategoryDeclaration.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/LinkDeclaration.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/WordDeclaration.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/TableDeclaration.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/RootParadigmDeclaration.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/ast/declaration/FunctionDeclaration.cpp


                   ${CMAKE_CURRENT_LIST_DIR}/structure/reference/ReferenceSchema.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/path/PathNode.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/path/PathTree.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/path/PathParser.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/Namespace.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/ParadigmRegister.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/script/Script.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/script/PrimitiveScript.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/script/AdditiveScript.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/script/MultiplicativeScript.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/script/NullScript.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/structure/script/ScriptRegister.cpp
                   
                   ${CMAKE_CURRENT_LIST_DIR}/structure/link/LinkRegister.cpp

                   ${CMAKE_CURRENT_LIST_DIR}/relation/RelationGraph.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/relation/Composition.cpp
                   ${CMAKE_CURRENT_LIST_DIR}/relation/Inclusion.cpp)
                   
set(IEMLParser_include ${PROJECT_SOURCE_DIR}/include
                       ${ANTLR4_INCLUDE_DIRS}
                       ${ANTLR_IEMLLexerGrammar_OUTPUT_DIR}
                       ${ANTLR_IEMLParserGrammar_OUTPUT_DIR}
                       ${ANTLR_ScriptLexerGrammar_OUTPUT_DIR}
                       ${ANTLR_ScriptParserGrammar_OUTPUT_DIR}
                       ${PROJECT_SOURCE_DIR}/thirdparty/nlohmann_json
                       ${PROJECT_SOURCE_DIR}/thirdparty/better_enums
                       ${BOOST_INCLUDE_DIR})

include_directories(${IEMLParser_include})

# IEMLParser_object
add_library (IEMLParser_object OBJECT ${IEMLParser_src})
set_target_properties(IEMLParser_object PROPERTIES LINK_FLAGS ${LINK_FLAGS})
set_property(TARGET IEMLParser_object PROPERTY POSITION_INDEPENDENT_CODE True)
target_include_directories(IEMLParser_object PUBLIC ${IEMLParser_include})
add_dependencies(IEMLParser_object antlr4_static external_boost)

target_compile_options(IEMLParser_object PRIVATE -Wall -Wextra -Wpedantic)

# IEMLParser_shared
add_library(IEMLParser_shared SHARED $<TARGET_OBJECTS:IEMLParser_object>)
target_link_libraries(IEMLParser_shared PUBLIC antlr4_static)
target_include_directories(IEMLParser_shared PUBLIC ${IEMLParser_include})

# IEMLParser_static
add_library(IEMLParser_static STATIC $<TARGET_OBJECTS:IEMLParser_object>)
target_link_libraries(IEMLParser_static PUBLIC antlr4_static)
target_include_directories(IEMLParser_static PUBLIC ${IEMLParser_include})
