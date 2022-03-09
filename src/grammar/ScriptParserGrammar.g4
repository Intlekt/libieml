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

script : script_=layer0 EOF # script__layer0
       | script_=layer1 EOF # script__layer1
       | script_=layer2 EOF # script__layer2
       | script_=layer3 EOF # script__layer3
       | script_=layer4 EOF # script__layer4
       | script_=layer5 EOF # script__layer5
       | script_=layer6 EOF # script__layer6
       ;

mult_layer0 : L_LAYER_MARK_0 primitive=L_PRIMITIVES_LAYER_0             # mult_layer0__primitive
            | L_LAYER_MARK_0 remarkable_addition=L_REMARKABLE_ADDITION  # mult_layer0__remarkable_addition
            ;

layer0 : children+=mult_layer0 (L_ADDITION_SIGN children+=mult_layer0)+ # layer0__addition
       | script_=mult_layer0                                            # layer0__script
       ;


mult_layer1 : L_LAYER_MARK_1 (children+=layer0) (children+=layer0)? (children+=layer0)? # mult_layer1__mult
            | L_LAYER_MARK_1 remarkable_multiplication=L_REMARKABLE_MULTIPLICATION      # mult_layer1__remarkable_mult
            ;
layer1 : children+=mult_layer1 (L_ADDITION_SIGN children+=mult_layer1)+ # layer1__addition
       | script_=mult_layer1                                            # layer1__script
       ;

mult_layer2 : L_LAYER_MARK_2 (children+=layer1) (children+=layer1)? (children+=layer1)? # mult_layer2__mult
            ;
layer2 : children+=mult_layer2 (L_ADDITION_SIGN children+=mult_layer2)+ # layer2__addition
       | script_=mult_layer2                                            # layer2__script
       ;

mult_layer3 : L_LAYER_MARK_3 (children+=layer2) (children+=layer2)? (children+=layer2)? # mult_layer3__mult
            ;
layer3 : children+=mult_layer3 (L_ADDITION_SIGN children+=mult_layer3)+ # layer3__addition
       | script_=mult_layer3                                            # layer3__script
       ;

mult_layer4 : L_LAYER_MARK_4 (children+=layer3) (children+=layer3)? (children+=layer3)? # mult_layer4__mult
            ;
layer4 : children+=mult_layer4 (L_ADDITION_SIGN children+=mult_layer4)+ # layer4__addition
       | script_=mult_layer4                                            # layer4__script
       ;

mult_layer5 : L_LAYER_MARK_5 (children+=layer4) (children+=layer4)? (children+=layer4)? # mult_layer5__mult
            ;
layer5 : children+=mult_layer5 (L_ADDITION_SIGN children+=mult_layer5)+ # layer5__addition
       | script_=mult_layer5                                            # layer5__script
       ;

mult_layer6 : L_LAYER_MARK_6 (children+=layer5) (children+=layer5)? (children+=layer5)? # mult_layer6__mult
            ;
layer6 : children+=mult_layer6 (L_ADDITION_SIGN children+=mult_layer6)+ # layer6__addition
       | script_=mult_layer6                                            # layer6__script
       ;
