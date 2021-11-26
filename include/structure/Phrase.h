#pragma once

#include "structure/Path.h"

namespace ieml::structure {

class Phrase : public PathTree {
public:
    Phrase(std::vector<std::shared_ptr<PathTree>> children) : 
        PathTree(std::make_shared<RootPathNode>(), children) {}
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