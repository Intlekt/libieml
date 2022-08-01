#include "IEMLGrammarVisitor.h"

#include "ast/interfaces/IDeclaration.h"
#include "ast/declaration/AuxiliaryDeclaration.h"
#include "ast/declaration/CategoryDeclaration.h"
#include "ast/declaration/ComponentDeclaration.h"
#include "ast/declaration/InflectionDeclaration.h"
#include "ast/declaration/JunctionDeclaration.h"
#include "ast/declaration/LanguageDeclaration.h"
#include "ast/declaration/NodeDeclaration.h"
#include "ast/declaration/ParanodeDeclaration.h"
#include "ast/declaration/WordDeclaration.h"
#include "ast/declaration/LinkDeclaration.h"
#include "ast/declaration/TableDeclaration.h"
#include "ast/declaration/RootParadigmDeclaration.h"
#include "ast/declaration/FunctionDeclaration.h"
#include "ast/Constants.h"
#include "ast/Identifier.h"
#include "ast/Program.h"
#include "ast/Phrase.h"
#include "ast/Reference.h"
#include "ast/Word.h"
#include "ast/CategoryParadigm.h"
#include "ast/InflectedCategory.h"
#include "ast/ReferenceStringValue.h"
#include "ast/InflectionList.h"
#include "ast/InflectionListParadigm.h"
#include "ast/Auxiliary.h"
#include "ast/AuxiliaryParadigm.h"
#include "ast/Path.h"
#include "ast/Junction.h"
#include "ast/DimensionDefinition.h"
#include "ast/RoleType.h"
#include "ast/ArgumentList.h"
#include "ast/TemplateLanguageString.h"
#include "ast/function/WordFunctionConditionExpr.h"

#define RETURN_VISITOR_RESULT_NO_ARGS(ReturnType, DerivedType) \
  return antlrcpp::Any(VisitorResult<std::shared_ptr<ReturnType>>(std::make_unique<DerivedType>(charRangeFromContext(ctx))));

#define RETURN_VISITOR_RESULT(ReturnType, DerivedType, ...) \
  return antlrcpp::Any(VisitorResult<std::shared_ptr<ReturnType>>(std::make_unique<DerivedType>(charRangeFromContext(ctx), __VA_ARGS__)));

#define RETURN_VISITOR_RESULT_MOVE(ReturnType, UNIQUE_PTR) \
  return antlrcpp::Any(VisitorResult<std::shared_ptr<ReturnType>>(std::move(UNIQUE_PTR)));

#define RETURN_VISITOR_RESULT_ERROR(ReturnType) \
  return antlrcpp::Any(VisitorResult<std::shared_ptr<ReturnType>>());

#define CHECK_SYNTAX_ERROR(ErrorListener, Context, Attribute, Message, Required)                             \
  antlrcpp::Any t_##Attribute;                                                                               \
  bool valid_##Attribute = true;                                                                             \
  if (Context->Attribute)                                                                                    \
  {                                                                                                          \
    t_##Attribute = visit(Context->Attribute);                                                               \
    if (t_##Attribute.isNull())                                                                              \
    {                                                                                                        \
      ErrorListener->parseError(*charRangeFromContext(Context), Message);                                    \
      valid_##Attribute = false;                                                                             \
    }                                                                                                        \
  }                                                                                                          \
  else if (Required)                                                                                         \
  {                                                                                                          \
    ErrorListener->parseError(*charRangeFromContext(Context), "Missing required " #Attribute " : " Message); \
    valid_##Attribute = false;                                                                               \
  }

#define CHECK_SYNTAX_ERROR_LIST(ErrorListener, Context, Type, Attribute, Message) \
  std::vector<std::shared_ptr<Type>> Attribute;                                   \
  __attribute__((unused)) bool valid_##Attribute = true;                          \
  for (auto child : Context->Attribute)                                           \
  {                                                                               \
    auto t_tmp = visit(child);                                                    \
    if (t_tmp.isNull())                                                           \
    {                                                                             \
      ErrorListener->parseError(*charRangeFromContext(Context), Message);         \
      valid_##Attribute = false;                                                  \
    }                                                                             \
    else                                                                          \
    {                                                                             \
      auto _tmp = std::move(t_tmp.as<VisitorResult<std::shared_ptr<Type>>>());    \
      if (_tmp.isError())                                                         \
        valid_##Attribute = false;                                                \
      else                                                                        \
        Attribute.emplace_back(std::move(_tmp.release()));                        \
    }                                                                             \
  }

#define CAST_OR_RETURN_IF_NULL(Context, Type, Attribute, ReturnType)                 \
  if (!valid_##Attribute)                                                            \
    RETURN_VISITOR_RESULT_ERROR(ReturnType);                                         \
  std::shared_ptr<Type> Attribute;                                                   \
  if (Context->Attribute)                                                            \
  {                                                                                  \
    auto _tmp = std::move(t_##Attribute.as<VisitorResult<std::shared_ptr<Type>>>()); \
    if (_tmp.isError())                                                              \
      RETURN_VISITOR_RESULT_ERROR(ReturnType);                                       \
    Attribute = std::move(_tmp.release());                                           \
  }

#define CAST_OR_RETURN_IF_NULL_LIST(Attribute, ReturnType) \
  if (!valid_##Attribute)                                  \
    RETURN_VISITOR_RESULT_ERROR(ReturnType);

#define CAST_OR_RETURN_IF_NULL_LIST_SIZED(ErrorListener, Context, Attribute, ReturnType, Size)                          \
  CAST_OR_RETURN_IF_NULL_LIST(Attribute, ReturnType)                                                                    \
  if (Attribute.size() != Size)                                                                                         \
  {                                                                                                                     \
    ErrorListener->parseError(*charRangeFromContext(Context), "Invalid number of " #Attribute ", expected " #Size "."); \
    RETURN_VISITOR_RESULT_ERROR(ReturnType);                                                                            \
  }

using namespace ieml::parser;

/**
 * PROGRAM
 */

antlrcpp::Any IEMLGrammarVisitor::visitProgram(IEMLParserGrammar::ProgramContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, IDeclaration, declarations, "Invalid declaration.");

  // declarations contains the syntax valid declarations
  // we dont return with CAST_OR_RETURN_IF_NULL_LIST to enable contextual error checking on the valid part
  RETURN_VISITOR_RESULT(Program, Program, std::move(declarations));
}

/**
 * DECLARATION
 */

antlrcpp::Any IEMLGrammarVisitor::visitComponentDeclaration(IEMLParserGrammar::ComponentDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase definition in component declaration.", true);
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, LanguageString, language_strings, "Invalid language string.");

  CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_, IDeclaration);
  CAST_OR_RETURN_IF_NULL_LIST(language_strings, IDeclaration);

  RETURN_VISITOR_RESULT(IDeclaration, ComponentDeclaration, std::move(language_strings), std::move(phrase_));
}

antlrcpp::Any IEMLGrammarVisitor::visitNodeDeclaration(IEMLParserGrammar::NodeDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase definition in node declaration.", true);
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, LanguageString, language_strings, "Invalid language string.");

  CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_, IDeclaration);
  CAST_OR_RETURN_IF_NULL_LIST(language_strings, IDeclaration);

  RETURN_VISITOR_RESULT(IDeclaration, NodeDeclaration, std::move(language_strings), std::move(phrase_));
}

antlrcpp::Any IEMLGrammarVisitor::visitParanodeDeclaration(IEMLParserGrammar::ParanodeDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase definition in node declaration.", true);
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, DimensionDefinition, dimensions, "Invalid dimension definition.");
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, LanguageString, language_strings, "Invalid language string.");

  CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_, IDeclaration);
  CAST_OR_RETURN_IF_NULL_LIST(language_strings, IDeclaration);
  CAST_OR_RETURN_IF_NULL_LIST(dimensions, IDeclaration);

  RETURN_VISITOR_RESULT(IDeclaration, ParanodeDeclaration, std::move(language_strings), std::move(dimensions), std::move(phrase_));
}

antlrcpp::Any IEMLGrammarVisitor::visitWordDeclaration(IEMLParserGrammar::WordDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_, "Invalid word for a word declaration.", true);

  CAST_OR_RETURN_IF_NULL(ctx, Word, word_, IDeclaration);

  RETURN_VISITOR_RESULT(IDeclaration, WordDeclaration, std::move(word_));
}

antlrcpp::Any IEMLGrammarVisitor::visitInflectionDeclaration(IEMLParserGrammar::InflectionDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, LanguageString, language_strings, "Invalid language string.");

  std::shared_ptr<Identifier> inflection_type;
  if (!ctx->inflection_type)
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid inflection type for an inflection declaration. Expected 'NOUN' or 'VERB'.");
  else
    inflection_type = std::make_unique<Identifier>(charRangeFromToken(ctx->inflection_type), ctx->inflection_type->getText());

  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_, "Invalid word for an inflection declaration.", true);

  CAST_OR_RETURN_IF_NULL(ctx, Word, word_, IDeclaration);
  if (inflection_type == nullptr)
    RETURN_VISITOR_RESULT_ERROR(IDeclaration);
  CAST_OR_RETURN_IF_NULL_LIST(language_strings, IDeclaration);

  RETURN_VISITOR_RESULT(IDeclaration,
                        InflectionDeclaration,
                        std::move(language_strings),
                        std::move(inflection_type),
                        std::move(word_));
}

antlrcpp::Any IEMLGrammarVisitor::visitAuxiliaryDeclaration(IEMLParserGrammar::AuxiliaryDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, LanguageString, language_strings, "Invalid language string.");
  CHECK_SYNTAX_ERROR(error_listener_, ctx, role_type_, "Invalid role number for an auxiliary declaration.", true);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_, "Invalid word for an auxiliary declaration.", true);

  CAST_OR_RETURN_IF_NULL(ctx, Word, word_, IDeclaration);
  CAST_OR_RETURN_IF_NULL(ctx, RoleType, role_type_, IDeclaration);
  CAST_OR_RETURN_IF_NULL_LIST(language_strings, IDeclaration);

  RETURN_VISITOR_RESULT(IDeclaration,
                        AuxiliaryDeclaration,
                        std::move(language_strings),
                        std::move(role_type_),
                        std::move(word_));
}

antlrcpp::Any IEMLGrammarVisitor::visitJunctionDeclaration(IEMLParserGrammar::JunctionDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, LanguageString, language_strings, "Invalid language string.");

  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_, "Invalid word for a junction declaration.", true);

  CAST_OR_RETURN_IF_NULL(ctx, Word, word_, IDeclaration);
  CAST_OR_RETURN_IF_NULL_LIST(language_strings, IDeclaration);

  RETURN_VISITOR_RESULT(IDeclaration,
                        JunctionDeclaration,
                        std::move(language_strings),
                        std::move(word_));
}

antlrcpp::Any IEMLGrammarVisitor::visitLanguageDeclaration(IEMLParserGrammar::LanguageDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, language, "Invalid language type.", true);

  CAST_OR_RETURN_IF_NULL(ctx, Identifier, language, IDeclaration);

  RETURN_VISITOR_RESULT(IDeclaration,
                        LanguageDeclaration,
                        std::move(language));
}

antlrcpp::Any IEMLGrammarVisitor::visitTableDeclaration(IEMLParserGrammar::TableDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, InvariantParanodeMapping, category_mappings, "Invalid invariant to paranode mappings.");
  CAST_OR_RETURN_IF_NULL_LIST(category_mappings, IDeclaration);

  RETURN_VISITOR_RESULT(IDeclaration,
                        TableDeclaration,
                        std::move(category_mappings));
}

antlrcpp::Any IEMLGrammarVisitor::visitRootParadigmDeclaration(IEMLParserGrammar::RootParadigmDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_, "Invalid word for a root paradigm declaration.", true);
  std::shared_ptr<Identifier> root_type;
  if (!ctx->type_root)
    error_listener_->parseError(*charRangeFromContext(ctx),
                                "Invalid root type for an root paradigm declaration. "
                                "Expected 'CATEGORY', 'INFLECTION', 'AUXILIARY' or 'JUNCTION'.");
  else
    root_type = std::make_unique<Identifier>(charRangeFromToken(ctx->type_root), ctx->type_root->getText());

  CAST_OR_RETURN_IF_NULL(ctx, Word, word_, IDeclaration);
  if (root_type == nullptr)
    RETURN_VISITOR_RESULT_ERROR(IDeclaration);

  RETURN_VISITOR_RESULT(IDeclaration,
                        RootParadigmDeclaration,
                        std::move(root_type),
                        std::move(word_));
}

antlrcpp::Any IEMLGrammarVisitor::visitLinkDeclaration(IEMLParserGrammar::LinkDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase definition in node declaration.", true);
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, LanguageString, language_strings, "Invalid language string.");
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, TemplateLanguageString, template_language_strings, "Invalid template language string.");
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, IInflectionList, inflection_lists, "Invalid inflection list.");
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, ArgumentList, arguments, "Invalid inflection list.");

  CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_, IDeclaration);
  CAST_OR_RETURN_IF_NULL_LIST(language_strings, IDeclaration);
  CAST_OR_RETURN_IF_NULL_LIST(template_language_strings, IDeclaration);
  CAST_OR_RETURN_IF_NULL_LIST(inflection_lists, IDeclaration);
  CAST_OR_RETURN_IF_NULL_LIST_SIZED(error_listener_, ctx, arguments, IDeclaration, 1);

  if (inflection_lists.size() > 1)
  {
    error_listener_->parseError(*charRangeFromContext(ctx),
                                "Invalid number of phrase word inflection list, expected 0 or 1.");
    RETURN_VISITOR_RESULT_ERROR(IDeclaration);
  }
  ieml::AST::IInflectionList::Ptr inflection = nullptr;
  if (inflection_lists.size() == 1)
    inflection = inflection_lists[0];

  RETURN_VISITOR_RESULT(IDeclaration,
                        LinkDeclaration,
                        std::move(arguments[0]),
                        std::move(language_strings),
                        std::move(template_language_strings),
                        std::move(inflection),
                        std::move(phrase_));
}
antlrcpp::Any IEMLGrammarVisitor::visitFunctionDeclaration(IEMLParserGrammar::FunctionDeclarationContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Identifier, links, "Invalid function link definition.");
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, IFunctionDomainList, domains, "Invalid function domain definition.");
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, WordFunctionConditionExpr, conditions, "Invalid function condition definition.");

  if (ctx->types.size() != 1)
  {
    error_listener_->parseError(*charRangeFromContext(ctx),
                                "Invalid number of function definition type, expected 1.");
    RETURN_VISITOR_RESULT_ERROR(IDeclaration);
  }
  if (!ctx->types[0])
  {
    error_listener_->parseError(*charRangeFromContext(ctx),
                                "Invalid function definition type");
    RETURN_VISITOR_RESULT_ERROR(IDeclaration);
  }

  const auto ftype = FunctionType::_from_string_nocase_nothrow(ctx->types[0]->getText().c_str());
  if (!ftype)
  {
    error_listener_->parseError(*charRangeFromContext(ctx),
                                "Invalid function definition type");
    RETURN_VISITOR_RESULT_ERROR(IDeclaration);
  }

  CAST_OR_RETURN_IF_NULL_LIST_SIZED(error_listener_, ctx, links, IDeclaration, 1);
  CAST_OR_RETURN_IF_NULL_LIST_SIZED(error_listener_, ctx, domains, IDeclaration, 1);
  CAST_OR_RETURN_IF_NULL_LIST_SIZED(error_listener_, ctx, conditions, IDeclaration, 1);

  RETURN_VISITOR_RESULT(IDeclaration,
                        FunctionDeclaration,
                        *ftype,
                        std::move(links[0]),
                        std::move(domains[0]),
                        std::move(conditions[0]));
}

/**
 * PHRASE
 */

antlrcpp::Any IEMLGrammarVisitor::visitPhrase__lines(IEMLParserGrammar::Phrase__linesContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, PhraseLine, phrase_lines, "Invalid phrase lines.");
  CAST_OR_RETURN_IF_NULL_LIST(phrase_lines, Phrase);

  RETURN_VISITOR_RESULT(Phrase, SimplePhrase, std::move(phrase_lines));
}

antlrcpp::Any IEMLGrammarVisitor::visitPhrase__junction(IEMLParserGrammar::Phrase__junctionContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Phrase, phrases, "Invalid phrases in phrase junction.");
  CHECK_SYNTAX_ERROR(error_listener_, ctx, junction_, "Invalid junction identifier in phrase junction.", true);

  CAST_OR_RETURN_IF_NULL_LIST(phrases, Phrase);
  CAST_OR_RETURN_IF_NULL(ctx, IJunction, junction_, Phrase);

  RETURN_VISITOR_RESULT(Phrase, JunctionPhrase, std::move(phrases), std::move(junction_));
}

/**
 * PHRASELINE
 */

antlrcpp::Any IEMLGrammarVisitor::visitPhrase_line__sub_phrase_line_auxiliary(IEMLParserGrammar::Phrase_line__sub_phrase_line_auxiliaryContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, role_type_, "Invalid role type.", true);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, sub_phrase, "Invalid sub phrase line : invalid auxiliary, inflection, category or references.", true);

  CAST_OR_RETURN_IF_NULL(ctx, AuxiliarySubPhraseLine, sub_phrase, PhraseLine);
  CAST_OR_RETURN_IF_NULL(ctx, RoleType, role_type_, PhraseLine);

  bool accentuation = ctx->accentuation;

  RETURN_VISITOR_RESULT(PhraseLine, SimplePhraseLine, std::move(role_type_), accentuation, std::move(sub_phrase))
}

antlrcpp::Any IEMLGrammarVisitor::visitPhrase_line__jonction_auxiliary(IEMLParserGrammar::Phrase_line__jonction_auxiliaryContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, role_type_, "Invalid role type.", true);
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, AuxiliarySubPhraseLine, sub_phrases, "Invalid sub phrase in phrase line junction.");
  CHECK_SYNTAX_ERROR(error_listener_, ctx, junction_, "Invalid junction identifier in phrase line junction.", true);

  bool accentuation = ctx->accentuation;

  CAST_OR_RETURN_IF_NULL_LIST(sub_phrases, PhraseLine);
  CAST_OR_RETURN_IF_NULL(ctx, IJunction, junction_, PhraseLine);
  CAST_OR_RETURN_IF_NULL(ctx, RoleType, role_type_, PhraseLine);

  RETURN_VISITOR_RESULT(PhraseLine, JunctionPhraseLine, std::move(sub_phrases), std::move(junction_), std::move(role_type_), accentuation);
}

/**
 * INFLEXION LIST
 */
antlrcpp::Any IEMLGrammarVisitor::visitInflection_list__identifiers(IEMLParserGrammar::Inflection_list__identifiersContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Identifier, inflections, "Invalid inflection identifiers in phrase line or path.");
  CAST_OR_RETURN_IF_NULL_LIST(inflections, IInflectionList);

  RETURN_VISITOR_RESULT(IInflectionList, InflectionList, std::move(inflections));
}

antlrcpp::Any IEMLGrammarVisitor::visitInflection_list__words(IEMLParserGrammar::Inflection_list__wordsContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Word, words, "Invalid inflection words in phrase line or path.");
  CAST_OR_RETURN_IF_NULL_LIST(words, IInflectionList);

  RETURN_VISITOR_RESULT(IInflectionList, WordInflectionList, std::move(words));
}

antlrcpp::Any IEMLGrammarVisitor::visitInflection_list_paradigm(IEMLParserGrammar::Inflection_list_paradigmContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, IInflectionList, inflection_lists, "Invalid inflection list in inflection paradigm.");
  CAST_OR_RETURN_IF_NULL_LIST(inflection_lists, IInflectionList);

  RETURN_VISITOR_RESULT(IInflectionList, InflectionListParadigm, std::move(inflection_lists));
}

/**
 * AUXILIARY
 */
antlrcpp::Any IEMLGrammarVisitor::visitAuxiliary__identifier(IEMLParserGrammar::Auxiliary__identifierContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, identifier_, "Invalid auxiliary identifier in a phrase line or a path.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Identifier, identifier_, Auxiliary);

  RETURN_VISITOR_RESULT(IAuxiliary, Auxiliary, std::move(identifier_));
}

antlrcpp::Any IEMLGrammarVisitor::visitAuxiliary__word(IEMLParserGrammar::Auxiliary__wordContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_, "Invalid auxiliary word in a phrase line or a path.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Word, word_, Auxiliary);

  RETURN_VISITOR_RESULT(IAuxiliary, WordAuxiliary, std::move(word_));
}

antlrcpp::Any IEMLGrammarVisitor::visitAuxiliary_paradigm(IEMLParserGrammar::Auxiliary_paradigmContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, IAuxiliary, identifiers, "Invalid auxiliary in an auxiliary paradigm.");
  CAST_OR_RETURN_IF_NULL_LIST(identifiers, IAuxiliary);

  RETURN_VISITOR_RESULT(IAuxiliary, AuxiliaryParadigm, std::move(identifiers));
}

/**
 * AUXILIARY SIMPLE OR PARADIGM
 */
antlrcpp::Any IEMLGrammarVisitor::visitAuxiliary_simple_or_paradigm__simple(IEMLParserGrammar::Auxiliary_simple_or_paradigm__simpleContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, auxiliary_, "Invalid auxiliary.", true);
  CAST_OR_RETURN_IF_NULL(ctx, IAuxiliary, auxiliary_, IAuxiliary);

  RETURN_VISITOR_RESULT_MOVE(IAuxiliary, auxiliary_);
}

antlrcpp::Any IEMLGrammarVisitor::visitAuxiliary_simple_or_paradigm__paradigm(IEMLParserGrammar::Auxiliary_simple_or_paradigm__paradigmContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, auxiliary_, "Invalid auxiliary paradigm.", true);
  CAST_OR_RETURN_IF_NULL(ctx, IAuxiliary, auxiliary_, IAuxiliary);

  RETURN_VISITOR_RESULT_MOVE(IAuxiliary, auxiliary_);
}

/**
 * SUB PHRASELINE with AUXILIARY
 */

antlrcpp::Any IEMLGrammarVisitor::visitSub_phrase_line_auxiliary__no_junction(IEMLParserGrammar::Sub_phrase_line_auxiliary__no_junctionContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, auxiliary_, "Invalid auxiliary identifier.", false);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, inflected_category_, "Missing a category : an identifier, a phrase or a word.", true);

  CAST_OR_RETURN_IF_NULL(ctx, InflectedCategory, inflected_category_, AuxiliarySubPhraseLine);
  CAST_OR_RETURN_IF_NULL(ctx, IAuxiliary, auxiliary_, AuxiliarySubPhraseLine);

  RETURN_VISITOR_RESULT(AuxiliarySubPhraseLine, SimpleAuxiliarySubPhraseLine, std::move(auxiliary_), std::move(inflected_category_));
}

antlrcpp::Any IEMLGrammarVisitor::visitSub_phrase_line_auxiliary__jonction(IEMLParserGrammar::Sub_phrase_line_auxiliary__jonctionContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, auxiliary_, "Invalid auxiliary identifier.", true);
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, InflectedCategory, inflected_categories, "Invalid inflected categories in auxiliarized phrase line junction.");
  CHECK_SYNTAX_ERROR(error_listener_, ctx, junction_, "Invalid junction type identifier.", true);

  CAST_OR_RETURN_IF_NULL(ctx, IAuxiliary, auxiliary_, AuxiliarySubPhraseLine);
  CAST_OR_RETURN_IF_NULL(ctx, IJunction, junction_, AuxiliarySubPhraseLine);
  CAST_OR_RETURN_IF_NULL_LIST(inflected_categories, AuxiliarySubPhraseLine)

  RETURN_VISITOR_RESULT(AuxiliarySubPhraseLine, JunctionAuxiliarySubPhraseLine, std::move(auxiliary_), std::move(inflected_categories), std::move(junction_));
}

/**
 * INFLEXED CATEGORY (SUB PHRASELINE without AUXILIARY)
 */
antlrcpp::Any IEMLGrammarVisitor::visitInflected_category__singular(IEMLParserGrammar::Inflected_category__singularContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, inflection_list_, "Invalid inflection list in inflected category.", false);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, category_, "Missing a category : an identifier, a phrase or a word.", true);
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Reference, references, "Invalid reference.");

  CAST_OR_RETURN_IF_NULL(ctx, IInflectionList, inflection_list_, InflectedCategory);
  CAST_OR_RETURN_IF_NULL(ctx, ICategory, category_, InflectedCategory);
  CAST_OR_RETURN_IF_NULL_LIST(references, InflectedCategory);

  RETURN_VISITOR_RESULT(InflectedCategory, InflectedCategory, std::move(inflection_list_), std::move(category_), std::move(references));
}
antlrcpp::Any IEMLGrammarVisitor::visitInflected_category__category_paradigm(IEMLParserGrammar::Inflected_category__category_paradigmContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, inflection_list_, "Invalid inflection list in inflected category.", false);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, category_, "Missing a substitution list of category.", true);

  CAST_OR_RETURN_IF_NULL(ctx, IInflectionList, inflection_list_, InflectedCategory);
  CAST_OR_RETURN_IF_NULL(ctx, CategoryParadigm, category_, InflectedCategory);

  RETURN_VISITOR_RESULT(InflectedCategory, InflectedCategory, std::move(inflection_list_), std::move(category_), std::vector<std::shared_ptr<Reference>>{});
}
antlrcpp::Any IEMLGrammarVisitor::visitInflected_category__inflection_paradigm(IEMLParserGrammar::Inflected_category__inflection_paradigmContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, inflection_list_, "Invalid inflection list in inflected category.", true);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, category_, "Missing a substitution list of category.", true);

  CAST_OR_RETURN_IF_NULL(ctx, IInflectionList, inflection_list_, InflectedCategory);
  CAST_OR_RETURN_IF_NULL(ctx, ICategory, category_, InflectedCategory);

  RETURN_VISITOR_RESULT(InflectedCategory, InflectedCategory, std::move(inflection_list_), std::move(category_), std::vector<std::shared_ptr<Reference>>{});
}

antlrcpp::Any IEMLGrammarVisitor::visitInflected_category__inflection_and_category_paradigm(IEMLParserGrammar::Inflected_category__inflection_and_category_paradigmContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, inflection_list_, "Invalid inflection list in inflected category.", true);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, category_, "Missing a substitution list of category.", true);

  CAST_OR_RETURN_IF_NULL(ctx, IInflectionList, inflection_list_, InflectedCategory);
  CAST_OR_RETURN_IF_NULL(ctx, CategoryParadigm, category_, InflectedCategory);

  RETURN_VISITOR_RESULT(InflectedCategory, InflectedCategory, std::move(inflection_list_), std::move(category_), std::vector<std::shared_ptr<Reference>>{});
}

/**
 *  CATEGORY
 */

antlrcpp::Any IEMLGrammarVisitor::visitCategory__identifier(IEMLParserGrammar::Category__identifierContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, identifier_, "Invalid identifier for a category.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Identifier, identifier_, ICategory);

  RETURN_VISITOR_RESULT_MOVE(ICategory, identifier_);
}

antlrcpp::Any IEMLGrammarVisitor::visitCategory__phrase(IEMLParserGrammar::Category__phraseContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase for a category.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_, ICategory);

  RETURN_VISITOR_RESULT_MOVE(ICategory, phrase_);
}

antlrcpp::Any IEMLGrammarVisitor::visitCategory__word(IEMLParserGrammar::Category__wordContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_, "Invalid word for a category.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Word, word_, ICategory);
  RETURN_VISITOR_RESULT_MOVE(ICategory, word_);
}

antlrcpp::Any IEMLGrammarVisitor::visitCategory_paradigm(IEMLParserGrammar::Category_paradigmContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, ICategory, categories, "Invalid category list for a paradigm declaration.");

  CAST_OR_RETURN_IF_NULL_LIST(categories, CategoryParadigm);

  RETURN_VISITOR_RESULT(CategoryParadigm, CategoryParadigm, std::move(categories));
}

/**
 *  INVARIANT TO PARANODE MAPPING
 */
antlrcpp::Any IEMLGrammarVisitor::visitInvariant_paranode_mapping(IEMLParserGrammar::Invariant_paranode_mappingContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, invariant, "Invalid invariant identifier for a invariant to paranode mapping.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Identifier, invariant, InvariantParanodeMapping);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, paranode, "Invalid paranode identifier for a invariant to paranode mapping.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Identifier, paranode, InvariantParanodeMapping);

  RETURN_VISITOR_RESULT(InvariantParanodeMapping, InvariantParanodeMapping, std::move(invariant), std::move(paranode));
}

/**
 *  WORD
 */

antlrcpp::Any IEMLGrammarVisitor::visitWord(IEMLParserGrammar::WordContext *ctx)
{
  if (!ctx->word_)
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid word for a category.");
    RETURN_VISITOR_RESULT_ERROR(Word);
  }

  std::string word_str_quoted = ctx->word_->getText();
  std::string word_str = word_str_quoted.substr(1, word_str_quoted.size() - 2);

  RETURN_VISITOR_RESULT(Word, Word, word_str);
}

/**
 *  LANGUAGE STRING
 */

antlrcpp::Any IEMLGrammarVisitor::visitLanguage_string(IEMLParserGrammar::Language_stringContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, value, "Invalid identifier value for language string.", true);
  if (!ctx->language)
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid language identifier for language string.");
    RETURN_VISITOR_RESULT_ERROR(LanguageString);
  }

  std::shared_ptr<Identifier> language = std::make_shared<Identifier>(charRangeFromToken(ctx->language),
                                                                      ctx->language->getText().substr(0, 2));
  CAST_OR_RETURN_IF_NULL(ctx, Identifier, value, LanguageString)

  RETURN_VISITOR_RESULT(LanguageString, LanguageString, std::move(language), std::move(value))
}

antlrcpp::Any IEMLGrammarVisitor::visitTemplate_language_string(IEMLParserGrammar::Template_language_stringContext *ctx)
{
  if (!ctx->language)
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid language identifier for language string.");
    RETURN_VISITOR_RESULT_ERROR(TemplateLanguageString);
  }
  std::shared_ptr<Identifier> language = std::make_shared<Identifier>(charRangeFromToken(ctx->language),
                                                                      ctx->language->getText().substr(0, 11));

  std::vector<std::pair<TemplateLanguageString::SType, std::string>> values;
  for (const auto &token : ctx->value)
  {
    if (!token)
    {
      error_listener_->parseError(*charRangeFromContext(ctx), "Invalid template language string value.");
      RETURN_VISITOR_RESULT_ERROR(TemplateLanguageString);
    }
    TemplateLanguageString::SType type;
    const auto text = token->getText();
    if (text[0] == '$')
      type = TemplateLanguageString::SType::VARIABLE;
    else
      type = TemplateLanguageString::SType::STRING;

    values.push_back({type, text});
  }

  RETURN_VISITOR_RESULT(TemplateLanguageString, TemplateLanguageString, std::move(language), std::move(values))
}

/**
 *  IDENTIFIER
 */

antlrcpp::Any IEMLGrammarVisitor::visitIdentifier(IEMLParserGrammar::IdentifierContext *ctx)
{
  if (ctx->identifiers.empty())
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid identifier : empty identifier.");
    RETURN_VISITOR_RESULT_ERROR(Identifier);
  }

  std::ostringstream os;
  bool first = true;
  for (antlr4::Token *token : ctx->identifiers)
  {
    if (first)
      first = false;
    else
      os << " ";

    os << token->getText();
  }

  RETURN_VISITOR_RESULT(Identifier, Identifier, os.str());
}
/**
 * JUNCTION
 */
antlrcpp::Any IEMLGrammarVisitor::visitJunction__identifier(IEMLParserGrammar::Junction__identifierContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, identifier_, "Invalid identifier for a junction.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Identifier, identifier_, IJunction);
  RETURN_VISITOR_RESULT(IJunction, Junction, std::move(identifier_));
}
antlrcpp::Any IEMLGrammarVisitor::visitJunction__word(IEMLParserGrammar::Junction__wordContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_, "Invalid word for a junction.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Word, word_, IJunction);
  RETURN_VISITOR_RESULT(IJunction, WordJunction, std::move(word_));
}

/**
 * PATH
 */
antlrcpp::Any IEMLGrammarVisitor::visitPath(IEMLParserGrammar::PathContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, PathNode, path_nodes, "Invalid path node list for a path definition.");
  CAST_OR_RETURN_IF_NULL_LIST(path_nodes, Path);
  RETURN_VISITOR_RESULT(Path, Path, std::move(path_nodes));
}

/**
 * PATH NODE
 */
antlrcpp::Any IEMLGrammarVisitor::visitPath_node__root(IEMLParserGrammar::Path_node__rootContext *ctx)
{
  RETURN_VISITOR_RESULT_NO_ARGS(PathNode, RootPathNode);
}

antlrcpp::Any IEMLGrammarVisitor::visitPath_node__paradigm(IEMLParserGrammar::Path_node__paradigmContext *ctx)
{
  if (!ctx->INTEGER())
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid paradigm index.");
    RETURN_VISITOR_RESULT_ERROR(PathNode);
  }
  size_t index = std::stoi(ctx->INTEGER()->getText());
  RETURN_VISITOR_RESULT(PathNode, ParadigmPathNode, index);
}

antlrcpp::Any IEMLGrammarVisitor::visitPath_node__auxiliary(IEMLParserGrammar::Path_node__auxiliaryContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, auxiliary_, "Invalid auxiliary for a path definition.", true);
  CAST_OR_RETURN_IF_NULL(ctx, IAuxiliary, auxiliary_, PathNode);
  RETURN_VISITOR_RESULT(PathNode, AuxiliaryPathNode, std::move(auxiliary_));
}

antlrcpp::Any IEMLGrammarVisitor::visitPath_node__inflection(IEMLParserGrammar::Path_node__inflectionContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, inflection_list_, "Invalid inflection list for a path definition.", true);
  CAST_OR_RETURN_IF_NULL(ctx, IInflectionList, inflection_list_, PathNode);
  RETURN_VISITOR_RESULT(PathNode, InflectionListPathNode, std::move(inflection_list_));
}

antlrcpp::Any IEMLGrammarVisitor::visitPath_node__word(IEMLParserGrammar::Path_node__wordContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_, "Invalid word for a path definition.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Word, word_, PathNode);
  RETURN_VISITOR_RESULT(PathNode, WordPathNode, std::move(word_));
}

antlrcpp::Any IEMLGrammarVisitor::visitPath_node__junction(IEMLParserGrammar::Path_node__junctionContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, junction_, "Invalid junction identifier for a path definition.", true);

  if (!ctx->INTEGER())
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid junction index.");
    RETURN_VISITOR_RESULT_ERROR(PathNode);
  }
  size_t index = std::stoi(ctx->INTEGER()->getText());

  CAST_OR_RETURN_IF_NULL(ctx, IJunction, junction_, PathNode);
  RETURN_VISITOR_RESULT(PathNode, JunctionPathNode, std::move(junction_), index);
}

antlrcpp::Any IEMLGrammarVisitor::visitPath_node__role_number(IEMLParserGrammar::Path_node__role_numberContext *ctx)
{
  if (!ctx->INTEGER())
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid role number");
    RETURN_VISITOR_RESULT_ERROR(PathNode);
  }
  size_t role_number = std::stoi(ctx->INTEGER()->getText());

  RETURN_VISITOR_RESULT(PathNode, RoleNumberPathNode, role_number);
}

/**
 * DIMENSION DEFINITION
 */
antlrcpp::Any IEMLGrammarVisitor::visitDimension_definition(IEMLParserGrammar::Dimension_definitionContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Path, paths, "Invalid path list for a paranode dimension definition.");

  if (!ctx->dimension_mark)
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid dimension mark for paranode dimension definition.");
    RETURN_VISITOR_RESULT_ERROR(DimensionDefinition);
  }

  auto dimension_mark = ctx->dimension_mark->getText();
  const size_t dimension_index = std::stoi(dimension_mark.substr(0, dimension_mark.find("d")));

  CAST_OR_RETURN_IF_NULL_LIST(paths, DimensionDefinition);

  RETURN_VISITOR_RESULT(DimensionDefinition, DimensionDefinition, dimension_index, std::move(paths))
}

/**
 * ROLE TYPE
 */
antlrcpp::Any IEMLGrammarVisitor::visitRole_type__integer(IEMLParserGrammar::Role_type__integerContext *ctx)
{
  int role_type = -1;
  if (!ctx->INTEGER())
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid role number.");
  else
    role_type = std::stoi(ctx->INTEGER()->getSymbol()->getText());

  if (role_type == -1)
    RETURN_VISITOR_RESULT_ERROR(RoleType);

  RETURN_VISITOR_RESULT(RoleType, IntegerRoleType, role_type);
}
antlrcpp::Any IEMLGrammarVisitor::visitRole_type__identifier(IEMLParserGrammar::Role_type__identifierContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, identifier_, "Invalid identifier for role type.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Identifier, identifier_, RoleType);

  RETURN_VISITOR_RESULT(RoleType, IdentifierRoleType, std::move(identifier_));
}

/**
 *  VARIABLE
 */
antlrcpp::Any IEMLGrammarVisitor::visitVariable(IEMLParserGrammar::VariableContext *ctx)
{
  if (!ctx->VARIABLE())
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Missing variable name.");
    RETURN_VISITOR_RESULT_ERROR(Variable);
  }

  const std::string name = ctx->VARIABLE()->getText();

  RETURN_VISITOR_RESULT(Variable, Variable, std::move(name));
}

/**
 * WORD FUNCTION DOMAIN
 */
antlrcpp::Any IEMLGrammarVisitor::visitWord_domain_list(IEMLParserGrammar::Word_domain_listContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, WordFunctionDomain, variable_domains, "Invalid word domain in word domain list definition.");
  CAST_OR_RETURN_IF_NULL_LIST(variable_domains, IFunctionDomainList);

  RETURN_VISITOR_RESULT(IFunctionDomainList,
                        WordFunctionDomainList,
                        std::move(variable_domains));
}
antlrcpp::Any IEMLGrammarVisitor::visitWord_variable_domain(IEMLParserGrammar::Word_variable_domainContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, variable_, "Invalid variable for domain definition.", true);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_, "Invalid script for word variable domain definition.", true);

  if (!ctx->domain_selector)
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Missing domain selector.");
    RETURN_VISITOR_RESULT_ERROR(WordFunctionDomain);
  }

  if (ctx->domain_selector->getText() != "in")
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid domain selector.");
    RETURN_VISITOR_RESULT_ERROR(WordFunctionDomain);
  }

  CAST_OR_RETURN_IF_NULL(ctx, Variable, variable_, WordFunctionDomain);
  CAST_OR_RETURN_IF_NULL(ctx, Word, word_, WordFunctionDomain);

  RETURN_VISITOR_RESULT(WordFunctionDomain,
                        WordFunctionDomain,
                        std::move(variable_),
                        std::move(word_));
}

/**
 * WORD FUNCTION CONDITION
 */
antlrcpp::Any IEMLGrammarVisitor::visitWord_condition_function__word_condition(IEMLParserGrammar::Word_condition_function__word_conditionContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_condition_, "Invalid variable for domain definition.", true);
  CAST_OR_RETURN_IF_NULL(ctx, WordFunctionCondition, word_condition_, WordFunctionConditionExpr);

  RETURN_VISITOR_RESULT(WordFunctionConditionExpr,
                        PredicateWordFunctionConditionExpr,
                        std::move(word_condition_));
}

antlrcpp::Any IEMLGrammarVisitor::visitWord_condition_function__parenthesis(IEMLParserGrammar::Word_condition_function__parenthesisContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_condition_function_, "Invalid inner word condition function for word condition function parenthesis definition.", true);
  CAST_OR_RETURN_IF_NULL(ctx, WordFunctionConditionExpr, word_condition_function_, WordFunctionConditionExpr);

  RETURN_VISITOR_RESULT(WordFunctionConditionExpr,
                        ParenthesisWordFunctionConditionExpr,
                        std::move(word_condition_function_));
}
antlrcpp::Any IEMLGrammarVisitor::visitWord_condition_function__operator(IEMLParserGrammar::Word_condition_function__operatorContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, left_, "Invalid left operand in word condition function definition.", true);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, right_, "Invalid right operand in word condition function definition.", true);

  if (!ctx->operator_)
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "No operator in word condition function definition boolean operation.");
    RETURN_VISITOR_RESULT_ERROR(WordFunctionConditionExpr);
  }
  const auto operator_ = BooleanWordFunctionConditionExprOperatorType::_from_string_nocase_nothrow(ctx->operator_->getText().c_str());

  if (!operator_)
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid operator in word condition function definition boolean operation.");
    RETURN_VISITOR_RESULT_ERROR(WordFunctionConditionExpr);
  }

  CAST_OR_RETURN_IF_NULL(ctx, WordFunctionConditionExpr, left_, WordFunctionConditionExpr);
  CAST_OR_RETURN_IF_NULL(ctx, WordFunctionConditionExpr, right_, WordFunctionConditionExpr);

  RETURN_VISITOR_RESULT(WordFunctionConditionExpr,
                        BooleanWordFunctionConditionExpr,
                        std::move(left_),
                        std::move(right_),
                        *operator_);
}

antlrcpp::Any IEMLGrammarVisitor::visitWord_condition__equal(IEMLParserGrammar::Word_condition__equalContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, left_accessor, "Invalid left operand in word condition function definition.", true);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, right_accessor, "Invalid right operand in word condition function definition.", true);

  CAST_OR_RETURN_IF_NULL(ctx, WordAccessor, left_accessor, WordFunctionCondition);
  CAST_OR_RETURN_IF_NULL(ctx, WordAccessor, right_accessor, WordFunctionCondition);

  RETURN_VISITOR_RESULT(WordFunctionCondition,
                        WordFunctionConditionEquality,
                        std::move(left_accessor),
                        std::move(right_accessor))
}

antlrcpp::Any IEMLGrammarVisitor::visitWord_condition__not_equal(IEMLParserGrammar::Word_condition__not_equalContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, left_accessor, "Invalid left operand in word condition function definition.", true);
  CHECK_SYNTAX_ERROR(error_listener_, ctx, right_accessor, "Invalid right operand in word condition function definition.", true);

  CAST_OR_RETURN_IF_NULL(ctx, WordAccessor, left_accessor, WordFunctionCondition);
  CAST_OR_RETURN_IF_NULL(ctx, WordAccessor, right_accessor, WordFunctionCondition);

  RETURN_VISITOR_RESULT(WordFunctionCondition,
                        WordFunctionConditionInequality,
                        std::move(left_accessor),
                        std::move(right_accessor))
}

antlrcpp::Any IEMLGrammarVisitor::visitWord_accessor__variable(IEMLParserGrammar::Word_accessor__variableContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, accessor_, "Invalid variable for word accessor definition.", true);
  CAST_OR_RETURN_IF_NULL(ctx, WordAccessor, accessor_, WordAccessor);

  RETURN_VISITOR_RESULT_MOVE(WordAccessor, std::move(accessor_));
}

antlrcpp::Any IEMLGrammarVisitor::visitWord_accessor__literal(IEMLParserGrammar::Word_accessor__literalContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_literal, "Invalid literal for word accessor definition.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Word, word_literal, WordAccessor);

  RETURN_VISITOR_RESULT(WordAccessor,
                        WordLiteralAccessor,
                        std::move(word_literal));
}

antlrcpp::Any IEMLGrammarVisitor::visitWord_accessor_variable__word_accessor(IEMLParserGrammar::Word_accessor_variable__word_accessorContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, word_accessor_, "Invalid source operand in word accessor definition.", true);

  if (!ctx->accessor)
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid accessor operand in word accessor definition.");
    RETURN_VISITOR_RESULT_ERROR(WordAccessor);
  }

  const auto accessor = WordAccessorType::_from_string_nocase_nothrow(ctx->accessor->getText().c_str());
  if (!accessor)
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid accessor operand in word accessor definition.");
    RETURN_VISITOR_RESULT_ERROR(WordAccessor);
  }

  CAST_OR_RETURN_IF_NULL(ctx, WordAccessor, word_accessor_, WordAccessor);

  RETURN_VISITOR_RESULT(WordAccessor,
                        MultiplicativeWordAccessor,
                        std::move(word_accessor_),
                        *accessor);
}
antlrcpp::Any IEMLGrammarVisitor::visitWord_accessor_variable__variable(IEMLParserGrammar::Word_accessor_variable__variableContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, variable_, "Invalid variable in word accessor definition.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Variable, variable_, WordAccessor);

  RETURN_VISITOR_RESULT(WordAccessor,
                        VariableWordAccessor,
                        std::move(variable_));
}

/**
 *  REFERENCE
 */

antlrcpp::Any IEMLGrammarVisitor::visitReference(IEMLParserGrammar::ReferenceContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, variable_, "Invalid variable for reference.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Variable, variable_, Reference);

  RETURN_VISITOR_RESULT(Reference, Reference, std::move(variable_));
}

/**
 *  REFERENCE VALUE
 */

antlrcpp::Any IEMLGrammarVisitor::visitReference_value__identifier(IEMLParserGrammar::Reference_value__identifierContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, identifier_, "Invalid identifier for reference value.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Identifier, identifier_, IReferenceValue);

  RETURN_VISITOR_RESULT_MOVE(IReferenceValue, identifier_);
}

antlrcpp::Any IEMLGrammarVisitor::visitReference_value__STRING(IEMLParserGrammar::Reference_value__STRINGContext *ctx)
{
  if (!ctx->value)
  {
    error_listener_->parseError(*charRangeFromContext(ctx), "Invalid string for reference value.");
    RETURN_VISITOR_RESULT_ERROR(IReferenceValue);
  }

  RETURN_VISITOR_RESULT(IReferenceValue, ReferenceStringValue, ctx->value->getText());
}

antlrcpp::Any IEMLGrammarVisitor::visitReference_value__phrase(IEMLParserGrammar::Reference_value__phraseContext *ctx)
{
  CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase for reference value.", true);
  CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_, IReferenceValue);

  RETURN_VISITOR_RESULT_MOVE(IReferenceValue, phrase_);
}

/**
 * LINK ARGUMENTS
 */
antlrcpp::Any IEMLGrammarVisitor::visitLink_arguments(IEMLParserGrammar::Link_argumentsContext *ctx)
{
  CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Variable, arguments_, "Invalid argument list for link declaration.");
  CAST_OR_RETURN_IF_NULL_LIST(arguments_, ArgumentList);

  RETURN_VISITOR_RESULT(ArgumentList, ArgumentList, std::move(arguments_));
}
