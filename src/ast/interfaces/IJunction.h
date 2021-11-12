#pragma once

#include "ast/Constants.h"


namespace ieml {
namespace AST {

class IJunction {
public:
    IJunction(JunctionType junction_type) : junction_type_(junction_type) {};

private:
    JunctionType junction_type_;
};

}
}