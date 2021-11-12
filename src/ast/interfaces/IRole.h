#pragma once

#include "Constants.h"



namespace ieml {
namespace AST {

class IRole {
public:
    IRole(RoleType role_type) : role_type_(role_type) {};

private:
    RoleType role_type_;

};

}
}