grammar ieml;

program : declarations+=declaration* EOF;

declaration : COMPONENT  language_strings+=language_string+ phrase_=phrase                               DECLARATION_END    # componentDeclaration
            | NODE       language_strings+=language_string+ phrase_=phrase                               DECLARATION_END    # nodeDeclaration
            | PARANODE   language_strings+=language_string+ phrase_=phrase                               DECLARATION_END    # paranodeDeclaration
            | WORD                                                                     word_=word        DECLARATION_END    # wordDeclaration
            | INFLECTION language_strings+=language_string+ inflection_type=IDENTIFIER word_=word        DECLARATION_END    # inflectionDeclaration
            | AUXILIARY  language_strings+=language_string+ role_type=INTEGER          word_=word        DECLARATION_END    # auxiliaryDeclaration
            | JUNCTION   language_strings+=language_string+                            word_=word        DECLARATION_END    # junctionDeclaration
            | LANGUAGE   language=identifier                                                             DECLARATION_END    # languageDeclaration
            ;

phrase : PARENTHESIS_START phrase_lines+=phrase_line (COMMA phrase_lines+=phrase_line)* PARENTHESIS_END                        # phrase__lines
       | PARENTHESIS_START junction_=junction JUNCTION_OPEN phrases+=phrase phrases+=phrase+ JUNCTION_CLOSE PARENTHESIS_END    # phrase__junction
       ;

phrase_line : role_type=INTEGER accentuation=SEMANTIC_ACCENT? sub_phrase=sub_phrase_line_auxiliary                           # phrase_line__sub_phrase_line_auxiliary
            | role_type=INTEGER accentuation=SEMANTIC_ACCENT? junction_=junction
              JUNCTION_OPEN sub_phrases+=sub_phrase_line_auxiliary sub_phrases+=sub_phrase_line_auxiliary+ JUNCTION_CLOSE    # phrase_line__jonction_auxiliary
            ;

junction : JUNCTION_MARK junction_type=identifier;

category : identifier_=identifier # category__identifier
         | phrase_=phrase         # category__phrase
         | word_=word             # category__word
         ;

category_paradigm       : PARADIGM_START categories+=category            (PARADIGM_SEP categories+=category)*            PARADIGM_END;

inflection_list: (FLEXION_MARK inflections+=identifier)+ ;
inflection_list_paradigm: PARADIGM_START inflection_lists+=inflection_list (PARADIGM_SEP inflection_lists+=inflection_list)* PARADIGM_END;

auxiliary: AUXILIARY_MARK identifier_=identifier      # auxiliary__identifier
         | AUXILIARY_MARK word_=word                  # auxiliary__word
         ;

auxiliary_paradigm: PARADIGM_START identifiers+=auxiliary (PARADIGM_SEP identifiers+=auxiliary)* PARADIGM_END;

auxiliary_simple_or_paradigm: auxiliary_=auxiliary            # auxiliary_simple_or_paradigm__simple
                            | auxiliary_=auxiliary_paradigm   # auxiliary_simple_or_paradigm__paradigm
                            ;

sub_phrase_line_auxiliary : auxiliary_=auxiliary_simple_or_paradigm?          inflected_category_=inflected_category                                                  # sub_phrase_line_auxiliary__no_junction
                          | auxiliary_=auxiliary_simple_or_paradigm junction_=junction 
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

word : word_=STRING;

path : (PATH_SEPARATOR path_nodes+=path_node)+;

path_node : CATEGORY_MARK                                                                   # path_node__root
          | PARADIGM_START INTEGER PARADIGM_END                                             # path_node__paradigm
          | auxiliary_=auxiliary                                                            # path_node__auxiliary
          | inflection_list_=inflection_list                                                # path_node__inflection
          | word_=word                                                                      # path_node__word
          | junction_=junction JUNCTION_OPEN INTEGER JUNCTION_CLOSE                         # path_node__junction
          | INTEGER                                                                         # path_node__role_number
          ;

LANGUAGE_STRING_MARK : [a-z][a-z]':';

STRING : '"'(~'"'|'\\"')*'"';

identifier : identifiers+=IDENTIFIER+ ;

IDENTIFIER : [a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ\-][0-9a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ'\-]*;


FLEXION_MARK : '~' ;
JUNCTION_MARK : '&' ;
AUXILIARY_MARK : '*' ;
CATEGORY_MARK : '#' ;

PATH_SEPARATOR : '/';

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
