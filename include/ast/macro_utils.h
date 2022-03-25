#pragma once

#include <memory>

#define IEML_DECLARE_PTR_TYPE_AST(TYPE) typedef std::shared_ptr<TYPE> Ptr;
#define IEML_DECLARE_PTR_TYPE_STRUCTURE(TYPE) typedef std::shared_ptr<TYPE> Ptr;
