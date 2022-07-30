#include "structure/path/PathNode.h"

#include <sstream>

using namespace ieml::structure;


bool RootPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::ROLE:
    case PathType::JUNCTION_PHRASE:
        return true;
    default:
        return false;
    }
}
PathType RootPathNode::getPathType() const {return PathType::ROOT;}
std::string RootPathNode::to_string() const { return "#"; }

bool ParadigmPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::PARADIGM_INDEX:
        return true;
    default:
        return false;
    }
}
PathType ParadigmPathNode::getPathType() const {return PathType::PARADIGM;}
std::string ParadigmPathNode::to_string() const { return "{}"; }

bool ParadigmIndexPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::ROOT:
        return true;
    default:
        return false;
    }
}
PathType ParadigmIndexPathNode::getPathType() const {return PathType::PARADIGM_INDEX;}
std::string ParadigmIndexPathNode::to_string() const { return std::to_string(index_); }


bool RoleNumberPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::JUNCTION_AUXILIARY:
    case PathType::AUXILIARY:
    case PathType::JUNCTION_INFLECTION:
    case PathType::INFLECTION:
    case PathType::JUNCTION_CATEGORY:
    case PathType::WORD:
    case PathType::ROOT:
        return true;
    default:
        return false;
    }
}
PathType RoleNumberPathNode::getPathType() const {return PathType::ROLE;}
std::string RoleNumberPathNode::to_string() const {return std::to_string(role_type_);}

std::string JunctionPathNode::to_string() const {
    return "&" + std::string(junction_type_->to_string());
}

bool PhraseJunctionPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::JUNCTION_PHRASE_INDEX:
        return true;
    default:
        return false;
    }
}
PathType PhraseJunctionPathNode::getPathType() const {return PathType::JUNCTION_PHRASE;}

bool AuxiliaryJunctionPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::JUNCTION_AUXILIARY_INDEX:
        return true;
    default:
        return false;
    }
}
PathType AuxiliaryJunctionPathNode::getPathType() const {return PathType::JUNCTION_AUXILIARY;}

bool InflectionJunctionPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::JUNCTION_INFLECTION_INDEX:
        return true;
    default:
        return false;
    }
}
PathType InflectionJunctionPathNode::getPathType() const {return PathType::JUNCTION_INFLECTION;}

bool CategoryJunctionPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::JUNCTION_CATEGORY_INDEX:
        return true;
    default:
        return false;
    }
}
PathType CategoryJunctionPathNode::getPathType() const {return PathType::JUNCTION_CATEGORY;}

std::string JunctionIndexPathNode::to_string() const {
    return "[" + std::to_string(index_) + "]";;
}
bool PhraseJunctionIndexPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::ROLE:
    case PathType::JUNCTION_PHRASE:
        return true;
    default:
        return false;
    }
}
PathType PhraseJunctionIndexPathNode::getPathType() const {return PathType::JUNCTION_PHRASE_INDEX;}

bool AuxiliaryJunctionIndexPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::JUNCTION_AUXILIARY:
    case PathType::AUXILIARY:
        return true;
    default:
        return false;
    }
}
PathType AuxiliaryJunctionIndexPathNode::getPathType() const {return PathType::JUNCTION_AUXILIARY_INDEX;}

bool InflectionJunctionIndexPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::JUNCTION_INFLECTION:
    case PathType::INFLECTION:
        return true;
    default:
        return false;
    }
}
PathType InflectionJunctionIndexPathNode::getPathType() const {return PathType::JUNCTION_INFLECTION_INDEX;}

bool CategoryJunctionIndexPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::JUNCTION_CATEGORY:
    case PathType::WORD:
    case PathType::ROOT:
        return true;
    default:
        return false;
    }
}
PathType CategoryJunctionIndexPathNode::getPathType() const {return PathType::JUNCTION_CATEGORY_INDEX;}

bool AuxiliaryPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::JUNCTION_INFLECTION:
    case PathType::INFLECTION:
    case PathType::JUNCTION_CATEGORY:
    case PathType::WORD:
    case PathType::ROOT:
        return true;
    default:
        return false;
    }
}
PathType AuxiliaryPathNode::getPathType() const {return PathType::AUXILIARY;}
std::string AuxiliaryPathNode::to_string() const {
    return "*" + auxiliary_type_->to_string();
}

bool InflectionPathNode::accept_next(const PathNode& next) const {
    switch (next.getPathType())
    {
    case PathType::JUNCTION_CATEGORY:
    case PathType::WORD:
    case PathType::ROOT:
        return true;
    default:
        return false;
    }
}
PathType InflectionPathNode::getPathType() const {return PathType::INFLECTION;}
std::string InflectionPathNode::to_string() const {
    std::ostringstream os;
    for (auto& inflection: inflections_)
        os << "~" << inflection->to_string();

    return os.str();
}

bool WordPathNode::accept_next(__attribute__((unused)) const PathNode& next) const {
    return false;
}
PathType WordPathNode::getPathType() const {return PathType::WORD;}
std::string WordPathNode::to_string() const {
    return word_->to_string();
}


size_t PathNode::HashFunctor::operator()(const ieml::structure::PathNode::Ptr& a) const {
    return std::hash<ieml::structure::PathNode>()(*a);
}
