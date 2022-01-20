#pragma once

#include "structure/Constants.h"

#include <memory>
#include <exception>



namespace ieml::structure {

class Word;
class PathTree;

class Element {
public:
    virtual ElementType getElementType() const = 0;

    virtual std::string to_string() const = 0;

    std::string uid() const {
        if (getElementType() == +ElementType::WORD) return "word_" + std::to_string(hash());
        else                                        return "category_" + std::to_string(hash());
    };
    virtual size_t hash() const = 0;

    bool operator==(const Element& o) const {return getElementType() == o.getElementType() && comp_element_(o) == 0;}
    bool operator!=(const Element& o) const {return getElementType() != o.getElementType() || comp_element_(o) != 0;}
    bool operator< (const Element& o) const {return getElementType() <  o.getElementType() || comp_element_(o) < 0;}
    bool operator> (const Element& o) const {return getElementType() >  o.getElementType() || comp_element_(o) > 0;}
    bool operator<=(const Element& o) const {return getElementType() <= o.getElementType() && comp_element_(o) <= 0;}
    bool operator>=(const Element& o) const {return getElementType() >= o.getElementType() && comp_element_(o) >= 0;}

protected:
    virtual int comp_element_(const Element&) const = 0;
};

}
