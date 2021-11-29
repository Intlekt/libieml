#pragma once

#include "structure/Constants.h"
#include "structure/Word.h"
#include "utils.h"

#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <functional>
#include <set>


namespace ieml::structure {

class PathNode {
public:
    virtual ~PathNode() = default;

    virtual bool accept_next(const PathNode& next) const = 0;

    virtual PathType getPathType() const = 0;

    bool operator==(const PathNode& a) const noexcept;
    bool operator!=(const PathNode& a) const noexcept;
    // virtual bool operator<(const PathNode& a) const noexcept = 0;

    virtual std::string to_string() const = 0;
};

class RootPathNode : public PathNode {
public:
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

};

class RoleNumberPathNode : public PathNode {
public:
    RoleNumberPathNode(RoleType role_type) : role_type_(role_type) {};
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

    RoleType getRoleType() const {return role_type_;}
private:
    const RoleType role_type_;
};

class JunctionPathNode : public PathNode {
public:
    JunctionPathNode(JunctionType junction_type) : junction_type_(junction_type) {}
    virtual std::string to_string() const override;

private:
    const JunctionType junction_type_;
};

class AuxiliaryJunctionPathNode : public JunctionPathNode {
public:
    AuxiliaryJunctionPathNode(JunctionType junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class InflexingJunctionPathNode : public JunctionPathNode {
public:
    InflexingJunctionPathNode(JunctionType junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class CategoryJunctionPathNode : public JunctionPathNode {
public:
    CategoryJunctionPathNode(JunctionType junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class JunctionIndexPathNode : public PathNode {
public:
    JunctionIndexPathNode(int index) : index_(index) {}
    virtual std::string to_string() const override;

private:
    const int index_;
};

class AuxiliaryJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    AuxiliaryJunctionIndexPathNode(int index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class InflexingJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    InflexingJunctionIndexPathNode(int index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class CategoryJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    CategoryJunctionIndexPathNode(int index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class AuxiliaryPathNode : public PathNode {
public:
    AuxiliaryPathNode(std::shared_ptr<AuxiliaryWord> auxiliary_type) : auxiliary_type_(auxiliary_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

private:
    const std::shared_ptr<AuxiliaryWord> auxiliary_type_;
};

class InflexingPathNode : public PathNode {
public:
    InflexingPathNode(const std::set<std::shared_ptr<InflexingWord>>& inflexings) : inflexings_(inflexings) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

private:
    const std::set<std::shared_ptr<InflexingWord>> inflexings_;
};

class WordPathNode : public PathNode {
public:
    WordPathNode(std::shared_ptr<Word> word) : word_(word) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

private:
    const std::shared_ptr<Word> word_;
};

class Path {
/**
 * @brief Path nodes in sequence
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
    Path(std::shared_ptr<PathNode> node, std::shared_ptr<Path> next): node_(std::move(node)), next_(std::move(next)) {}
    void check();
    std::string to_string() const;
    std::shared_ptr<PathNode> getNode() const {return node_;}
    std::shared_ptr<Path> getNext() const {return next_;}

private:
    const std::shared_ptr<PathNode> node_;
    const std::shared_ptr<Path> next_;
};

class PathTree {
public:
    PathTree(std::shared_ptr<PathNode> node, std::set<std::shared_ptr<PathTree>> children) : 
        node_(std::move(node)), children_(std::move(children)) {}
    PathTree(std::shared_ptr<PathNode> node) : node_(std::move(node)), children_() {}

    static std::shared_ptr<PathTree> buildFromPaths(std::vector<std::shared_ptr<Path>> paths);
    std::shared_ptr<PathNode> getNode() const {return node_;}
    std::vector<std::shared_ptr<PathTree>> getChildren() const;
    
    std::string to_string() const;

    bool operator==(const PathTree& rhs) const {
        return to_string() == rhs.to_string();
    }

private:
    const std::shared_ptr<PathNode> node_;
    const std::set<std::shared_ptr<PathTree>> children_;
};
}

namespace std {
template<>
struct hash<ieml::structure::PathNode*>
{
    size_t operator()( ieml::structure::PathNode* const & s) const noexcept
    {
        return hash<string>{}(s->to_string());
    }
};
}

