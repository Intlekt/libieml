parser grammar IEMLParserGrammar;

options { tokenVocab=IEMLLexerGrammar; }


program : declarations+=declaration* EOF;
   
declaration : COMPONENT     language_strings+=language_string+                                       phrase_=phrase            DECLARATION_END    # componentDeclaration
            | NODE          language_strings+=language_string+                                       phrase_=phrase            DECLARATION_END    # nodeDeclaration
            | PARANODE      language_strings+=language_string+ dimensions+=dimension_definition+     phrase_=phrase            DECLARATION_END    # paranodeDeclaration
            | LINK          language_strings+=language_string+                                       phrase_=phrase            DECLARATION_END    # linkDeclaration
            | ROOT_PARADIGM                                    TYPE_MARK type_root=IDENTIFIER        word_=word                DECLARATION_END    # rootParadigmDeclaration
            | WORD                                                                                   word_=word                DECLARATION_END    # wordDeclaration
            | INFLECTION    language_strings+=language_string+ CLASS_MARK inflection_type=IDENTIFIER word_=word                DECLARATION_END    # inflectionDeclaration
            | AUXILIARY     language_strings+=language_string+ ROLE_MARK role_type_=role_type        word_=word                DECLARATION_END    # auxiliaryDeclaration
            | JUNCTION      language_strings+=language_string+                                       word_=word                DECLARATION_END    # junctionDeclaration
            | LANGUAGE      language=identifier                                                                                DECLARATION_END    # languageDeclaration
            | TABLE                                                    (category_mappings+=invariant_paranode_mapping)+        DECLARATION_END    # tableDeclaration
            ;

phrase : PARENTHESIS_START phrase_lines+=phrase_line (COMMA phrase_lines+=phrase_line)* PARENTHESIS_END                        # phrase__lines
       | PARENTHESIS_START junction_=junction JUNCTION_OPEN phrases+=phrase phrases+=phrase+ JUNCTION_CLOSE PARENTHESIS_END    # phrase__junction
       ;

phrase_line : role_type_=role_type accentuation=SEMANTIC_ACCENT? sub_phrase=sub_phrase_line_auxiliary                           # phrase_line__sub_phrase_line_auxiliary
            | role_type_=role_type accentuation=SEMANTIC_ACCENT? junction_=junction
              JUNCTION_OPEN sub_phrases+=sub_phrase_line_auxiliary sub_phrases+=sub_phrase_line_auxiliary+ JUNCTION_CLOSE    # phrase_line__jonction_auxiliary
            ;

junction : JUNCTION_MARK identifier_=identifier       # junction__identifier
         | JUNCTION_MARK word_=word                   # junction__word
         ;

category : CATEGORY_MARK identifier_=identifier # category__identifier
         | CATEGORY_MARK phrase_=phrase         # category__phrase
         | CATEGORY_MARK word_=word             # category__word
         ;

category_paradigm       : PARADIGM_START categories+=category            (PARADIGM_SEP categories+=category)*            PARADIGM_END;

invariant_paranode_mapping : CATEGORY_MARK invariant=identifier MAPPING paranode=identifier;

inflection_list: (FLEXION_MARK inflections+=identifier)+  # inflection_list__identifiers
               | (FLEXION_MARK words+=word)+              # inflection_list__words
               ;
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

inflected_category : inflection_list_=inflection_list?                category_=category            references+=reference*    #inflected_category__singular 
                   | inflection_list_=inflection_list?                category_=category_paradigm                             #inflected_category__category_paradigm
                   | inflection_list_=inflection_list_paradigm        category_=category                                      #inflected_category__inflection_paradigm
                   | inflection_list_=inflection_list_paradigm        category_=category_paradigm                             #inflected_category__inflection_and_category_paradigm
                   ;

variable: VARIABLE;

reference : REFERENCE_OPEN variable_=variable REFERENCE_CLOSE;

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

dimension_definition : dimension_mark=DIMENSION_MARK paths+=path (PARADIGM_SEP paths+=path)*;

role_type : INTEGER                   # role_type__integer
          | identifier_=identifier    # role_type__identifier
          ;
   
identifier : identifiers+=IDENTIFIER+ ;
