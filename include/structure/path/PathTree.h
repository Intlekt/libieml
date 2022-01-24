#pragma once

#include "structure/Element.h"
#include "structure/path/PathNode.h"
// #include "structure/path/Path.h"


namespace ieml::structure {

class PathTree : public Element {
public:
    virtual ElementType getElementType() const override {return ElementType::PATH_TREE;};

    std::string to_string() const;

    bool is_path() const {
        if (children_.size() > 1) {
            return false;
        } else if (children_.size() == 1) {
            return (*children_.begin())->is_path();
        } else return true;
    };

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

        std::shared_ptr<PathTree> buildFromPaths(const Set& paths);

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

    typedef std::pair<std::shared_ptr<PathTree>, std::shared_ptr<PathTree>> SubPathTree;

    /**
     * @brief Return all matching SubPathTree in the PathTree that match a function. The second parameter return True when the search must stop. 
     * 
     * @param reg path tree register
     * @param f the matching function
     * @param should_stop if we return the match of children of matched tree
     * @return std::vector<SubPathTree> 
     */
    std::vector<SubPathTree> find_sub_tree(PathTree::Register& reg,
                                           std::function<bool(const std::shared_ptr<PathTree>&)> f,
                                           std::function<bool(const std::shared_ptr<PathTree>&)> should_stop) const;

    /**
     * @brief Return the Set of singular sequence of a PathTree
     * Singular sequences are :
     *  - for a paradigm : the children of the paradigm node.
     *  - for a non paradigm : the pathtree itself
     * 
     * @param pt the path tree
     * @return Set the singular sequence set
     */
    static Set singular_sequences(const std::shared_ptr<PathTree>& pt);

    /**
     * @brief Return the path set that correspond to this path tree.
     * 
     * @param pt 
     * @return Path::Set 
     */
    static Set paths(const std::shared_ptr<PathTree>& pt);

    /**
     * @brief Return the path tree that correspond to this path set, if possible
     * 
     * @param paths 
     * @return Path::Set 
     */
    static PathTree fromPaths(const Set& paths);


    std::shared_ptr<PathNode> getNode() const {return node_;}
    std::vector<std::shared_ptr<PathTree>> getChildrenAsVector() const;
    const Set getChildren() const {return children_;};

    virtual size_t hash() const override {return hash_;};

    bool is_valid() const;

    std::string to_string_path() const {
        auto r = "/" + node_->to_string();
        if (children_.size() != 0)
            r += (*children_.begin())->to_string_path();
        return r;
    }

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

    const std::shared_ptr<PathNode> node_;
    const Set children_;
    const size_t hash_;
    

    static int comp(const std::shared_ptr<PathNode>&, const Set&, 
                    const std::shared_ptr<PathNode>&, const Set&);

    virtual int comp_element_(const Element&) const override;
};
}