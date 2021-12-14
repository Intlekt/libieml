grammar ieml;

program : declarations+=declaration* EOF;

declaration : COMPONENT  language_strings+=language_string+ phrase_=phrase                        DECLARATION_END    # componentDeclaration
            | NODE       language_strings+=language_string+ phrase_=phrase                        DECLARATION_END    # nodeDeclaration
            | WORD                                                                    word=STRING DECLARATION_END    # wordDeclaration
            | INFLECTION language_strings+=language_string+ inflexing_type=IDENTIFIER word=STRING DECLARATION_END    # inflexingDeclaration
            | AUXILIARY  language_strings+=language_string+ role_type=INTEGER         word=STRING DECLARATION_END    # auxiliaryDeclaration
            | JUNCTION   language_strings+=language_string+                           word=STRING DECLARATION_END    # junctionDeclaration
            | LANGUAGE   language=identifier                                                      DECLARATION_END    # languageDeclaration
            ;

phrase : PARENTHESIS_START phrase_lines+=phrase_line (COMMA phrase_lines+=phrase_line)* PARENTHESIS_END                                            # phrase__lines
       | PARENTHESIS_START JUNCTION_MARK junction_type=identifier JUNCTION_OPEN phrases+=phrase phrases+=phrase+ JUNCTION_CLOSE PARENTHESIS_END    # phrase__junction
       ;

phrase_line : role_type=INTEGER accentuation=SEMANTIC_ACCENT? sub_phrase=sub_phrase_line_auxiliary                           # phrase_line__sub_phrase_line_auxiliary
            | role_type=INTEGER accentuation=SEMANTIC_ACCENT? JUNCTION_MARK junction_type=identifier 
              JUNCTION_OPEN sub_phrases+=sub_phrase_line_auxiliary sub_phrases+=sub_phrase_line_auxiliary+ JUNCTION_CLOSE    # phrase_line__jonction_auxiliary
            ;

category : identifier_=identifier # category__identifier
         | phrase_=phrase         # category__phrase
         | word=STRING            # category__word
         ;


sub_phrase_line_auxiliary : (AUXILIARY_MARK auxiliary=identifier)? inflexed_category_=inflexed_category                                     # sub_phrase_line_auxiliary__sub_phrase_no_auxiliary
                          | AUXILIARY_MARK auxiliary=identifier JUNCTION_MARK junction_type=identifier 
                            JUNCTION_OPEN inflexed_categories+=inflexed_category inflexed_categories+=inflexed_category+ JUNCTION_CLOSE     # sub_phrase_line_auxiliary__jonction_no_auxiliary
                          ;

inflexed_category : (FLEXION_MARK inflexions+=identifier)* CATEGORY_MARK category_=category references+=reference*;

reference : REFERENCE_OPEN (id=INTEGER)? data_type=IDENTIFIER value=reference_value REFERENCE_CLOSE;

reference_value: identifier_=identifier  # reference_value__identifier
               | value=STRING            # reference_value__STRING
               | phrase_=phrase          # reference_value__phrase
               ;

language_string : language=identifier '\'' value=identifier '\'';


STRING : '"'(~'"'|'\\"')*'"';

identifier : identifiers+=IDENTIFIER+ ;

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

PARENTHESIS_START : '(';
PARENTHESIS_END : ')';
COMMA : ',';

SUBSTITUTION_SEP : ';' ;

INTEGER : [0-9]+ ;


COMMENT :  '//' ~( '\r' | '\n' )* -> skip;

WS: [ \t\n] -> skip ;

LANGUAGE : '@language';
COMPONENT : '@component';
NODE : '@node';
WORD : '@word';
INFLECTION : '@inflection';
AUXILIARY : '@auxiliary';
JUNCTION : '@junction';
// REFERENCE_ID : 'id';
// REFERENCE_DATATYPE : 'dt';
// REFERENCE_VALUE : 'va';
