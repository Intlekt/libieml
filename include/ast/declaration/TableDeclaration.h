#pragma once


#include "ast/interfaces/IDeclaration.h"
#include "ast/InvariantParanodeMapping.h"


namespace ieml::AST {
class TableDeclaration : public virtual IDeclaration {
public:

    TableDeclaration(
        CharRange::Ptr&& char_range,
        std::vector<std::shared_ptr<InvariantParanodeMapping>>&& mapping) : 
        AST(std::move(char_range)),
        IDeclaration(DeclarationType::TABLE),
        mappings_(std::move(mapping)) {} 

    virtual std::string to_string() const override;

    virtual void check_declaration(ieml::parser::ParserContextManager& ctx) const override;

    virtual std::string getDeclarationString() const override {return "@table";};
    
private:
    const std::vector<std::shared_ptr<InvariantParanodeMapping>> mappings_;

};
}