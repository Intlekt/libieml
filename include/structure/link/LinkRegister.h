#pragma once


#include "structure/path/PathTree.h"
#include "structure/link/Link.h"
#include "structure/link/function/WordCondition.h"
#include "structure/link/function/WordDomain.h"

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

    void register_function(
        const Link::Ptr& link, 
        WordDomain&& domain,
        WordCondition::Ptr&& condition
    );

private:
    std::unordered_map<PathTree::Ptr, Link::Ptr> links_; 
    std::unordered_map<Link::Ptr, std::pair<WordDomain, WordCondition::Ptr>> word_functions_; 
};


}