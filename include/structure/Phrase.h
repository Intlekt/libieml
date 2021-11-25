#pragma once


namespace ieml::structure {

class Phrase {
public:

    bool operator==(const Phrase& rhs) const {
        return true;
    }


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