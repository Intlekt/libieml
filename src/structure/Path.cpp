#include "structure/Path.h"

namespace ieml::structure {
    
    bool PathNode::operator==(const PathNode& a) const noexcept {
        return std::hash<PathNode*>{}(const_cast<PathNode * const>(this)) == std::hash<PathNode*>{}(const_cast<PathNode * const>(&a));
    }
    bool PathNode::operator!=(const PathNode& a) const noexcept {
        return std::hash<PathNode*>{}(const_cast<PathNode * const>(this)) != std::hash<PathNode*>{}(const_cast<PathNode * const>(&a));
    }

    bool RootPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::ROLE:
            return true;
        default:
            return false;
        }
    };
    PathType RootPathNode::getPathType() const {return PathType::ROOT;};
    std::string RootPathNode::to_string() const { return ""; };

    bool RoleNumberPathNode::accept_next(const PathNode& next) const {
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
    PathType RoleNumberPathNode::getPathType() const {return PathType::ROLE;};
    std::string RoleNumberPathNode::to_string() const {return std::to_string(role_type_);}

    std::string JunctionPathNode::to_string() const {
        return "&" + std::string(junction_type_._to_string());
    }

    bool AuxiliaryJunctionPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_AUXILIARY_INDEX:
            return true;
        default:
            return false;
        }
    };
    PathType AuxiliaryJunctionPathNode::getPathType() const {return PathType::JUNCTION_AUXILIARY;};

    bool InflexingJunctionPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_INFLEXING_INDEX:
            return true;
        default:
            return false;
        }
    };
    PathType InflexingJunctionPathNode::getPathType() const {return PathType::JUNCTION_INFLEXING;};

    bool CategoryJunctionPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_CATEGORY_INDEX:
            return true;
        default:
            return false;
        }
    };
    PathType CategoryJunctionPathNode::getPathType() const {return PathType::JUNCTION_CATEGORY;};

    std::string JunctionIndexPathNode::to_string() const {
        return "[" + std::to_string(index_) + "]";;
    }

    bool AuxiliaryJunctionIndexPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_AUXILIARY:
        case PathType::AUXILIARY:
            return true;
        default:
            return false;
        }
    };
    PathType AuxiliaryJunctionIndexPathNode::getPathType() const {return PathType::JUNCTION_AUXILIARY_INDEX;};

    bool InflexingJunctionIndexPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_INFLEXING:
        case PathType::INFLEXING:
            return true;
        default:
            return false;
        }
    };
    PathType InflexingJunctionIndexPathNode::getPathType() const {return PathType::JUNCTION_INFLEXING_INDEX;};

    bool CategoryJunctionIndexPathNode::accept_next(const PathNode& next) const {
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
    PathType CategoryJunctionIndexPathNode::getPathType() const {return PathType::JUNCTION_CATEGORY_INDEX;};

    bool AuxiliaryPathNode::accept_next(const PathNode& next) const {
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
    PathType AuxiliaryPathNode::getPathType() const {return PathType::AUXILIARY;};
    std::string AuxiliaryPathNode::to_string() const {
        return "*" + std::string(auxiliary_type_._to_string());
    }

    bool InflexingPathNode::accept_next(const PathNode& next) const {
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
    PathType InflexingPathNode::getPathType() const {return PathType::INFLEXING;};
    std::string InflexingPathNode::to_string() const {
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

    bool WordPathNode::accept_next(const PathNode& next) const {
        return false;
    };
    PathType WordPathNode::getPathType() const {return PathType::WORD;};
    std::string WordPathNode::to_string() const {
        return "'" + word_.to_string() + "'";
    }

    void Path::check() {
        if (next_) { 
            if(!node_->accept_next(*(next_->node_)))
                throw std::invalid_argument("Invalid path descendent for " + node_->to_string() + " got " + next_->node_->to_string());

            next_->check();
        }
    }
    std::string Path::to_string() const {
        auto node_s = "/" + node_->to_string();
        if (next_)
            node_s += next_->to_string();

        return node_s;
    }

    std::shared_ptr<PathTree> PathTree::buildFromPaths(std::vector<std::shared_ptr<Path>> paths) {
        const auto node = paths[0]->getNode();
        PathType path_type(PathType::ROOT);
        if (paths[0]->getNext())
            path_type = paths[0]->getNext()->getNode()->getPathType();

        std::unordered_map<PathNode*, std::vector<std::shared_ptr<Path>>> children_paths;

        for (auto path : paths) {
            // maybe change that to test equality by value ?
            // will have issues with junction 
            if (*path->getNode() != *node)
                throw std::invalid_argument("All path does not share the same prefix: had '" + node->to_string() + "', got '" + path->getNode()->to_string() + "'.");

            auto subpath = path->getNext();
            if (subpath) {
                if (subpath->getNode()->getPathType() != path_type)
                    throw std::invalid_argument("All subpath does not share the same path type: had '" + std::string(path_type._to_string()) + "', got '" \
                                                    + std::string(subpath->getNode()->getPathType()._to_string()) + "'.");

                if (children_paths.count(subpath->getNode().get()) < 1) {
                    children_paths.insert({subpath->getNode().get(), {subpath}});
                } else {
                    children_paths[subpath->getNode().get()].push_back(subpath);
                }
            }
        }
        
        std::vector<std::shared_ptr<PathTree>> children;

        for (auto paths: children_paths) {
            children.push_back(buildFromPaths(paths.second));
        }

        return std::make_shared<PathTree>(std::move(node), std::move(children));
    }
}
