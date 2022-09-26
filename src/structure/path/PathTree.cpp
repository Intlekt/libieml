#include "structure/path/PathTree.h"

#include <sstream>
#include <vector>

using namespace ieml::structure;

std::vector<PathTree::Set> PathTree::cartesian_product(const std::vector<PathTree::Vector> &children_list)
{
    size_t size = 1;
    for (const auto &child : children_list)
        size *= child.size();

    std::vector<PathTree::Set> bins(size);

    size_t curr_ratio = 1;
    for (const auto &children : children_list)
    {
        for (size_t i = 0; i < size; ++i)
            bins[i].insert(children[(i / curr_ratio) % children.size()]);

        curr_ratio *= children.size();
    }

    return bins;
}

PathTree::Vector PathTree::Register::get_or_create_product(const PathNode::Vector &node_set, const std::vector<PathTree::Vector> &children_list)
{
    Vector res;
    for (auto &children : cartesian_product(children_list))
        for (auto &node : node_set)
            res.push_back(get_or_create(node, children));

    return res;
}

PathTree::Vector PathTree::Register::get_or_create_product(const std::shared_ptr<PathNode> &node, const std::vector<PathTree::Vector> &children_list)
{
    return get_or_create_product(PathNode::Vector{node}, children_list);
}

PathTree::Ptr PathTree::Register::get_or_create(const std::shared_ptr<PathNode> &node, const PathTree::Set &children)
{
    auto key = Key(node, children);
    auto it = store_.find(key);
    if (it != store_.end())
        return it->second;

    auto item = PathTree::Ptr(new PathTree(node, children));
    store_.insert({key, item});
    return item;
}

PathTree::Ptr PathTree::Register::get_or_create(const std::shared_ptr<PathNode> &node)
{
    return get_or_create(node, Set{});
}

PathTree::Ptr PathTree::Register::buildFromPaths(const PathTree::Set &paths)
{
    if (paths.size() == 0)
        return nullptr;

    const auto &node = (*paths.begin())->getNode();

    // all of subpaths node must be of same type
    // Not : we ignore the first node of the path. The path without the first node is called
    // subpath. The first node of the path must be the same for all paths
    PathType path_type(PathType::ROOT);
    if ((*paths.begin())->getChildren().size() != 0)
        path_type = (*(*paths.begin())->getChildren().begin())->getNode()->getPathType();

    // Map the first node of each subpath to the subpath set that have the node as prefix
    std::unordered_map<PathNode::Ptr, PathTree::Set, PathNode::HashFunctor, PathNode::EqualityFunctor> children_paths;

    // Group subpaths by first node
    for (auto path : paths)
    {
        if (*path->getNode() != *node)
            throw std::invalid_argument("All path does not share the same prefix: had '" + node->to_string() + "', got '" + path->getNode()->to_string() + "'.");

        auto children = path->getChildren();
        if (children.size() != 0)
        {
            auto subpath = *children.begin();
            if (subpath->getNode()->getPathType() != path_type)
                throw std::invalid_argument("All subpath does not share the same path type: had '" + std::string(path_type._to_string()) + "', got '" + std::string(subpath->getNode()->getPathType()._to_string()) + "'.");

            if (children_paths.count(subpath->getNode()) < 1)
            {
                children_paths.insert({subpath->getNode(), {subpath}});
            }
            else
            {
                children_paths[subpath->getNode()].insert(subpath);
            }
        }
    }

    Set children;
    // then build all sub path tree
    for (auto paths : children_paths)
    {
        children.insert(buildFromPaths(paths.second));
    }

    return get_or_create(node, children);
}

PathTree::Vector PathTree::Register::buildFromPaths_product(const PathTree::Set &invariant_paths, const std::vector<PathTree::Vector> &variant_paths)
{
    Vector res;
    for (auto &bin : cartesian_product(variant_paths))
    {
        bin.insert(invariant_paths.begin(), invariant_paths.end());
        res.push_back(buildFromPaths(bin));
    }
    return res;
}

std::vector<PathTree::Set> PathTree::Register::expand_path(const PathTree::Ptr &path_tree,
                                                           const Set &prefixes)
{
    std::vector<PathTree::Set> res;

    for (const auto &ss : singular_sequences(path_tree))
    {
        Set set;

        for (const auto &path : paths(ss))
            for (const auto &prefix : prefixes)
                if (prefix->is_prefix(path))
                    set.insert(path);

        res.push_back(set);
    }

    return res;
}

std::string PathTree::to_string() const
{
    std::stringstream os;

    os << node_->to_string();

    for (auto &child : children_)
    {
        os << "(" << child->to_string() << ")";
    }
    return os.str();
}

bool PathTree::is_phrase_word() const
{
    return get_phrase_word() != nullptr;
}

Word::Ptr PathTree::get_phrase_word() const
{
    // match only the path tree that is composed by
    // 1) # root
    // 2) role 0
    // 3) inflection list (optional)
    // 4) word
    // => return word

    if (node_->getPathType() != +PathType::ROOT || children_.size() != 1)
        return nullptr;

    auto role = *children_.begin();
    if (role->node_->getPathType() != +PathType::ROLE || role->children_.size() != 1)
        return nullptr;

    auto infl_or_word = *role->children_.begin();
    if (infl_or_word->node_->getPathType() == +PathType::WORD)
        return *infl_or_word->node_->getWords().begin();

    if (infl_or_word->node_->getPathType() != +PathType::INFLECTION || infl_or_word->children_.size() != 1)
        return nullptr;

    auto word = *infl_or_word->children_.begin();
    if (word->node_->getPathType() == +PathType::WORD)
        return *word->node_->getWords().begin();

    return nullptr;
}

std::vector<PathTree::Ptr> PathTree::getChildrenAsVector() const
{
    std::vector<PathTree::Ptr> v;

    if (children_.size() == 0)
        return v;

    v.reserve(children_.size());
    for (auto it = children_.begin(); it != children_.end(); ++it)
    {
        v.push_back(*it);
    }

    return v;
}

std::vector<PathTree::SubPathTree> PathTree::find_sub_tree(PathTree::Register &register_,
                                                           std::function<bool(const PathTree::Ptr &)> f,
                                                           std::function<bool(const PathTree::Ptr &)> should_stop) const
{
    std::vector<SubPathTree> res;
    for (auto &child : children_)
    {
        bool matched = f(child);
        if (matched)
            res.push_back(SubPathTree{register_.get_or_create(node_), child});

        if (!should_stop(child))
        {
            auto child_res = child->find_sub_tree(register_, f, should_stop);
            res.reserve(res.size() + child_res.size());

            for (auto &item : child_res)
            {
                res.push_back(SubPathTree{register_.get_or_create(node_, {item.first}), item.second});
            }
        }
    }
    return res;
}

PathTree::Vector PathTree::singular_sequences(const PathTree::Ptr &pt)
{
    PathTree::Vector res;
    switch (pt->getNode()->getPathType())
    {
    case PathType::ROOT:
    case PathType::JUNCTION_PHRASE:
        return {pt};
    case PathType::PARADIGM:
        for (auto &p_index : pt->getChildren())
        {
            // the child of PARADIGM is PARADIGM_INDEX
            const auto &children = p_index->getChildren();
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

PathTree::Set PathTree::Register::paths(const PathTree::Ptr &pt)
{
    if (pt->is_path())
        return {pt};

    PathTree::Set res;
    for (const auto &child : pt->children_)
    {
        for (const auto &subpath : paths(child))
        {
            res.insert(get_or_create(pt->node_, {subpath}));
        }
    }

    return res;
}

PathTree::Set PathTree::Register::invariant_paths(const PathTree::Ptr &paradigm)
{
    PathTree::Vector singular_sequences = PathTree::singular_sequences(paradigm);

    std::unordered_map<PathTree::Ptr, PathTree::Set> prefix_to_subSS;

    bool first = true;
    for (PathTree::Ptr ss : singular_sequences)
    {
        // Get all sub phrase or word (all sub #), skipping the first one (the root)
        const auto is_phrase_or_word = [ss](const PathTree::Ptr &pt)
        { return ss != pt && (pt->is_phrase() || pt->is_word()); };

        auto subtrees = ss->find_sub_tree(
            *this,
            is_phrase_or_word,
            is_phrase_or_word);

        if (first)
        {
            first = false;
            for (auto pair : subtrees)
            {
                prefix_to_subSS.insert({pair.first, PathTree::Set{pair.second}});
            }
        }
        else
        {
            std::unordered_map<PathTree::Ptr, PathTree::Set> prefix_to_subSS_next;

            for (auto pair : subtrees)
            {
                const auto it = prefix_to_subSS.find(pair.first);

                // if the prefix is not present we drop the prefix key
                if (it == prefix_to_subSS.end())
                {
                    continue;
                }

                it->second.insert(pair.second);
                prefix_to_subSS_next.insert({it->first, it->second});
            }

            prefix_to_subSS = prefix_to_subSS_next;
        }
    }

    std::unordered_map<PathTree::Ptr, PathTree::Ptr> prefix_to_subinvariant;

    for (auto pair : prefix_to_subSS)
    {
        if (pair.second.size() == 1)
        {
            // if only one subtree exists at this prefix
            prefix_to_subinvariant.insert({pair.first, *pair.second.begin()});
        }
        else
        {
            bool drop = false;
            PathTree::Vector subphrases;
            // muliple subtree at this prefix
            for (auto subtree : pair.second)
            {
                if (subtree->is_word())
                {
                    // if any of the subtree is a word, we drop the prefix
                    drop = true;
                    break;
                }

                subphrases.push_back(subtree);
            }

            if (!drop)
            {
                // recurse to build the subinvariant
                const auto invariant = buildFromPaths(invariant_paths(build_paradigm(subphrases)));
                if (invariant != nullptr)
                {
                    if (invariant->is_valid())
                    {
                        // if the invariant is a valid path tree (no root is missing), otherwise it is nullptr
                        prefix_to_subinvariant.insert({pair.first, invariant});
                    }
                }
            }
        }
    }

    PathTree::Set paths;
    for (auto pair : prefix_to_subinvariant)
    {
        for (auto path : this->paths(this->concat(pair.first, pair.second)))
        {
            paths.insert(path);
        }
    }

    return paths;
}

PathTree::Set PathTree::Register::singular_sequence_intersection(const PathTree::Ptr &paradigm)
{
    PathTree::Vector singular_sequences = PathTree::singular_sequences(paradigm);

    auto it = singular_sequences.begin();
    PathTree::Set invariant_paths = paths(*it);

    ++it;
    while (it != singular_sequences.end())
    {
        PathTree::Set ss_paths = paths(*it);
        PathTree::Set out;
        std::copy_if(invariant_paths.begin(),
                     invariant_paths.end(),
                     std::inserter(out, out.begin()),
                     [&ss_paths](const PathTree::Ptr &pt)
                     { return ss_paths.count(pt) != 0; });
        invariant_paths = out;
        ++it;
    }

    return invariant_paths;
}

PathTree::Ptr PathTree::Register::build_paradigm(const PathTree::Vector &paradigm)
{
    structure::PathTree::Set children;

    for (size_t i = 0; i < paradigm.size(); ++i)
    {
        children.insert({get_or_create(
            std::make_shared<structure::ParadigmIndexPathNode>(i),
            {paradigm[i]})});
    }

    return get_or_create(std::make_shared<structure::ParadigmPathNode>(), children);
}

PathTree::Ptr PathTree::Register::build_phrase_word(const CategoryWord::Ptr word, const InflectionPathNode::Ptr inflection)
{
    auto w_pt = get_or_create(std::make_shared<WordPathNode>(word), PathTree::Set{});
    if (inflection)
        w_pt = get_or_create(inflection, PathTree::Set{w_pt});
    w_pt = get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), PathTree::Set{w_pt});
    return get_or_create(std::make_shared<RootPathNode>(), PathTree::Set{w_pt});
}

PathTree::Ptr PathTree::Register::concat(const PathTree::Ptr &left, const PathTree::Ptr &right)
{
    if (left->getChildren().size() == 0)
    {
        return this->get_or_create(left->getNode(), {right});
    }
    auto child = this->concat(*left->getChildren().begin(), right);
    return this->get_or_create(left->getNode(), {child});
}

bool PathTree::is_contained_singular(const PathTree::Ptr &path_tree) const
{
    if (*node_ != *path_tree->node_)
        return false;
    if (children_.size() == 0)
        return true;
    if (path_tree->children_.size() == 0)
        return false;

    const auto &child = *children_.begin();

    for (const auto &pt_child : path_tree->children_)
        if (child->is_contained_singular(pt_child))
            return true;

    return false;
}

bool PathTree::is_contained(const PathTree::Ptr &path_tree) const
{
    if (!is_path())
        throw std::invalid_argument("is_prefix must be called on a path.");

    for (const auto &ss : singular_sequences(path_tree))
        if (!is_contained_singular(ss))
            return false;

    return true;
}

bool PathTree::is_prefix_singular(const PathTree::Ptr &path_tree) const
{
    if (*node_ != *path_tree->node_)
        return false;
    if (children_.size() == 0)
        return true;
    if (path_tree->children_.size() == 0)
        return false;

    const auto &child = *children_.begin();

    for (const auto &pt_child : path_tree->children_)
        if (!child->is_prefix_singular(pt_child))
            return false;

    return true;
}

bool PathTree::is_prefix(const PathTree::Ptr &path_tree) const
{
    if (!is_path())
        throw std::invalid_argument("is_prefix must be called on a path.");

    for (const auto &ss : singular_sequences(path_tree))
        if (!is_prefix_singular(ss))
            return false;

    return true;
}

bool PathTree::is_valid() const
{
    // ensure that all the index are used for junction and paradigms
    if (is_paradigm() || is_junction())
    {
        size_t i = 0;
        for (auto const &c : children_)
        {
            if (i != c->node_->getIndex())
                return false;
            ++i;
        }
    }

    // ensure that the phrase has at least a root role
    if (is_phrase())
    {
        const auto roles = get_all_roles();

        bool has_root = false;
        for (const auto &role : roles)
        {
            if (role == +RoleType::ROOT)
            {
                has_root = true;
                break;
            }
        }

        if (!has_root)
        {
            return false;
        }
    }

    if (children_.size() != 0)
    {
        for (const auto &c : children_)
        {
            if (!node_->accept_next(*c->getNode()))
                return false;
            if (!c->is_valid())
                return false;
        }
    }
    return true;
}

std::vector<RoleType> PathTree::get_all_roles() const
{
    if (!is_phrase())
    {
        throw std::invalid_argument("Cannot gather roles for non-phrase PathTree.");
    }

    std::vector<RoleType> roles;
    for (const auto &child : children_)
    {
        if (child->is_role())
        {
            roles.push_back(child->getNode()->getRoleType());
        }
    }

    return roles;
}

int PathTree::comp(const std::shared_ptr<PathNode> &nodeA, const PathTree::Set &childrenA,
                   const std::shared_ptr<PathNode> &nodeB, const PathTree::Set &childrenB)
{
    if (*nodeA == *nodeB)
    {
        auto it = childrenA.begin();
        auto it_r = childrenB.begin();

        for (; it != childrenA.end() && it_r != childrenB.end();)
        {
            if (**it != **it_r)
                return **it < **it_r ? -1 : 1;

            ++it;
            ++it_r;
        }

        if (it == childrenA.end() && it_r == childrenB.end())
            return 0;

        return it == childrenA.end() ? -1 : 1;
    }
    else
        return *nodeA < *nodeB ? -1 : 1;
}

int PathTree::comp_element_(const Element &o_elem) const
{
    const auto &o = dynamic_cast<const PathTree &>(o_elem);
    return comp(node_, children_, o.getNode(), o.getChildren());
}

size_t PathTree::hash_internal(const std::shared_ptr<PathNode> &node, const Set &children)
{
    size_t seed = 0;
    hash_combine(seed, *node);

    for (auto &c : children)
        hash_combine(seed, c->hash());

    return seed;
}

size_t PathTree::count_paths(const Set &children)
{
    if (children.size() == 0)
        return 1;
    size_t total = 0;
    for (const auto &child : children)
        total += count_paths(child->getChildren());

    return total;
}