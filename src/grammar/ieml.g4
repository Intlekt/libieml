grammar ieml;

declarations : declaration* EOF;

declaration : DECLARATION_MARK 'component' language_string+ phrase DECLARATION_END # component;

phrase : '(' phrase_line (',' phrase_line)* ')'
       | '(' jonction_phrase ')';

jonction_phrase : JONCTION_MARK identifier JONCTION_OPEN phrase phrase+ JONCTION_CLOSE;

phrase_line : INTEGER accentuation=SEMANTIC_ACCENT? role_auxiliary
            | INTEGER accentuation=SEMANTIC_ACCENT? jonction_auxiliary;

role_no_auxiliary : (FLEXION_MARK inflection+=identifier)*
                    CATEGORY_MARK category=identifier
                    reference?;

role_auxiliary : (AUXILIARY_MARK auxiliary=identifier)? role_no_auxiliary
                 | (AUXILIARY_MARK auxiliary=identifier)? jonction_no_auxiliary;

jonction_no_auxiliary : JONCTION_MARK identifier JONCTION_OPEN role_no_auxiliary role_no_auxiliary+ JONCTION_CLOSE ;

jonction_auxiliary : JONCTION_MARK identifier JONCTION_OPEN role_auxiliary role_auxiliary+ JONCTION_CLOSE ;
// ambiguité : jonction auxialiary avec role_auxiliary -> jonction_no_aux pour les cas ou pas du tout d'auxiliaires

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
JONCTION_MARK : '&' ;
AUXILIARY_MARK : '*' ;
CATEGORY_MARK : '#' ;

JONCTION_OPEN : '[' ;
JONCTION_CLOSE : ']' ;

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