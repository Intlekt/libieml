#pragma once

#include <memory>

#include "SyntaxError.h"
#include "ast/CharRange.h"
#include "antlr4-runtime.h"


namespace ieml::parser {

class AbstractVisitor {
public:
  template<class T>
  class VisitorResult {
  public:
    VisitorResult(): is_error_(true) {}
    VisitorResult(T&& value): is_error_(false), value_(std::move(value)) {}

    VisitorResult(const VisitorResult& vr) = delete;
    VisitorResult(VisitorResult&& vr) noexcept : is_error_(vr.is_error_), value_(std::move(vr.value_)) {};
    VisitorResult& operator= (const VisitorResult&) = delete;
    VisitorResult& operator= (VisitorResult&&) = default;

    bool isError() const {return is_error_;};

    T&& release() { return std::move(value_); }

    const T& value() const {return value_;}

  private:
    bool is_error_;
    T value_;
  };

  AbstractVisitor(const std::string& file_id, IEMLParserErrorListener* error_listener) 
      : file_id_(file_id), error_listener_(error_listener) {}


protected:
  std::shared_ptr<ieml::AST::CharRange> charRangeFromToken(antlr4::Token* token) const ;
  std::shared_ptr<ieml::AST::CharRange> charRangeFromContext(antlr4::ParserRuleContext* ctx) const ;

  std::string file_id_;
  size_t line_offset_ = 0;
  size_t char_offset_ = 0;
  IEMLParserErrorListener* error_listener_;
};
}