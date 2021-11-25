#pragma once

#include <memory>

#include "ast/Constants.h"
#include "ast/Identifier.h"
#include "ast/interfaces/AST.h"


namespace ieml {
namespace AST {

template<class T >
class IJunction {
public:
    IJunction(std::vector<std::unique_ptr<T>>&& items,
              std::unique_ptr<Identifier>&& junction_identifier) : 
        items_(std::move(items)),
        junction_identifier_(std::move(junction_identifier)) {
            static_assert(std::is_base_of<AST, T>::value, "T is not an AST.");
        };

protected:
    std::string junction_to_string() const {
        std::ostringstream os;

        os << "&" << junction_identifier_->getName() << " [";

        bool first = true;
        for (auto&& item : items_) {
            if (first) first = false;
            else os << " ";

            os << item->to_string();
        }

        os << "]";
        return os.str();
    }

private:
    std::unique_ptr<Identifier> junction_identifier_;
    std::vector<std::unique_ptr<T>> items_;
};

}
}