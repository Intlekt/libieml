parser grammar ScriptParserGrammar;

options { tokenVocab=ScriptLexerGrammar; }

// we parse the script reversed because ANTLR4 only support direct left recursion 
// https://stackoverflow.com/questions/41017948/antlr4-the-following-sets-of-rules-are-mutually-left-recursive
// that means that we cannot write :
// 
// additive_script : multiplicative_script
//                 | multiplicative_script ADDITION_SIGN additive_script
//
// multiplicative_script: additive_script     LAYER_MARK;

script : script_=additive_script EOF;

additive_script : script_=multiplicative_script                                                      # additive_script__multiplication
                | children+=multiplicative_script (ADDITION_SIGN children+=multiplicative_script)+   # additive_script__addition
                | mark=LAYER_MARK_0 remarkable_addition=REMARKABLE_ADDITION                          # additive_script__remarkable
                ;

multiplicative_script : mark=LAYER_MARK_NON0 (children+=additive_script) (children+=additive_script)? (children+=additive_script)? # multiplicative_script__multiplication
                      | mark=LAYER_MARK_0 primitive=PRIMITIVES_LAYER_0                                                             # multiplicative_script__primitives
                      | mark=LAYER_MARK_1 remarkable_multiplication=REMARKABLE_MULTIPLICATION                                      # multiplicative_script__remarkable
                      ;
