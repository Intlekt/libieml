#include "AbstractVisitor.h"


using namespace ieml::parser;


std::shared_ptr<ieml::AST::CharRange> AbstractVisitor::charRangeFromToken(antlr4::Token* token) const {
    return std::make_unique<ieml::AST::CharRange>(file_id_, 
                                                  token->getLine() + line_offset_, 
                                                  token->getLine() + line_offset_, 
                                                  token->getCharPositionInLine() + char_offset_, 
                                                  token->getCharPositionInLine() + token->getText().size() + char_offset_);
}

std::shared_ptr<ieml::AST::CharRange> AbstractVisitor::charRangeFromContext(antlr4::ParserRuleContext* ctx) const {
    antlr4::Token* start = ctx->getStart();
    antlr4::Token* end   = ctx->getStop();

    size_t line_s, line_e, char_s, char_e;

    // these tests are for the case if start is after end for a empty production rule (see documentation for Token.getStart())
    if (start->getLine() < end->getLine()) {
        line_s = start->getLine();
        line_e = end->getLine();
        char_s = start->getCharPositionInLine();
        char_e = end->getCharPositionInLine() + end->getText().size();
    } else if (start->getLine() > end->getLine()) {
        line_e = start->getLine();
        line_s = end->getLine();
        char_e = start->getCharPositionInLine() + start->getText().size();
        char_s = end->getCharPositionInLine();
    } else {
        line_s = line_e = start->getLine();
        if (start->getCharPositionInLine() <= end->getCharPositionInLine()) {
        char_s = start->getCharPositionInLine();
        char_e = end->getCharPositionInLine() + end->getText().size();
        } else {
        char_e = start->getCharPositionInLine() + start->getText().size();
        char_s = end->getCharPositionInLine();
        }
    } 

    return std::make_unique<ieml::AST::CharRange>(file_id_, 
                                                  line_s + line_offset_, 
                                                  line_e + line_offset_, 
                                                  char_s + char_offset_, 
                                                  char_e + char_offset_);
}