#pragma once

#include <memory>
#include <vector>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IJunction.h"
#include "ast/Constants.h"
#include "ast/Identifier.h"
#include "ast/Auxiliary.h"
#include "ast/InflectionList.h"
#include "ast/Word.h"


namespace ieml::AST {

class PathNode : public virtual AST {
public:
    PathNode() {}

    ieml::structure::PathTree::Set check_path_node(parser::ParserContextManager& ctx, 
                                                   const structure::RoleType& current_role,
                                                   const std::vector<std::shared_ptr<PathNode>>::const_iterator& begin,
                                                   const std::vector<std::shared_ptr<PathNode>>::const_iterator& end) {

        const auto& node_list = _check_path_node(ctx, current_role, begin, end);
        if (node_list.size() == 0) return {nullptr};
        
        structure::RoleType next_role = current_role;
        if ((*node_list.begin())->getPathType() == +ieml::structure::PathType::ROLE) {
            next_role = (*node_list.begin())->getRoleType();
        }

        ieml::structure::PathTree::Set children;
        if (begin != end) 
            children = (*begin)->check_path_node(ctx, next_role, begin + 1, end);

        std::shared_ptr<ieml::structure::PathTree> current;
        auto last_children = children;

        for (auto it_node = node_list.rbegin(); it_node != node_list.rend(); ++it_node) {
            if (last_children.size() != 0) {
                const auto& child = *last_children.begin();
                if (child == nullptr) return {nullptr};

                if (!(*it_node)->accept_next(*child->getNode())) {
                    ctx.getErrorManager().visitorError(
                        getCharRange(),
                        "Invalid path definition, a " + std::string((*it_node)->getPathType()._to_string()) + 
                        " path node cannot be followed by a " + std::string(child->getNode()->getPathType()._to_string()) + " path node."
                    );

                    return {nullptr};
                }
            }

            current = ctx.getPathTreeRegister().get_or_create(*it_node, last_children);
            last_children = {current};
        }

        return {current};
    }

    virtual PathType getPathType() const = 0;

private:

    virtual std::vector<std::shared_ptr<ieml::structure::PathNode>> _check_path_node(parser::ParserContextManager& ctx, 
                                                                                    const structure::RoleType& current_role,
                                                                                    const std::vector<std::shared_ptr<PathNode>>::const_iterator& begin,
                                                                                    const std::vector<std::shared_ptr<PathNode>>::const_iterator& end) = 0;

};


class RootPathNode: public virtual PathNode, public virtual AST {
public:
    RootPathNode(std::shared_ptr<CharRange>&& char_range) : 
        AST(std::move(char_range)),
        PathNode() {}

    virtual std::string to_string() const override {
        return "#";
    }
    virtual PathType getPathType() const override {return PathType::ROOT;};

private:

    virtual std::vector<std::shared_ptr<ieml::structure::PathNode>> _check_path_node(__attribute__((unused)) parser::ParserContextManager& ctx, 
                                                                                     __attribute__((unused)) const structure::RoleType& current_role,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& begin,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& end) override {
        return {std::make_shared<ieml::structure::RootPathNode>()};
    };
};

class ParadigmPathNode: public virtual AST, public virtual PathNode {
public:
    ParadigmPathNode(std::shared_ptr<CharRange>&& char_range,
                     size_t index) : 
            AST(std::move(char_range)),
            PathNode(),
            index_(index) {}

    virtual std::string to_string() const override {
        return "{" + std::to_string(index_) + "}";
    }
    virtual PathType getPathType() const override {return PathType::PARADIGM;};

private:
    virtual std::vector<std::shared_ptr<ieml::structure::PathNode>> _check_path_node(__attribute__((unused)) parser::ParserContextManager& ctx, 
                                                                                     __attribute__((unused)) const structure::RoleType& current_role,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& begin,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& end) override {
        return {
            std::make_shared<ieml::structure::ParadigmPathNode>(),
            std::make_shared<ieml::structure::ParadigmIndexPathNode>(index_)
        };
    };

    const size_t index_;
};

class JunctionPathNode: public virtual AST, public PathNode {
public:
    JunctionPathNode(std::shared_ptr<CharRange>&& char_range,
                     std::shared_ptr<IJunction>&& junction_type,
                     size_t index) : 
            AST(std::move(char_range)),
            PathNode(),
            junction_type_(std::move(junction_type)),
            index_(index) {}

    virtual std::string to_string() const override {
        return "&" + junction_type_->to_string() + "[" + std::to_string(index_) + "]";
    }
    virtual PathType getPathType() const override {return PathType::JUNCTION;};


private:
    virtual std::vector<std::shared_ptr<ieml::structure::PathNode>> _check_path_node(__attribute__((unused)) parser::ParserContextManager& ctx, 
                                                                                     __attribute__((unused)) const structure::RoleType& current_role,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& begin,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& end) override {
        
        const auto& junction = junction_type_->check_junction(ctx);
        if (junction == nullptr) return {};
        
        switch ((*begin)->getPathType()) {
            case PathType::ROLE:
                return {
                    std::make_shared<ieml::structure::PhraseJunctionPathNode>(junction),
                    std::make_shared<ieml::structure::PhraseJunctionIndexPathNode>(index_)
                };

            case PathType::AUXILIARY:
                return {
                    std::make_shared<ieml::structure::AuxiliaryJunctionPathNode>(junction),
                    std::make_shared<ieml::structure::AuxiliaryJunctionIndexPathNode>(index_)
                };

            case PathType::INFLECTION:
                return {
                    std::make_shared<ieml::structure::InflectionJunctionPathNode>(junction),
                    std::make_shared<ieml::structure::InflectionJunctionIndexPathNode>(index_)
                };

            case PathType::ROOT:
            case PathType::WORD:
                return {
                    std::make_shared<ieml::structure::CategoryJunctionPathNode>(junction),
                    std::make_shared<ieml::structure::CategoryJunctionIndexPathNode>(index_)
                };

            default:
                ctx.getErrorManager().visitorError(
                    getCharRange(),
                    "A junction path node cannot be followed by a " + std::string((*begin)->getPathType()._to_string()) + " path node."
                );
                return {};
        }
    };

    const std::shared_ptr<IJunction> junction_type_;
    const size_t index_;
};
 
class AuxiliaryPathNode: public virtual AST, public PathNode  {
public:
    AuxiliaryPathNode(std::shared_ptr<CharRange>&& char_range,
                      std::shared_ptr<IAuxiliary>&& auxiliary) : 
            AST(std::move(char_range)),
            PathNode(),
            auxiliary_(std::move(auxiliary)) {}

    virtual std::string to_string() const override {
        return auxiliary_->to_string();
    }
    virtual PathType getPathType() const override {return PathType::AUXILIARY;};

private:
    virtual std::vector<std::shared_ptr<ieml::structure::PathNode>> _check_path_node(parser::ParserContextManager& ctx, 
                                                                                    const structure::RoleType& current_role,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& begin,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& end) override {
        const auto& auxiliary_set = auxiliary_->check_auxiliary(ctx, current_role);

        if (auxiliary_set.size() != 1) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Cannot use an auxiliary paradigm in a path definition."
            );
            return {};
        }

        if (!*auxiliary_set.begin()) return {};
        
        return {*auxiliary_set.begin()};
    };

    const std::shared_ptr<IAuxiliary> auxiliary_;
};
 
class InflectionListPathNode: public virtual AST, public PathNode {
public:
    InflectionListPathNode(std::shared_ptr<CharRange>&& char_range,
                           std::shared_ptr<IInflectionList>&& inflection_list) : 
            AST(std::move(char_range)),
            PathNode(),
            inflection_list_(std::move(inflection_list)) {}

    virtual std::string to_string() const override {
        return inflection_list_->to_string();
    }
    virtual PathType getPathType() const override {return PathType::INFLECTION;};

private:
    virtual std::vector<std::shared_ptr<ieml::structure::PathNode>> _check_path_node(parser::ParserContextManager& ctx, 
                                                                                    const structure::RoleType& current_role,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& begin,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& end) override {
        const auto& inflection_list_set = inflection_list_->check_inflection_list(ctx, current_role);

        if (inflection_list_set.size() != 1) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Cannot use an inflection list paradigm in a path definition."
            );
            return {};
        }

        if (!*inflection_list_set.begin()) return {};
        
        return {*inflection_list_set.begin()};
    }

    const std::shared_ptr<IInflectionList> inflection_list_;
};

class WordPathNode: public virtual AST, public PathNode {
public:
    WordPathNode(std::shared_ptr<CharRange>&& char_range,
                 std::shared_ptr<Word>&& word) : 
            AST(std::move(char_range)),
            PathNode(),
            word_(std::move(word)) {}

    virtual std::string to_string() const override {
        return word_->to_string();
    }
    virtual PathType getPathType() const override {return PathType::WORD;};

private:
    virtual std::vector<std::shared_ptr<ieml::structure::PathNode>> _check_path_node(parser::ParserContextManager& ctx, 
                                                                                     __attribute__((unused)) const structure::RoleType& current_role,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& begin,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& end) override {
        const auto& word_node = word_->check_category_word(ctx);

        if (!word_node) return {};

        return {word_node};
    }

    const std::shared_ptr<Word> word_;
};

class RoleNumberPathNode: public virtual AST, public PathNode {
public:
    RoleNumberPathNode(std::shared_ptr<CharRange>&& char_range,
                       size_t role_number) : 
            AST(std::move(char_range)),
            PathNode(),
            role_number_(role_number) {}

    virtual std::string to_string() const override {
        return std::to_string(role_number_);
    }
    virtual PathType getPathType() const override {return PathType::ROLE;};

private:
    virtual std::vector<std::shared_ptr<ieml::structure::PathNode>> _check_path_node(parser::ParserContextManager& ctx, 
                                                                                     __attribute__((unused)) const structure::RoleType& current_role,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& begin,
                                                                                     __attribute__((unused)) const std::vector<std::shared_ptr<PathNode>>::const_iterator& end) override {
        auto type = structure::RoleType::_from_integral_nothrow(role_number_);

        if (!type) {
            ctx.getErrorManager().visitorError(
                getCharRange(), "Invalid role number, got '" + std::to_string(role_number_) + "'"
            );
            return {};
        }

        return {std::make_shared<structure::RoleNumberPathNode>(*type)};
    }

    const size_t role_number_;
};

}