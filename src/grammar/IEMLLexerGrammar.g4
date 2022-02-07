lexer grammar IEMLLexerGrammar;


LANGUAGE_STRING_MARK : [a-z][a-z]':';
DIMENSION_MARK       : [1-3]'d:';
   
STRING               : '"'(~'"'|'\\"')*'"';
   
IDENTIFIER           : [a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ\-][0-9a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ'\-]*;
   
FLEXION_MARK         : '~' ;
JUNCTION_MARK        : '&' ;
AUXILIARY_MARK       : '*' ;
CATEGORY_MARK        : '#' ;
     
PATH_SEPARATOR       : '/';
   
JUNCTION_OPEN        : '[' ;
JUNCTION_CLOSE       : ']' ;
     
REFERENCE_OPEN       : '<' ;
REFERENCE_CLOSE      : '>' ;
     
SEMANTIC_ACCENT      : '!' ;
   
DECLARATION_MARK     : '@' ;
DECLARATION_END      : '.' ;
    
PARADIGM_START       : '{' ;
PARADIGM_END         : '}' ;
PARADIGM_SEP         : ';' ;
   
PARENTHESIS_START    : '(';
PARENTHESIS_END      : ')';
COMMA                : ',';
   

VARIABLE             : '$'[0-9a-zA-Z]+;

INTEGER              : [0-9]+ ;
   
COMMENT              : '//' ~( '\r' | '\n' )* -> skip;
WS                   : [ \t\n] -> skip ;
   
LANGUAGE             : '@language';
COMPONENT            : '@component';
NODE                 : '@node';
WORD                 : '@word';
INFLECTION           : '@inflection';
AUXILIARY            : '@auxiliary';
JUNCTION             : '@junction';
PARANODE             : '@paranode';
LINK                 : '@link';
TABLE                : '@table';
   