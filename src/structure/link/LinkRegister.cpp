#include "structure/link/LinkRegister.h"


using namespace ieml::structure;


Link::Ptr LinkRegister::create_link(const PathTree::Ptr& p, 
                                    const LinkArgumentDefinition& arguments,
                                    const TemplateName& template_language_string,
                                    const PathNode::Ptr& phrase_tree_inflection) {

        const auto res = std::shared_ptr<Link>(new Link(arguments, template_language_string, phrase_tree_inflection, p));
        links_.insert({p, res});
        return res;
}

void LinkRegister::register_function(const Link::Ptr& link, WordFunction&& function) {
        word_functions_.insert({link, std::move(function)});
}