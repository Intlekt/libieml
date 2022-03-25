#pragma once

#include <enum.h>


namespace ieml::structure {

/* A Reference type is the accepted type object for a given argument. It can be : 
    *      1) a dtype (only utf8 string at the moment) 
    *      2) a path tree            (~type)
    *      3) a path tree instance   (~object)
    */

BETTER_ENUM(ReferenceType, char, PATH_TREE, WORD)


class ReferenceValue {
public:
    ReferenceValue(ReferenceType type,
                   Element::Ptr element) :
        type_(type),
        element_(element) {}

    Word::Ptr getWord() const {
        return std::dynamic_pointer_cast<Word>(element_);
    }

    std::string hash() const {return element_->uid();}

private:
    const ReferenceType type_;
    const Element::Ptr element_;
};

}