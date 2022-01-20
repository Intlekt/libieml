#pragma once

#include "structure/Element.h"
#include "structure/path/PathNode.h"
#include "structure/path/Path.h"


namespace ieml::structure {

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