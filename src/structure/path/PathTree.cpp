#include "structure/path/PathTree.h"

#include <sstream>

using namespace ieml::structure;



PathTree::Vector PathTree::Register::get_or_create_product(const PathNode::Vector& node_set, const std::vector<PathTree::Vector>& children_list) {
    size_t size = 1;
    for (auto& set : children_list) size *= set.size();

    std::vector<Set> bins(size);
    for (auto& set : children_list) {
        size_t i = 0;
        for (auto& child: set) {
            for (size_t j = 0; j < size / set.size(); ++j)
                bins[j * set.size() + i].insert(child);

            ++i;
        }
    }

    Vector res;
    for (auto& children: bins)
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

PathTree::Set PathTree::paths(PathTree::Register& reg, const std::shared_ptr<PathTree>& pt) {
    if (pt->is_path())
        return {pt};
    
    PathTree::Set res;
    for (const auto& child: pt->children_) {
        for (const auto& subpath: paths(reg, child)) {
            res.insert(reg.get_or_create(pt->node_, {subpath}));
        }
    }

    return res;
}

std::shared_ptr<PathTree> PathTree::paradigm_invariant(PathTree::Register& reg, const std::shared_ptr<PathTree>& paradigm) {
    PathTree::Vector singular_sequences = PathTree::singular_sequences(paradigm);

    auto it = singular_sequences.begin();
    PathTree::Set invariant_paths = PathTree::paths(reg, *it);

    ++it;
    while (it != singular_sequences.end()) {
        PathTree::Set ss_paths = PathTree::paths(reg, *it);
        PathTree::Set out;
        std::copy_if(invariant_paths.begin(), 
                     invariant_paths.end(), 
                     std::inserter(out, out.begin()), 
                     [&ss_paths](const std::shared_ptr<PathTree>& pt){return ss_paths.count(pt) != 0;});
        invariant_paths = out;
        ++it;
    }

    return reg.buildFromPaths(invariant_paths);
}



bool PathTree::is_valid() const {
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
