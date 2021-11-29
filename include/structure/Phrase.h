#pragma once

#include "structure/Path.h"

namespace ieml::structure {

class Phrase : public PathTree {
public:
    Phrase(std::set<std::shared_ptr<PathTree>> children) : 
        PathTree(std::make_shared<RootPathNode>(), children) {}
};
}

namespace std {
template<>
struct hash<ieml::structure::Phrase>
{
    size_t operator()(const ieml::structure::Phrase& s) const noexcept
    {
        return hash<std::string>{}(s.to_string());
    }
};

STD_HASH_SHARED_PTR(ieml::structure::Phrase);
}