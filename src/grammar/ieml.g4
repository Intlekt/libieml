grammar ieml;

declarations : declaration* EOF;

declaration : DECLARATION_MARK 'component' language_string+ phrase DECLARATION_END # component
            ;

phrase : '(' phrase_line (',' phrase_line)* ')' # phrase_lines
       | '(' junction_phrase ')'                # phrase_junction
       ;

junction_phrase : JUNCTION_OPEN identifier JONCTION_OPEN phrase phrase+ JUNCTION_CLOSE;

phrase_line : INTEGER accentuation=SEMANTIC_ACCENT? sub_phrase_line_auxiliary       # phrase_line__sub_phrase_line_auxiliary
            | INTEGER accentuation=SEMANTIC_ACCENT? jonction_auxiliary              # phrase_line__jonction_auxiliary
            ;

flexed_category : (FLEXION_MARK inflection+=identifier)* CATEGORY_MARK category=identifier reference?;

sub_phrase_line_auxiliary : (AUXILIARY_MARK auxiliary=identifier)? flexed_category       # sub_phrase_line_auxiliary__sub_phrase_no_auxiliary
                          | (AUXILIARY_MARK auxiliary=identifier)? jonction_no_auxiliary        # sub_phrase_line_auxiliary__jonction_no_auxiliary
                          ;

jonction_no_auxiliary : JUNCTION_OPEN identifier JONCTION_OPEN flexed_category flexed_category+ JUNCTION_CLOSE 
                      ;

jonction_auxiliary : JUNCTION_OPEN identifier JONCTION_OPEN sub_phrase_line_auxiliary sub_phrase_line_auxiliary+ JUNCTION_CLOSE ;
// ambiguité : jonction auxialiary avec sub_phrase_line_auxiliary -> jonction_no_aux pour les cas ou pas du tout d'auxiliaires

reference : REFERENCE_OPEN
            ('id' INTEGER)?
            'dt' IDENTIFIER
            'va' reference_value
            REFERENCE_CLOSE;

reference_value: identifier
               | STRING
               | phrase;

language_string : language=identifier '"' value=identifier '"';

STRING : '\''(~'\''|'\\\'')*'\'';

identifier : IDENTIFIER+ ;

IDENTIFIER : [a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ][0-9a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ]*;

//SCRIPT : '"'[EUASBTOMFIacbedgfihkjmlonpsutwyx][EUASBTOMFIacbedgfihkjmlonpsutwyx.\-;:,'’_+]+'"';

FLEXION_MARK : '~' ;
JUNCTION_OPEN : '&' ;
AUXILIARY_MARK : '*' ;
CATEGORY_MARK : '#' ;

JONCTION_OPEN : '[' ;
JUNCTION_CLOSE : ']' ;

REFERENCE_OPEN : '<' ;
REFERENCE_CLOSE : '>' ;

SEMANTIC_ACCENT : '!' ;

DECLARATION_MARK : '@' ;
DECLARATION_END : '.' ;

SUBSTITUTION_START : '{' ;
SUBSTITUTION_END : '}' ;
SUBSTITUTION_SEP : ';' ;

INTEGER : [0-9]+ ;


COMMENT :  '//' ~( '\r' | '\n' )* -> skip;

WS: [ \t\n] -> skip ;