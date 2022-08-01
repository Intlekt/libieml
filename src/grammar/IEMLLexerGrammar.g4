lexer grammar IEMLLexerGrammar;


LANGUAGE_STRING_MARK : [a-z][a-z]':';
CLASS_MARK : 'class:';
ROLE_MARK : 'role:';
TYPE_MARK : 'type:';

ARGUMENTS_MARK : 'args:';
PHRASE_WORD_INFLECTION_MARK: 'phraseWordInflection:';
LANGUAGE_STRING_TEMPLATE_MARK : 'template-'[a-z][a-z]':';
LINK_MARK : 'link:';
DOMAIN_MARK : 'domain:';
CONDITION_MARK: 'condition:';


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

EQUAL                : '==';
NOTEQUAL             : '!=';


SEMANTIC_ACCENT      : '!' ;
   
DECLARATION_MARK     : '@' ;
DOT                  : '.' ;
    
PARADIGM_START       : '{' ;
PARADIGM_END         : '}' ;
PARADIGM_SEP         : ';' ;
   
PARENTHESIS_START    : '(';
PARENTHESIS_END      : ')';
COMMA                : ',';

MAPPING              : '->';

VARIABLE             : '$'[0-9a-zA-Z]+;

INTEGER              : [0-9]+ ;
   
COMMENT              : '//' ~( '\r' | '\n' )* -> skip;
WS                   : [ \t\n] -> skip ;
   
FUNCTION             : '@function';
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
ROOT_PARADIGM        : '@rootparadigm';
   