lexer grammar ScriptLexerGrammar;


PRIMITIVES_LAYER_0 : [EAUSBT];
REMARKABLE_ADDITION : [OMFI];
REMARKABLE_MULTIPLICATION : ('ow'|'aw'|'y'|'o'|'e'|'uw'|'ew'|'u'|'a'|'i'|'j'|'g'|'s'|'b'|'t'|'h'|'c'|'k'|'m'|'n'|'p'|'x'|'d'|'f'|'l');
// REMARKABLE_MULTIPLICATION : ('wo'|'wa'|'y'|'o'|'e'|'wu'|'we'|'u'|'a'|'i'|'j'|'g'|'s'|'b'|'t'|'h'|'c'|'k'|'m'|'n'|'p'|'x'|'d'|'f'|'l');


ADDITION_SIGN : '+';

// LAYER0_MARK : ':' ;
// LAYER1_MARK : '.' ;
// LAYER2_MARK : '-' ;
// LAYER3_MARK : '\'';
// LAYER4_MARK : ',' ;
// LAYER5_MARK : '_' ;
// LAYER6_MARK : ';' ;
LAYER_MARK_0    : ':';
LAYER_MARK_1    : '.';
LAYER_MARK_NON0 : ([.\-',_;]|LAYER_MARK_1);
