parser grammar IEMLParserGrammar;

options { tokenVocab=IEMLLexerGrammar; }


program : declarations+=declaration* EOF;
   
declaration : COMPONENT     language_strings+=language_string+                                       phrase_=phrase                                                                                                              DOT    # componentDeclaration
            | NODE          language_strings+=language_string+                                       phrase_=phrase                                                                                                              DOT    # nodeDeclaration
            | PARANODE      language_strings+=language_string+ dimensions+=dimension_definition+     phrase_=phrase                                                                                                              DOT    # paranodeDeclaration
            | ROOT_PARADIGM                                    TYPE_MARK type_root=IDENTIFIER        word_=word                                                                                                                  DOT    # rootParadigmDeclaration
            | WORD                                                                                   word_=word                                                                                                                  DOT    # wordDeclaration
            | INFLECTION    language_strings+=language_string+ CLASS_MARK inflection_type=IDENTIFIER word_=word                                                                                                                  DOT    # inflectionDeclaration
            | AUXILIARY     language_strings+=language_string+ ROLE_MARK role_type_=role_type        word_=word                                                                                                                  DOT    # auxiliaryDeclaration
            | JUNCTION      language_strings+=language_string+                                       word_=word                                                                                                                  DOT    # junctionDeclaration
            | LANGUAGE      language=identifier                                                                                                                                                                                  DOT    # languageDeclaration
            | TABLE                                                    (category_mappings+=invariant_paranode_mapping)+                                                                                                          DOT    # tableDeclaration
            | LINK          (arguments+=link_arguments | language_strings+=language_string| PHRASE_WORD_INFLECTION_MARK inflection_lists+=inflection_list | template_language_strings+=template_language_string)+ phrase_=phrase DOT    # linkDeclaration
            | FUNCTION      (TYPE_MARK types+=IDENTIFIER | LINK_MARK links+=identifier | DOMAIN_MARK domains+=word_domain_list | CONDITION_MARK conditions+=word_condition_function)+                                            DOT    # functionDeclaration            
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

word_domain_list : PARENTHESIS_START variable_domains+=word_variable_domain (COMMA variable_domains+=word_variable_domain)* PARENTHESIS_END;

word_variable_domain : variable_=variable domain_selector=IDENTIFIER word_=word
                     ;

word_condition_function : PARENTHESIS_START word_condition_function_=word_condition_function PARENTHESIS_END                  # word_condition_function__parenthesis
                        | left_=word_condition_function operator_=IDENTIFIER right_=word_condition_function        # word_condition_function__operator
                        | word_condition_=word_condition                                                             # word_condition_function__word_condition
                        ;

word_condition : left_accessor=word_accessor EQUAL    right_accessor=word_accessor        # word_condition__equal
               | left_accessor=word_accessor NOTEQUAL right_accessor=word_accessor        # word_condition__not_equal
               ;

word_accessor : accessor_=word_accessor_variable # word_accessor__variable
							| word_literal=word # word_accessor__literal
							;

word_accessor_variable : word_accessor_=word_accessor_variable DOT accessor=IDENTIFIER        # word_accessor_variable__word_accessor
              | variable_=variable                                          # word_accessor_variable__variable
              ;


reference : REFERENCE_OPEN variable_=variable REFERENCE_CLOSE;

reference_value: identifier_=identifier  # reference_value__identifier
               | value=STRING            # reference_value__STRING
               | phrase_=phrase          # reference_value__phrase
               ;

language_string : language=LANGUAGE_STRING_MARK value=identifier;

// we have to use VARIABLE TOKEN because value has to hold an uniform type
template_language_string : language=LANGUAGE_STRING_TEMPLATE_MARK (value+=IDENTIFIER | value+=VARIABLE)+ ;

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

link_arguments: ARGUMENTS_MARK PARENTHESIS_START arguments_+=variable (COMMA arguments_+=variable)* PARENTHESIS_END;