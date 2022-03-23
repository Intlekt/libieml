#pragma once


#include "structure/path/PathTree.h"
#include "structure/link/Link.h"

namespace ieml::structure {

class LinkRegister {
public:
    LinkRegister() = default;

    LinkRegister(const LinkRegister&) = delete;
    LinkRegister& operator()(LinkRegister&) = delete;

    Link::Ptr getLinkByPathTree(PathTree::Ptr p) const { return links_.find(p)->second;}
    bool exists(PathTree::Ptr p) const { return links_.count(p) == 1;}

    Link::Ptr create_link(
         const PathTree::Ptr& p, 
         const LinkArgumentDefinition& arguments,
         const TemplateName& template_language_string,
         const PathNode::Ptr& phrase_tree_inflection);

private:
    std::unordered_map<PathTree::Ptr, Link::Ptr> links_; 
};


}