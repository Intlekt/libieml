grammar ieml;

program : declaration* EOF;

declaration : DECLARATION_MARK 'component' language_string+ phrase DECLARATION_END # component
            ;

phrase : '(' phrase_line (',' phrase_line)* ')'                                          # phrase__lines
       | '(' JUNCTION_MARK identifier JUNCTION_OPEN phrase phrase+ JUNCTION_CLOSE ')'    # phrase__junction
       ;

// ambiguité : jonction auxialiary avec sub_phrase_line_auxiliary -> jonction_no_aux pour les cas ou pas du tout d'auxiliaires
phrase_line : INTEGER accentuation=SEMANTIC_ACCENT? sub_phrase_line_auxiliary                      # phrase_line__sub_phrase_line_auxiliary
            | INTEGER accentuation=SEMANTIC_ACCENT? JUNCTION_MARK identifier 
              JUNCTION_OPEN sub_phrase_line_auxiliary sub_phrase_line_auxiliary+ JUNCTION_CLOSE    # phrase_line__jonction_auxiliary
            ;

category : identifier # category__identifier
         | phrase     # category__phrase
         | STRING     # category__word
         ;

inflexed_category : (FLEXION_MARK inflexions+=identifier)* CATEGORY_MARK category reference?;

sub_phrase_line_auxiliary : (AUXILIARY_MARK auxiliary=identifier)? inflexed_category              # sub_phrase_line_auxiliary__sub_phrase_no_auxiliary
                          | (AUXILIARY_MARK auxiliary=identifier)? JUNCTION_MARK junction_type=identifier 
                            JUNCTION_OPEN inflexed_category inflexed_category+ JUNCTION_CLOSE     # sub_phrase_line_auxiliary__jonction_no_auxiliary
                          ;

reference : REFERENCE_OPEN ('id' INTEGER)? 'dt' IDENTIFIER 'va' reference_value REFERENCE_CLOSE;

reference_value: identifier  # reference_value__identifier
               | STRING      # reference_value__STRING
               | phrase      # reference_value__phrase
               ;

language_string : language=identifier '"' value=identifier '"';

STRING : '\''(~'\''|'\\\'')*'\'';

identifier : IDENTIFIER+ ;

IDENTIFIER : [a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ][0-9a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ]*;

FLEXION_MARK : '~' ;
JUNCTION_MARK : '&' ;
AUXILIARY_MARK : '*' ;
CATEGORY_MARK : '#' ;

JUNCTION_OPEN : '[' ;
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