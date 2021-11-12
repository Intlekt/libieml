#pragma once

#include <map>
#include <string>

#include "Constants.h"



namespace ieml {
namespace AST {

class ITranslatable {
public:
    ITranslatable(const std::map<LanguageType, const std::vector<const std::string>>& translations) translations_(translations) {};

private:
    std::map<LanguageType, const std::vector<const std::string>> translations_;

};

}
}