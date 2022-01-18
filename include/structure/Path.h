#pragma once

#include "structure/Constants.h"
#include "structure/Word.h"
#include "structure/WordRegister.h"
#include "utils.h"
#include "structure/Element.h"

#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <functional>
#include <set>
#include <stdexcept>


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
    virtual const std::set<std::shared_ptr<Word>> getWords() const {return {};};

    virtual size_t getIndex() const {throw std::invalid_argument("No index defined for " +std::string(getPathType()._to_string())+ " path node.");};
    virtual RoleType getRoleType() const {throw std::invalid_argument("No RoleType defined for " +std::string(getPathType()._to_string())+ " path node.");};
    virtual std::shared_ptr<JunctionWord> getJunctionType() const {throw std::invalid_argument("No JunctionWord defined for " +std::string(getPathType()._to_string())+ " path node.");};

    virtual std::shared_ptr<AuxiliaryWord> getAuxialiryType() const {throw std::invalid_argument("No AuxiliaryWord defined for " +std::string(getPathType()._to_string())+ " path node.");};
    virtual const std::set<std::shared_ptr<InflectionWord>>& getInflections() const {throw std::invalid_argument("No InflectionWords defined for " +std::string(getPathType()._to_string())+ " path node.");}; 
    virtual const std::shared_ptr<CategoryWord> getCategoryWord() const {throw std::invalid_argument("No CategoryWord defined for " +std::string(getPathType()._to_string())+ " path node.");};


    typedef std::set<std::shared_ptr<PathNode>> Set;

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

class ParadigmPathNode : public PathNode {
public:
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

private:
    virtual int comp(const PathNode& a) const {return 0;};
};

class ParadigmIndexPathNode : public PathNode {
public:
    ParadigmIndexPathNode(size_t index) : index_(index) {};
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

    virtual size_t getIndex() const override {return index_;};

private:
    virtual int comp(const PathNode& a) const {
        if (getIndex() == a.getIndex()) return  0;
        if (getIndex() <  a.getIndex()) return -1;
        else                            return  1;
    };

    const size_t index_;
};



class RoleNumberPathNode : public PathNode {
public:
    RoleNumberPathNode(RoleType role_type) : role_type_(role_type) {};
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

    virtual RoleType getRoleType() const override {return role_type_;};
private:
    virtual int comp(const PathNode& a) const {
        if (getRoleType() == a.getRoleType()) return  0;
        if (getRoleType() <  a.getRoleType()) return -1;
        else                            return  1;
    };

    const RoleType role_type_;
};


class JunctionPathNode : public PathNode {
public:
    JunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : junction_type_(junction_type) {}
    virtual std::string to_string() const override;
    
    virtual std::shared_ptr<JunctionWord> getJunctionType() const override {return junction_type_;};
    virtual const std::set<std::shared_ptr<Word>> getWords() const override {return {junction_type_};};

private:
    virtual int comp(const PathNode& a) const {
        if (*getJunctionType() == *a.getJunctionType()) return  0;
        if (*getJunctionType() <  *a.getJunctionType()) return -1;
        else                                            return  1;
    };

    const std::shared_ptr<JunctionWord> junction_type_;
};

class PhraseJunctionPathNode : public JunctionPathNode {
public:
    PhraseJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class AuxiliaryJunctionPathNode : public JunctionPathNode {
public:
    AuxiliaryJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class InflectionJunctionPathNode : public JunctionPathNode {
public:
    InflectionJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class CategoryJunctionPathNode : public JunctionPathNode {
public:
    CategoryJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class JunctionIndexPathNode : public PathNode {
public:
    JunctionIndexPathNode(size_t index) : index_(index) {}
    virtual std::string to_string() const override;

    virtual size_t getIndex() const override {return index_;};
private:
    virtual int comp(const PathNode& a) const override {
        if (getIndex() == a.getIndex()) return  0;
        if (getIndex() <  a.getIndex()) return -1;
        else                    return  1;
    };

    const size_t index_;
};

class PhraseJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    PhraseJunctionIndexPathNode(size_t index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class AuxiliaryJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    AuxiliaryJunctionIndexPathNode(size_t index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class InflectionJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    InflectionJunctionIndexPathNode(size_t index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class CategoryJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    CategoryJunctionIndexPathNode(size_t index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class AuxiliaryPathNode : public PathNode {
public:
    AuxiliaryPathNode(std::shared_ptr<AuxiliaryWord> auxiliary_type) : auxiliary_type_(auxiliary_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;
    virtual const std::set<std::shared_ptr<Word>> getWords() const override {return {auxiliary_type_};};

    virtual std::shared_ptr<AuxiliaryWord> getAuxialiryType() const override {return auxiliary_type_;};
private:
    virtual int comp(const PathNode& a) const {
        if (*getAuxialiryType() == *a.getAuxialiryType()) return  0;
        if (*getAuxialiryType() <  *a.getAuxialiryType()) return -1;
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

    virtual const std::set<std::shared_ptr<Word>> getWords() const override {
        return std::set<std::shared_ptr<Word>>(inflections_.begin(), inflections_.end());
    };

    virtual const std::set<std::shared_ptr<InflectionWord>>& getInflections() const override {return inflections_;}; 

private:
    virtual int comp(const PathNode& a) const {
        if (getInflections().size() != a.getInflections().size()) return (getInflections().size() < a.getInflections().size() ? -1 : 1);

        auto it_a = a.getInflections().begin();
        for (auto it_self = getInflections().begin(); it_self != getInflections().end(); it_self++) {
            if (**it_self != **it_a) return (**it_self < **it_a ? -1 : 1);
            it_a++;
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

    virtual const std::set<std::shared_ptr<Word>> getWords() const override {
        return {word_};
    };

    virtual const std::shared_ptr<CategoryWord> getCategoryWord() const override {return word_;};
private:
    virtual int comp(const PathNode& a) const {
        if (*getCategoryWord() == *a.getCategoryWord()) return  0;
        if (*getCategoryWord() <  *a.getCategoryWord()) return -1;
        else                                            return  1;
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

class PathTree : public Element {
public:
    virtual ElementType getElementType() const override {return ElementType::PATH_TREE;};

    std::string to_string() const;

    bool is_phrase() const {return node_->getPathType() == +PathType::ROOT;};

    bool is_word() const {return node_->getPathType() == +PathType::WORD;};
    bool is_inflection() const {return node_->getPathType() == +PathType::INFLECTION;};
    bool is_auxiliary() const {return node_->getPathType() == +PathType::AUXILIARY;};
    bool is_junction() const {return node_->getPathType() == +PathType::JUNCTION_AUXILIARY || 
                                     node_->getPathType() == +PathType::JUNCTION_PHRASE || 
                                     node_->getPathType() == +PathType::JUNCTION_CATEGORY || 
                                     node_->getPathType() == +PathType::JUNCTION_INFLECTION;};
    bool is_paradigm() const {return node_->getPathType() == +PathType::PARADIGM;}

    struct CompareFunctor {
        bool operator()(const std::shared_ptr<PathTree>& l, const std::shared_ptr<PathTree>& r) const {
            return comp(l->node_, l->children_, r->node_, r->children_) < 0;
        } 
    };
    typedef std::set<std::shared_ptr<PathTree>, CompareFunctor> Set;

    class Register {
    public:
        typedef std::pair<std::shared_ptr<PathNode>, PathTree::Set> Key;

        Set get_or_create_product(const PathNode::Set& node_set, const std::vector<Set>& children_list);
        Set get_or_create_product(const std::shared_ptr<PathNode>& node, const std::vector<Set>& children_list);
        std::shared_ptr<PathTree> get_or_create(const std::shared_ptr<PathNode>& node, const Set& children);
        std::shared_ptr<PathTree> get_or_create(const std::shared_ptr<PathNode>& node) ;

        std::shared_ptr<PathTree> buildFromPaths(std::vector<std::shared_ptr<Path>> paths);

    private:
        struct eqKey {
            bool operator()(const Key& a, const Key& b) const {
                return comp(a.first, a.second, b.first, b.second) == 0;
            }
        };

        struct hashKey {
            size_t operator()(const Key& a) const {
                return PathTree::hash_internal(a.first, a.second);
            }
        };

        std::unordered_map<Key, std::shared_ptr<PathTree>, hashKey, eqKey> store_;
    };

    typedef std::pair<std::shared_ptr<Path>, std::shared_ptr<PathTree>> SubPathTree;

    /*
     * Return all matching SubPathTree
     *
     * parameters:
     *  - f : the matching function
     *  - sub_occurence : if we return the match of children of matched tree
     */
    std::vector<SubPathTree> find_sub_tree(std::function<bool(const std::shared_ptr<PathTree>&)> f,
                                           std::function<bool(const std::shared_ptr<PathTree>&)> should_stop) const;

    static Set singular_sequences(const std::shared_ptr<PathTree>& pt);

    std::shared_ptr<PathNode> getNode() const {return node_;}
    std::vector<std::shared_ptr<PathTree>> getChildrenAsVector() const;
    const Set getChildren() const {return children_;};

    virtual size_t hash() const override {return hash_;};

private:
    PathTree(const std::shared_ptr<PathNode>& node, const Set& children) : 
        node_(node), children_(children), hash_(hash_internal(node, children)) {}

    PathTree(const std::shared_ptr<PathNode>& node) : 
        node_(node), children_(), hash_(hash_internal(node, {})) {}

    PathTree(const PathTree&) = delete;
    PathTree& operator=(const PathTree&) = delete;


    static size_t hash_internal(const std::shared_ptr<PathNode>& node, const Set& children) {
        size_t seed = 0;
        hash_combine(seed, *node);

        for (auto& c: children)
            hash_combine(seed, c->hash());

        return seed;
    }

    const size_t hash_;
    const std::shared_ptr<PathNode> node_;
    const Set children_;


    static int comp(const std::shared_ptr<PathNode>&, const Set&, 
                    const std::shared_ptr<PathNode>&, const Set&);

    virtual int comp_element_(const Element&) const override;
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

}

