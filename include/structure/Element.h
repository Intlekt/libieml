#pragma once

#include "structure/Constants.h"

#include <memory>
#include <exception>
#include <string>



namespace ieml::structure {

class Word;
class PathTree;

class Element {
public:
    virtual ~Element() = default;

    virtual ElementType getElementType() const = 0;

    virtual std::string to_string() const = 0;

    std::string uid() const {
        return prefix() + "_" + std::to_string(hash());
    };
    virtual std::string prefix() const = 0;
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
