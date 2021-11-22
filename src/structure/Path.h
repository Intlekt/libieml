#pragma once

#include "Constants.h"
#include "Word.h"

#include <string>
#include <sstream>
#include <vector>



namespace ieml::structure {
enum class PathType {
    ROLE = 0,
    JUNCTION_AUXILIARY = 1,
    AUXILIARY = 2,
    JUNCTION_INFLEXING = 3,
    INFLEXING = 4,
    JUNCTION_CATEGORY = 5,
    WORD = 6
};


class PathNode {
public:
    virtual std::string to_string() const = 0;

    virtual bool accept_next(const PathNode& next) const = 0;

    virtual PathType getPathType() const = 0;

};

class RoleNumberPathNode : public PathNode {
public:
    RoleNumberPathNode(RoleType role_type) : role_type_(role_type) {}

    virtual std::string to_string() const {
        return std::to_string(role_type_);
    }

    virtual bool accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_AUXILIARY:
        case PathType::AUXILIARY:
        case PathType::JUNCTION_INFLEXING:
        case PathType::INFLEXING:
        case PathType::JUNCTION_CATEGORY:
        case PathType::WORD:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const {return PathType::ROLE;};

private:
    const RoleType role_type_;
};

class JunctionPathNode : public PathNode {
public:
    JunctionPathNode(JunctionType junction_type, int index) : 
        junction_type_(junction_type), index_(index) {}

    virtual std::string to_string() const {
        return "&" + std::string(junction_type_._to_string()) + "[" + std::string(index_) + "]";
    }

private:
    const JunctionType junction_type_;
    const int index_;
};
class AuxiliaryJunctionPathNode : public JunctionPathNode {
public:
    AuxiliaryJunctionPathNode(JunctionType junction_type, int index) : 
        JunctionPathNode(junction_type, index) {}

    virtual bool accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_AUXILIARY:
        case PathType::AUXILIARY:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const {return PathType::JUNCTION_AUXILIARY;};

};
class InflexingJunctionPathNode : public JunctionPathNode {
public:
    InflexingJunctionPathNode(JunctionType junction_type, int index) : 
        JunctionPathNode(junction_type, index) {}

    virtual bool accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_INFLEXING:
        case PathType::INFLEXING:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const {return PathType::JUNCTION_INFLEXING;};

};
class CategoryJunctionPathNode : public JunctionPathNode {
public:
    CategoryJunctionPathNode(JunctionType junction_type, int index) : 
        JunctionPathNode(junction_type, index) {}

    virtual bool accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_CATEGORY:
        case PathType::WORD:
        case PathType::ROLE:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const {return PathType::JUNCTION_CATEGORY;};
};

class AuxiliaryPathNode : public PathNode {
public:
    AuxiliaryPathNode(AuxiliaryType auxiliary_type) : auxiliary_type_(auxiliary_type) {}

    virtual std::string to_string() const {
        return "*" + std::to_string(auxiliary_type_);
    }

    virtual bool accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_INFLEXING:
        case PathType::INFLEXING:
        case PathType::JUNCTION_CATEGORY:
        case PathType::WORD:
        case PathType::ROLE:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const {return PathType::AUXILIARY;};

private:
    const AuxiliaryType auxiliary_type_;
};

class InflexingPathNode : public PathNode {
public:
    InflexingPathNode(std::vector<InflexingType> inflexings) : inflexings_(inflexings) {}

    virtual std::string to_string() const {
        std::ostringstream os;
        os << "~";
        bool first = true;
        for (auto& inflexing: inflexings_) {
            if (first) first = false;
            else os << ",";
            os << inflexing._to_string();
        }

        return os.str();
    }

    virtual bool accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_CATEGORY:
        case PathType::WORD:
        case PathType::ROLE:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const {return PathType::INFLEXING;};

private:
    const std::vector<InflexingType> inflexings_;
};

class WordPathNode : public PathNode {
public:
    WordPathNode(Word word) : word_(word) {}

    virtual std::string to_string() const {
        return "";// word_.to_string();
    }

    virtual bool accept_next(const PathNode& next) const {
        return false;
    };

    virtual PathType getPathType() const {return PathType::WORD;};

private:
    const Word word_;
};


class Path {
/**
 * @brief Path in a phrase
 * 
 * /{role number} /{junction[]}? /{auxiliary}? /{junction[]}? /{inflexing[]}? /{junction[]}? /{category} ...
 * 
 * 
 * 1) Checks if nodes are valid vector of PathNode ?
 *      - valid transitions e.g. {role number} -> {junction[]}
 *      
 * 
 * 
 */

public:
    Path(std::vector<PathNode> nodes): nodes_(nodes) {}

    std::string to_string() const {
        std::ostringstream os;
        for (auto& n: nodes_) {
            os << "/" << n.to_string();
        }
        return os.str();
    }

private:
    const std::vector<PathNode> nodes_;
};





class PathTree {
public:



private:



};


}