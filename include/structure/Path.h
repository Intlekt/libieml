#pragma once

#include "structure/Constants.h"
#include "structure/Word.h"
#include "structure/WordRegister.h"
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

    bool operator==(const PathNode& a) const {return to_string() == a.to_string();};
    bool operator!=(const PathNode& a) const {return to_string() != a.to_string();};
    bool operator< (const PathNode& a) const {
        if (getPathType() == a.getPathType()) return comp(a) <  0;
        else                                  return getPathType() < a.getPathType();
    };
    bool operator> (const PathNode& a) const {
        if (getPathType() == a.getPathType()) return comp(a) >  0;
        else                                  return getPathType() > a.getPathType();
    };
    bool operator<=(const PathNode& a) const {
        if (getPathType() == a.getPathType()) return comp(a) <= 0;
        else                                  return getPathType() < a.getPathType();
    };
    bool operator>=(const PathNode& a) const {
        if (getPathType() == a.getPathType()) return comp(a) >=  0;
        else                                  return getPathType() > a.getPathType();
    };

    virtual std::string to_string() const = 0;
private:
    virtual int comp(const PathNode& a) const = 0;
};

class RootPathNode : public PathNode {
public:
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

private:
    virtual int comp(const PathNode& a) const {return 0;};
};

class RoleNumberPathNode : public PathNode {
public:
    RoleNumberPathNode(RoleType role_type) : role_type_(role_type) {};
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

    RoleType getRoleType() const {return role_type_;}
private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const RoleNumberPathNode&>(a);
        if (role_type_ == b.role_type_) return  0;
        if (role_type_ <  b.role_type_) return -1;
        else                            return  1;
    };

    const RoleType role_type_;
};

class JunctionPathNode : public PathNode {
public:
    JunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : junction_type_(junction_type) {}
    virtual std::string to_string() const override;
    
    std::shared_ptr<JunctionWord> getJunctionType() const {return junction_type_;};

    // bool operator< (const JunctionPathNode& a) const noexcept {return *junction_type_ <  *a.junction_type_;};
    // bool operator> (const JunctionPathNode& a) const noexcept {return *junction_type_ >  *a.junction_type_;};
    // bool operator<=(const JunctionPathNode& a) const noexcept {return *junction_type_ <= *a.junction_type_;};
    // bool operator>=(const JunctionPathNode& a) const noexcept {return *junction_type_ >= *a.junction_type_;};
private:
    const std::shared_ptr<JunctionWord> junction_type_;
};

class PhraseJunctionPathNode : public JunctionPathNode {
public:
    PhraseJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const PhraseJunctionPathNode&>(a);

        if (*getJunctionType() == *b.getJunctionType()) return  0;
        if (*getJunctionType() <  *b.getJunctionType()) return -1;
        else                                            return  1;
    };
};

class AuxiliaryJunctionPathNode : public JunctionPathNode {
public:
    AuxiliaryJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const AuxiliaryJunctionPathNode&>(a);

        if (*getJunctionType() == *b.getJunctionType()) return  0;
        if (*getJunctionType() <  *b.getJunctionType()) return -1;
        else                                            return  1;
    };
};

class InflectionJunctionPathNode : public JunctionPathNode {
public:
    InflectionJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const InflectionJunctionPathNode&>(a);

        if (*getJunctionType() == *b.getJunctionType()) return  0;
        if (*getJunctionType() <  *b.getJunctionType()) return -1;
        else                                            return  1;
    };
};

class CategoryJunctionPathNode : public JunctionPathNode {
public:
    CategoryJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const CategoryJunctionPathNode&>(a);

        if (*getJunctionType() == *b.getJunctionType()) return  0;
        if (*getJunctionType() <  *b.getJunctionType()) return -1;
        else                                            return  1;
    };
};

class JunctionIndexPathNode : public PathNode {
public:
    JunctionIndexPathNode(int index) : index_(index) {}
    virtual std::string to_string() const override;

    int getIndex() const {return index_;};
    // bool operator< (const JunctionIndexPathNode& a) const noexcept {return index_ <  a.index_;};
    // bool operator> (const JunctionIndexPathNode& a) const noexcept {return index_ >  a.index_;};
    // bool operator<=(const JunctionIndexPathNode& a) const noexcept {return index_ <= a.index_;};
    // bool operator>=(const JunctionIndexPathNode& a) const noexcept {return index_ >= a.index_;};
private:
    const int index_;
};

class PhraseJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    PhraseJunctionIndexPathNode(int index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const PhraseJunctionIndexPathNode&>(a);

        if (getIndex() == b.getIndex()) return  0;
        if (getIndex() <  b.getIndex()) return -1;
        else                    return  1;
    };

};

class AuxiliaryJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    AuxiliaryJunctionIndexPathNode(int index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;

private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const AuxiliaryJunctionIndexPathNode&>(a);

        if (getIndex() == b.getIndex()) return  0;
        if (getIndex() <  b.getIndex()) return -1;
        else                    return  1;
    };
};

class InflectionJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    InflectionJunctionIndexPathNode(int index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const InflectionJunctionIndexPathNode&>(a);

        if (getIndex() == b.getIndex()) return  0;
        if (getIndex() <  b.getIndex()) return -1;
        else                    return  1;
    };
};

class CategoryJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    CategoryJunctionIndexPathNode(int index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;

private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const CategoryJunctionIndexPathNode&>(a);

        if (getIndex() == b.getIndex()) return  0;
        if (getIndex() <  b.getIndex()) return -1;
        else                    return  1;
    };
};

class AuxiliaryPathNode : public PathNode {
public:
    AuxiliaryPathNode(std::shared_ptr<AuxiliaryWord> auxiliary_type) : auxiliary_type_(auxiliary_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

    // bool operator< (const AuxiliaryPathNode& a) const noexcept {return *auxiliary_type_ <  *a.auxiliary_type_;};
    // bool operator> (const AuxiliaryPathNode& a) const noexcept {return *auxiliary_type_ >  *a.auxiliary_type_;};
    // bool operator<=(const AuxiliaryPathNode& a) const noexcept {return *auxiliary_type_ <= *a.auxiliary_type_;};
    // bool operator>=(const AuxiliaryPathNode& a) const noexcept {return *auxiliary_type_ >= *a.auxiliary_type_;};

private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const AuxiliaryPathNode&>(a);

        if (*auxiliary_type_ == *b.auxiliary_type_) return  0;
        if (*auxiliary_type_ <  *b.auxiliary_type_) return -1;
        else                                        return  1;
    };

    const std::shared_ptr<AuxiliaryWord> auxiliary_type_;
};

class InflectionPathNode : public PathNode {
public:
    InflectionPathNode(const std::set<std::shared_ptr<InflectionWord>>& inflections) : inflections_(inflections) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;



    // bool operator< (const InflectionPathNode& a) const noexcept {return cmp(a) <  0;};
    // bool operator> (const InflectionPathNode& a) const noexcept {return cmp(a) >  0;};
    // bool operator<=(const InflectionPathNode& a) const noexcept {return cmp(a) <= 0;};
    // bool operator>=(const InflectionPathNode& a) const noexcept {return cmp(a) >= 0;};

private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const InflectionPathNode&>(a);

        if (inflections_.size() != b.inflections_.size()) return (inflections_.size() < b.inflections_.size() ? -1 : 1);

        auto it_b = b.inflections_.begin();
        for (auto it_self = inflections_.begin(); it_self != inflections_.end(); it_self++) {
            if (**it_self != **it_b) return (**it_self < **it_b ? -1 : 1);
            it_b++;
        }
        // they are equal
        return 0;
    }

    const std::set<std::shared_ptr<InflectionWord>> inflections_;
};

/*
 * WordPathNode
 *
 * The terminal path to a category Word
 */
class WordPathNode : public PathNode {
public:
    WordPathNode(std::shared_ptr<CategoryWord> word) : word_(word) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

    // bool operator< (const WordPathNode& a) const noexcept {return *word_ <  *a.word_;};
    // bool operator> (const WordPathNode& a) const noexcept {return *word_ >  *a.word_;};
    // bool operator<=(const WordPathNode& a) const noexcept {return *word_ <= *a.word_;};
    // bool operator>=(const WordPathNode& a) const noexcept {return *word_ >= *a.word_;};

private:
    virtual int comp(const PathNode& a) const {
        auto b = dynamic_cast<const WordPathNode&>(a);

        if (*word_ == *b.word_) return  0;
        if (*word_ <  *b.word_) return -1;
        else                    return  1;
    };

    const std::shared_ptr<CategoryWord> word_;
};

class Path {
/**
 * @brief Path nodes in sequence
 * 
 * /{role number} /{junction[]}? /{auxiliary}? /{junction[]}? /{inflection[]}? /{junction[]}? /{category} ...
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
    Path(std::shared_ptr<PathNode> node): node_(std::move(node)), next_(nullptr) {}
    void check();
    std::string to_string() const;
    std::shared_ptr<PathNode> getNode() const {return node_;}
    std::shared_ptr<Path> getNext() const {return next_;}

    static std::shared_ptr<Path> from_string(const std::string& s, const WordRegister& ctx);
    
    bool operator==(const Path& r) const {
        return compare(r) == 0;
    };
    bool operator!=(const Path& r) const {return compare(r) != 0;};

    bool operator< (const Path& r) const {return compare(r) <  0;};
    bool operator> (const Path& r) const {return compare(r) >  0;};
    bool operator<=(const Path& r) const {return compare(r) <= 0;};
    bool operator>=(const Path& r) const {return compare(r) >= 0;};

private:
    const std::shared_ptr<PathNode> node_;
    const std::shared_ptr<Path> next_;

    int compare(const Path& r) const {
        if (*node_ == *r.node_) {
            if (next_ == nullptr && r.next_ == nullptr) return 0;
            if (next_ == nullptr) return -1;
            if (r.next_ == nullptr) return 1;
            return next_->compare(*r.next_);
        } else return *node_ <  *r.node_ ? -1 : 1;
    }
};

class PathTree {
public:
    std::shared_ptr<PathNode> getNode() const {return node_;}
    std::vector<std::shared_ptr<PathTree>> getChildren() const;
    
    std::string to_string() const;

    bool is_phrase() const {return node_->getPathType() == +PathType::ROOT;};

    bool operator==(const PathTree& rhs) const {return comp(node_, children_, rhs.node_, rhs.children_) == 0;};
    bool operator!=(const PathTree& rhs) const {return comp(node_, children_, rhs.node_, rhs.children_) != 0;};

    bool operator< (const PathTree& rhs) const {return comp(node_, children_, rhs.node_, rhs.children_) <  0;};
    bool operator> (const PathTree& rhs) const {return comp(node_, children_, rhs.node_, rhs.children_) >  0;};
    bool operator<=(const PathTree& rhs) const {return comp(node_, children_, rhs.node_, rhs.children_) <= 0;};
    bool operator>=(const PathTree& rhs) const {return comp(node_, children_, rhs.node_, rhs.children_) >= 0;};

    struct CompareFunctor {
        bool operator()(const std::shared_ptr<PathTree>& l, const std::shared_ptr<PathTree>& r) const; 
    };
    typedef std::set<std::shared_ptr<PathTree>, CompareFunctor> Children;

    class Register {
    public:
        typedef std::pair<std::shared_ptr<PathNode>, PathTree::Children> Key;

        std::shared_ptr<PathTree> get_or_create(const std::shared_ptr<PathNode>& node, const Children& children) {
            auto key = Key(node, children);
            auto it = store_.find(key);
            if (it != store_.end()) return it->second;

            auto item = std::shared_ptr<PathTree>(new PathTree(node, children));
            store_.insert({key, item});
            return item;
        }
        std::shared_ptr<PathTree> get_or_create(const std::shared_ptr<PathNode>& node) {
            return get_or_create(node, Children{});
        }

        std::shared_ptr<PathTree> buildFromPaths(std::vector<std::shared_ptr<Path>> paths);

    private:
        struct cmpKey {
            bool operator()(const Key& a, const Key& b) const {
                return comp(a.first, a.second, b.first, b.second) < 0;
            }
        };

        std::map<Key, std::shared_ptr<PathTree>, cmpKey> store_;
    };

    typedef std::pair<std::shared_ptr<Path>, std::shared_ptr<PathTree>> SubPathTree;

    /*
     * Return all matching SubPathTree
     *
     * parameters:
     *  - f : the matching function
     *  - sub_occurence : if we return the match of children of matched tree
     */
    std::vector<SubPathTree> find_sub_tree(std::function<bool(const std::shared_ptr<PathTree>&)> f, bool sub_occurence) const {
        std::vector<SubPathTree> res;
        for (auto& child: children_) {
            bool matched = f(child);
            if (matched) res.push_back(SubPathTree{std::make_shared<Path>(node_), child});
            
            if (!matched || sub_occurence) {
                auto child_res = child->find_sub_tree(f, sub_occurence);
                res.reserve(res.size() + child_res.size());

                for (auto& item: child_res) {
                    res.push_back(SubPathTree{std::make_shared<Path>(node_, item.first), item.second});
                }
            }
        }
        return res;
    };


private:
    PathTree(const std::shared_ptr<PathNode>& node, const Children& children) : 
        node_(node), children_(children) {}

    PathTree(const std::shared_ptr<PathNode>& node) : 
        node_(node), children_() {}

    PathTree(const PathTree&) = delete;
    PathTree& operator=(const PathTree&) = delete;


    const std::shared_ptr<PathNode> node_;
    const Children children_;


    static int comp(const std::shared_ptr<PathNode>& nodeA, const Children& childrenA, 
                    const std::shared_ptr<PathNode>& nodeB, const Children& childrenB) {
        if (*nodeA == *nodeB) {
            auto it = childrenA.begin();
            auto it_r = childrenB.begin();

            for (;it != childrenA.end() && it_r != childrenB.end();) {
                if (*it != *it_r) return *it < *it_r ? -1 : 1;

                ++it;
                ++it_r;
            }

            if (it == childrenA.end() && it_r == childrenB.end()) return 0;

            return it == childrenA.end() ? -1 : 1;
        } else 
            return *nodeA < *nodeB ? -1 : 1;
    }
};

}


namespace std {
template<>
struct hash<ieml::structure::PathNode>
{
    size_t operator()(const ieml::structure::PathNode& s) const noexcept
    {
        return hash<string>{}(s.to_string());
    }
};

template<>
struct hash<ieml::structure::PathTree>
{
    size_t operator()(const ieml::structure::PathTree& s) const noexcept
    {
        size_t seed = 0;
        hash_combine(seed, s.getNode());

        for (auto& c: s.getChildren())
            hash_combine(seed, *c);

        return seed;
    }
};

}

