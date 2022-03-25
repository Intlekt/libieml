#pragma once


#include "structure/path/PathTree.h"
#include "structure/link/Link.h"
#include "structure/link/function/WordFunction.h"

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
        WordFunction&& function
    );
    
    const auto& getLinks() const {return links_;}
    auto getFunctions(const Link::Ptr& link) const { return word_functions_.equal_range(link);}

private:
    std::unordered_map<PathTree::Ptr, Link::Ptr> links_; 
    std::unordered_multimap<Link::Ptr, WordFunction> word_functions_; 
};

}