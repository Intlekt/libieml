grammar ieml;

program : declarations+=declaration* EOF;

declaration : COMPONENT  language_strings+=language_string+ phrase_=phrase                         DECLARATION_END    # componentDeclaration
            | NODE       language_strings+=language_string+ phrase_=phrase                         DECLARATION_END    # nodeDeclaration
            | PARANODE   language_strings+=language_string+ phrase_=phrase                         DECLARATION_END    # paranodeDeclaration
            | WORD                                                                     word=STRING DECLARATION_END    # wordDeclaration
            | INFLECTION language_strings+=language_string+ inflection_type=IDENTIFIER word=STRING DECLARATION_END    # inflectionDeclaration
            | AUXILIARY  language_strings+=language_string+ role_type=INTEGER          word=STRING DECLARATION_END    # auxiliaryDeclaration
            | JUNCTION   language_strings+=language_string+                            word=STRING DECLARATION_END    # junctionDeclaration
            | LANGUAGE   language=identifier                                                       DECLARATION_END    # languageDeclaration
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
category_paradigm       : PARADIGM_START categories+=category            (PARADIGM_SEP categories+=category)*            PARADIGM_END;

inflection_list: (FLEXION_MARK inflections+=identifier)+ ;
inflection_list_paradigm: PARADIGM_START inflection_lists+=inflection_list (PARADIGM_SEP inflection_lists+=inflection_list)* PARADIGM_END;

auxiliary: AUXILIARY_MARK identifier_=identifier                                                                                          # auxiliary__simple
         | PARADIGM_START AUXILIARY_MARK identifiers+=identifier  (PARADIGM_SEP AUXILIARY_MARK identifiers+=identifier)* PARADIGM_END     # auxiliary__paradigm
         ;


sub_phrase_line_auxiliary : auxiliary_=auxiliary?          inflected_category_=inflected_category                                                  # sub_phrase_line_auxiliary__no_junction
                          | auxiliary_=auxiliary JUNCTION_MARK junction_type=identifier 
                            JUNCTION_OPEN inflected_categories+=inflected_category inflected_categories+=inflected_category+ JUNCTION_CLOSE      # sub_phrase_line_auxiliary__jonction
                          ;

inflected_category : inflection_list_=inflection_list?                CATEGORY_MARK category_=category references+=reference*        #inflected_category__singular 
                   | inflection_list_=inflection_list?                CATEGORY_MARK category_=category_paradigm                      #inflected_category__category_paradigm
                   | inflection_list_=inflection_list_paradigm        CATEGORY_MARK category_=category                               #inflected_category__inflection_paradigm
                   | inflection_list_=inflection_list_paradigm        CATEGORY_MARK category_=category_paradigm                      #inflected_category__inflection_and_category_paradigm
                   ;

reference : REFERENCE_OPEN (id=INTEGER)? data_type=IDENTIFIER value=reference_value REFERENCE_CLOSE;

reference_value: identifier_=identifier  # reference_value__identifier
               | value=STRING            # reference_value__STRING
               | phrase_=phrase          # reference_value__phrase
               ;

language_string : language=LANGUAGE_STRING_MARK value=identifier;

LANGUAGE_STRING_MARK : [a-z][a-z]':';

STRING : '"'(~'"'|'\\"')*'"';

identifier : identifiers+=IDENTIFIER+ ;

IDENTIFIER : [a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ\-][0-9a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ'\-]*;

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

PARADIGM_START : '{' ;
PARADIGM_END : '}' ;
PARADIGM_SEP : ';' ;

PARENTHESIS_START : '(';
PARENTHESIS_END : ')';
COMMA : ',';


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
PARANODE: '@paranode';
TABLE: '@table';
// REFERENCE_ID : 'id';
// REFERENCE_DATATYPE : 'dt';
// REFERENCE_VALUE : 'va';
