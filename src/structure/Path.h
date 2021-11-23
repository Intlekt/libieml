#pragma once

#include "Constants.h"
#include "Word.h"

#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <stdexcept>



namespace ieml::structure {
enum class PathType {
    ROOT,
    ROLE,
    JUNCTION_AUXILIARY,
    AUXILIARY,
    JUNCTION_INFLEXING,
    INFLEXING,
    JUNCTION_CATEGORY,
    WORD
};


class PathNode {
public:
    PathNode(std::shared_ptr<PathNode> next) : next_(std::move(next)) {
        if (next_ && !accept_next(*next_)) {
            throw std::invalid_argument("Invalid path descendent for " + node_to_string() + " got " + next_->node_to_string());
        }
    }

    virtual ~PathNode() = default;

    std::string to_string() const {
        auto node_s = "/" + node_to_string();
        if (next_)
            node_s += next_->to_string();

        return node_s;
    }

    virtual bool accept_next(const PathNode& next) const = 0;

    virtual PathType getPathType() const = 0;

protected:
    virtual std::string node_to_string() const = 0;


private:
    std::shared_ptr<PathNode> next_;
};


class RootPathNode : public PathNode {
public:
    RootPathNode(std::shared_ptr<PathNode> next) : PathNode(std::move(next)) {}

    virtual bool accept_next(const PathNode& next) const override {
        switch (next.getPathType())
        {
        case PathType::ROLE:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const override {return PathType::ROOT;};
protected:
    virtual std::string node_to_string() const override {
        return "";
    }
};

class RoleNumberPathNode : public PathNode {
public:
    RoleNumberPathNode(RoleType role_type, std::shared_ptr<PathNode> next) : 
        role_type_(role_type), PathNode(std::move(next)) {}

    virtual bool accept_next(const PathNode& next) const override {
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

    virtual PathType getPathType() const override {return PathType::ROLE;};

protected:
    virtual std::string node_to_string() const override {
        return std::to_string(role_type_);
    }

private:
    const RoleType role_type_;
};

class JunctionPathNode : public PathNode {
public:
    JunctionPathNode(JunctionType junction_type, int index, std::shared_ptr<PathNode> next) : 
        junction_type_(junction_type), index_(index), PathNode(std::move(next)) {}

protected:
    virtual std::string node_to_string() const override {
        return "&" + std::string(junction_type_._to_string()) + "[" + std::to_string(index_) + "]";;
    }
private:
    const JunctionType junction_type_;
    const int index_;
};

class AuxiliaryJunctionPathNode : public JunctionPathNode {
public:
    AuxiliaryJunctionPathNode(JunctionType junction_type, int index, std::shared_ptr<PathNode> next) : 
        JunctionPathNode(junction_type, index, std::move(next)) {}

    virtual bool accept_next(const PathNode& next) const override {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_AUXILIARY:
        case PathType::AUXILIARY:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const override {return PathType::JUNCTION_AUXILIARY;};

};

class InflexingJunctionPathNode : public JunctionPathNode {
public:
    InflexingJunctionPathNode(JunctionType junction_type, int index, std::shared_ptr<PathNode> next) : 
        JunctionPathNode(junction_type, index, std::move(next)) {}

    virtual bool accept_next(const PathNode& next) const override {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_INFLEXING:
        case PathType::INFLEXING:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const override {return PathType::JUNCTION_INFLEXING;};
};

class CategoryJunctionPathNode : public JunctionPathNode {
public:
    CategoryJunctionPathNode(JunctionType junction_type, int index, std::shared_ptr<PathNode> next) : 
        JunctionPathNode(junction_type, index, std::move(next)) {}

    virtual bool accept_next(const PathNode& next) const override {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_CATEGORY:
        case PathType::WORD:
        case PathType::ROOT:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const override {return PathType::JUNCTION_CATEGORY;};
};

class AuxiliaryPathNode : public PathNode {
public:
    AuxiliaryPathNode(AuxiliaryType auxiliary_type, std::shared_ptr<PathNode> next) : 
        auxiliary_type_(auxiliary_type), PathNode(std::move(next)) {}

    virtual bool accept_next(const PathNode& next) const override {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_INFLEXING:
        case PathType::INFLEXING:
        case PathType::JUNCTION_CATEGORY:
        case PathType::WORD:
        case PathType::ROOT:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const override {return PathType::AUXILIARY;};

protected:
    virtual std::string node_to_string() const override {
        return "*" + std::string(auxiliary_type_._to_string());
    }

private:
    const AuxiliaryType auxiliary_type_;
};

class InflexingPathNode : public PathNode {
public:
    InflexingPathNode(const std::vector<InflexingType>& inflexings, std::shared_ptr<PathNode> next) : 
        inflexings_(inflexings), PathNode(std::move(next)) {}

    virtual bool accept_next(const PathNode& next) const override {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_CATEGORY:
        case PathType::WORD:
        case PathType::ROOT:
            return true;
        default:
            return false;
        }
    };

    virtual PathType getPathType() const override {return PathType::INFLEXING;};

protected:
    virtual std::string node_to_string() const override {
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

private:
    const std::vector<InflexingType> inflexings_;
};

class WordPathNode : public PathNode {
public:
    WordPathNode(Word word) : word_(word), PathNode(nullptr) {}

    virtual bool accept_next(const PathNode& next) const override {
        return false;
    };

    virtual PathType getPathType() const override {return PathType::WORD;};

protected:
    virtual std::string node_to_string() const override {
        return "'" + word_.to_string() + "'";
    }

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
    Path(std::vector<std::unique_ptr<PathNode>>&& nodes): nodes_(std::move(nodes)) {
        for(auto it = nodes_.begin(); it + 1 != nodes_.end(); ++it) {
            if (!(*it)->accept_next(**(it + 1)))
                throw std::invalid_argument("Invalid path descendent for " + (*it)->to_string() + " with " + (*(it + 1))->to_string());
        }
    }

    std::string to_string() const {
        std::ostringstream os;
        for (const auto& n: nodes_) {
            os << "/" << n->to_string();
        }
        return os.str();
    }

private:
    const std::vector<std::unique_ptr<PathNode>> nodes_;
};



class PathTree {
public:
    PathTree(std::vector<std::unique_ptr<Path>>&& paths) : paths_(std::move(paths)) {}




private:
    std::vector<std::unique_ptr<Path>> paths_;
};


}