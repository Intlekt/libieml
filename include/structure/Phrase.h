#pragma once

#include "structure/Path.h"

namespace ieml::structure {

class Phrase : public PathTree {



};
}

namespace std {
template<>
struct hash<ieml::structure::Phrase>
{
    size_t operator()(const ieml::structure::Phrase& s) const noexcept
    {
        return 0;
    }
};
}