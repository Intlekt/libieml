#include "structure/path/PathTree.h"

#include <sstream>
#include <vector>

using namespace ieml::structure;


std::vector<PathTree::Set> PathTree::cartesian_product(const std::vector<PathTree::Vector>& children_list) {
    size_t size = 1;
    for (const auto& child: children_list) 
        size *= child.size();

    std::vector<PathTree::Set> bins(size);

    size_t curr_ratio = 1;
    for (const auto& children: children_list) {
        for (size_t i = 0; i < size; ++i)
            bins[i].insert(children[(i / curr_ratio) % children.size()]);

        curr_ratio *= children.size();
    }


    return bins;
}


PathTree::Vector PathTree::Register::get_or_create_product(const PathNode::Vector& node_set, const std::vector<PathTree::Vector>& children_list) {
    Vector res;
    for (auto& children: cartesian_product(children_list))
        for (auto& node: node_set)
            res.push_back(get_or_create(node, children));

    return res;
}

PathTree::Vector PathTree::Register::get_or_create_product(const std::shared_ptr<PathNode>& node, const std::vector<PathTree::Vector>& children_list) {
    return get_or_create_product(PathNode::Vector{node}, children_list);
}

std::shared_ptr<PathTree> PathTree::Register::get_or_create(const std::shared_ptr<PathNode>& node, const PathTree::Set& children) {
    auto key = Key(node, children);
    auto it = store_.find(key);
    if (it != store_.end()) return it->second;

    auto item = std::shared_ptr<PathTree>(new PathTree(node, children));
    store_.insert({key, item});
    return item;
}

std::shared_ptr<PathTree> PathTree::Register::get_or_create(const std::shared_ptr<PathNode>& node) {
    return get_or_create(node, Set{});
}

std::shared_ptr<PathTree> PathTree::Register::buildFromPaths(const PathTree::Set& paths) {
    if (paths.size() == 0) 
        return nullptr;
    
    const auto& node = (*paths.begin())->getNode();

    PathType path_type(PathType::ROOT);
    if ((*paths.begin())->getChildren().size() != 0)
        path_type = (*(*paths.begin())->getChildren().begin())->getNode()->getPathType();

    std::unordered_map<PathNode*, PathTree::Set> children_paths;

    for (auto path : paths) {
        if (*path->getNode() != *node)
            throw std::invalid_argument("All path does not share the same prefix: had '" + node->to_string() + "', got '" + path->getNode()->to_string() + "'.");

        auto children = path->getChildren();
        if (children.size() != 0) {
            auto subpath = *children.begin();
            if (subpath->getNode()->getPathType() != path_type)
                throw std::invalid_argument("All subpath does not share the same path type: had '" + std::string(path_type._to_string()) + "', got '" \
                                                + std::string(subpath->getNode()->getPathType()._to_string()) + "'.");

            if (children_paths.count(subpath->getNode().get()) < 1) {
                children_paths.insert({subpath->getNode().get(), {subpath}});
            } else {
                children_paths[subpath->getNode().get()].insert(subpath);
            }
        }
    }
    
    Set children;

    for (auto paths: children_paths) {
        children.insert(buildFromPaths(paths.second));
    }

    return get_or_create(node, children);
}

PathTree::Vector PathTree::Register::buildFromPaths_product(const PathTree::Set& invariant_paths, const std::vector<PathTree::Vector>& variant_paths) {
    Vector res;
    for (auto& bin: cartesian_product(variant_paths)) {
        bin.insert(invariant_paths.begin(), invariant_paths.end());
        res.push_back(buildFromPaths(bin));
    }
    return res;
}

std::vector<PathTree::Set> PathTree::Register::expand_path(const std::shared_ptr<PathTree>& path_tree, 
                                                           const Set& prefixes) {
    std::vector<PathTree::Set> res;

    for (const auto& ss : singular_sequences(path_tree)) {
        Set set;
        
        for (const auto& path: paths(ss)) 
            for (const auto& prefix: prefixes)
                if (prefix->is_prefix(path))
                    set.insert(path);

        res.push_back(set);
    }

    return res;
}

std::string PathTree::to_string() const {
    std::stringstream os;

    os << node_->to_string();

    for (auto& child: children_) {
        os << "(" << child->to_string() << ")";
    }
    return os.str();
}

std::vector<std::shared_ptr<PathTree>> PathTree::getChildrenAsVector() const {
    std::vector<std::shared_ptr<PathTree>> v;

    if (children_.size() == 0)
        return v;

    v.reserve(children_.size());
    for(auto it = children_.begin(); it != children_.end(); ++it ) {
        v.push_back(*it);
    }

    return v;
}

std::vector<PathTree::SubPathTree> PathTree::find_sub_tree(PathTree::Register& register_,
                                                           std::function<bool(const std::shared_ptr<PathTree>&)> f,
                                                           std::function<bool(const std::shared_ptr<PathTree>&)> should_stop) const {
    std::vector<SubPathTree> res;
    for (auto& child: children_) {
        bool matched = f(child);
        if (matched) res.push_back(SubPathTree{register_.get_or_create(node_), child});
        
        if (!should_stop(child)) {
            auto child_res = child->find_sub_tree(register_, f, should_stop);
            res.reserve(res.size() + child_res.size());

            for (auto& item: child_res) {
                res.push_back(SubPathTree{register_.get_or_create(node_, {item.first}), item.second});
            }
        }
    }
    return res;
}

PathTree::Vector PathTree::singular_sequences(const std::shared_ptr<PathTree>& pt) {
    PathTree::Vector res;
    switch (pt->getNode()->getPathType()) {
        case PathType::ROOT:
        case PathType::JUNCTION_PHRASE:
            return {pt};
        case PathType::PARADIGM:
            for (auto& p_index: pt->getChildren()) {
                // the child of PARADIGM is PARADIGM_INDEX
                const auto& children = p_index->getChildren();
                auto ss_it = children.begin();
                if (ss_it == children.end())             
                    throw std::invalid_argument("Invalid path sequence for singular sequences " + pt->to_string_path());

                res.push_back(*ss_it);
            }
            return res;
        default:
            throw std::invalid_argument("Invalid path type for singular sequences " + std::string(pt->getNode()->getPathType()._to_string()));
    }
}

PathTree::Set PathTree::Register::paths(const std::shared_ptr<PathTree>& pt) {
    if (pt->is_path())
        return {pt};
    
    PathTree::Set res;
    for (const auto& child: pt->children_) {
        for (const auto& subpath: paths(child)) {
            res.insert(get_or_create(pt->node_, {subpath}));
        }
    }

    return res;
}

PathTree::Set PathTree::Register::invariant_paths(const std::shared_ptr<PathTree>& paradigm) {
    PathTree::Vector singular_sequences = PathTree::singular_sequences(paradigm);

    auto it = singular_sequences.begin();
    PathTree::Set invariant_paths = paths(*it);

    ++it;
    while (it != singular_sequences.end()) {
        PathTree::Set ss_paths = paths(*it);
        PathTree::Set out;
        std::copy_if(invariant_paths.begin(), 
                     invariant_paths.end(), 
                     std::inserter(out, out.begin()), 
                     [&ss_paths](const std::shared_ptr<PathTree>& pt){return ss_paths.count(pt) != 0;});
        invariant_paths = out;
        ++it;
    }

    return invariant_paths;
}

bool PathTree::is_contained_singular(const std::shared_ptr<PathTree>& path_tree) const {
    if (*node_ != *path_tree->node_) return false;
    if (children_.size() == 0) return true;
    if (path_tree->children_.size() == 0) return false;

    const auto& child = *children_.begin();

    for (const auto& pt_child: path_tree->children_)
        if (child->is_contained_singular(pt_child))
            return true;

    return false;
}

bool PathTree::is_contained(const std::shared_ptr<PathTree>& path_tree) const {
    if (!is_path()) 
        throw std::invalid_argument("is_prefix must be called on a path.");
    
    for (const auto& ss: singular_sequences(path_tree))
        if (!is_contained_singular(ss))
            return false;
    
    return true;
}

bool PathTree::is_prefix_singular(const std::shared_ptr<PathTree>& path_tree) const {
    if (*node_ != *path_tree->node_) return false;
    if (children_.size() == 0) return true;
    if (path_tree->children_.size() == 0) return false;

    const auto& child = *children_.begin();

    for (const auto& pt_child: path_tree->children_)
        if (!child->is_prefix_singular(pt_child))
            return false;

    return true;
}

bool PathTree::is_prefix(const std::shared_ptr<PathTree>& path_tree) const {
    if (!is_path()) 
        throw std::invalid_argument("is_prefix must be called on a path.");
    
    for (const auto& ss: singular_sequences(path_tree))
        if (!is_prefix_singular(ss))
            return false;
    
    return true;
}

bool PathTree::is_valid() const {
    // ensure that all the index are used for junction and paradigms
    if (is_paradigm() || is_junction()) {
        size_t i = 0;
        for (auto const& c: children_) {
            if (i != c->node_->getIndex()) return false;
            ++i;
        }
    }

    // ensure that the phrase has at least a root role
    // if (is_phrase()) {

    // }


    if (children_.size() != 0) {
        for (const auto& c: children_) {
            if (!node_->accept_next(*c->getNode())) return false;
            if (!c->is_valid()) return false;
        }
    }
    return true;
}


int PathTree::comp(const std::shared_ptr<PathNode>& nodeA, const PathTree::Set& childrenA, 
                    const std::shared_ptr<PathNode>& nodeB, const PathTree::Set& childrenB) {
    if (*nodeA == *nodeB) {
        auto it = childrenA.begin();
        auto it_r = childrenB.begin();

        for (;it != childrenA.end() && it_r != childrenB.end();) {
            if (**it != **it_r) return **it < **it_r ? -1 : 1;

            ++it;
            ++it_r;
        }

        if (it == childrenA.end() && it_r == childrenB.end()) return 0;

        return it == childrenA.end() ? -1 : 1;
    } else 
        return *nodeA < *nodeB ? -1 : 1;
}

int PathTree::comp_element_(const Element& o_elem) const {
    const auto& o = dynamic_cast<const PathTree&>(o_elem);
    return comp(node_, children_, o.getNode(), o.getChildren());
}

size_t PathTree::hash_internal(const std::shared_ptr<PathNode>& node, const Set& children) {
    size_t seed = 0;
    hash_combine(seed, *node);

    for (auto& c: children)
        hash_combine(seed, c->hash());

    return seed;
}

size_t PathTree::count_paths(const Set& children) {
    if (children.size() == 0)
        return 1;
    size_t total = 0;
    for (const auto& child: children)
        total += count_paths(child->getChildren());

    return total;
}