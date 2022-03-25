#pragma once


#include "structure/link/LinkArgumentDefinition.h"
#include "structure/Namespace.h"
#include "structure/path/PathNode.h"
#include "structure/path/PathTree.h"

namespace ieml::parser {
    class ParserContextManager;
}

namespace ieml::structure {
class LinkRegister;
class WordRegister;
class CategoryRegister;

class Link {
public:
    typedef std::shared_ptr<Link> Ptr;

    Link(const Link&) = delete;
    Link& operator()(Link&) = delete;


    friend class LinkRegister;

    typedef std::unordered_map<std::string, Word::Ptr> WordValuation;

    const LinkArgumentDefinition& getArguments() const {return arguments_;};

    bool canDescribeWord() const {return phrase_tree_inflection_ != nullptr;}

    const Name getNameForValuation(PathTree::Register& preg, CategoryRegister& creg, WordRegister& wreg, const WordValuation& valuation) const;
private:
    Link(const LinkArgumentDefinition& arguments,
         const TemplateName& template_language_string,
         const PathNode::Ptr& phrase_tree_inflection,
         const PathTree::Ptr& path_tree) : 
        arguments_(arguments),
        template_language_string_(template_language_string),
        phrase_tree_inflection_(phrase_tree_inflection),
        path_tree_(path_tree) {}

    const LinkArgumentDefinition arguments_;
    const TemplateName template_language_string_;
    
    // can be null => in that case the link does not works between words.
    const InflectionPathNode::Ptr phrase_tree_inflection_;
    
    const PathTree::Ptr path_tree_;
};
}