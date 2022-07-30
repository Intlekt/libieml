#pragma once

#include "structure/Constants.h"
#include "structure/Word.h"

#include <memory>
#include <set>



namespace ieml::structure {

class PathNode {
public:

    typedef std::shared_ptr<PathNode> Ptr;

    virtual ~PathNode() = default;

    virtual bool accept_next(const PathNode& next) const = 0;

    virtual PathType getPathType() const = 0;

    bool operator==(const PathNode& a) const {return to_string() == a.to_string();};
    bool operator!=(const PathNode& a) const {return to_string() != a.to_string();};
    bool operator< (const PathNode& a) const {
        if (getPathType() == a.getPathType()) return comp(a) <  0;
        else                                  return getPathType() < a.getPathType();
    };
    bool operator> (const PathNode& a) const {
        if (getPathType() == a.getPathType()) return comp(a) >  0;
        else                                  return getPathType() > a.getPathType();
    };
    bool operator<=(const PathNode& a) const {
        if (getPathType() == a.getPathType()) return comp(a) <= 0;
        else                                  return getPathType() < a.getPathType();
    };
    bool operator>=(const PathNode& a) const {
        if (getPathType() == a.getPathType()) return comp(a) >=  0;
        else                                  return getPathType() > a.getPathType();
    };

    virtual std::string to_string() const = 0;
    virtual const std::set<std::shared_ptr<Word>> getWords() const {return {};};

    virtual size_t getIndex() const {throw std::invalid_argument("No index defined for " +std::string(getPathType()._to_string())+ " path node.");};
    virtual RoleType getRoleType() const {throw std::invalid_argument("No RoleType defined for " +std::string(getPathType()._to_string())+ " path node.");};
    virtual std::shared_ptr<JunctionWord> getJunctionType() const {throw std::invalid_argument("No JunctionWord defined for " +std::string(getPathType()._to_string())+ " path node.");};

    virtual std::shared_ptr<AuxiliaryWord> getAuxialiryType() const {throw std::invalid_argument("No AuxiliaryWord defined for " +std::string(getPathType()._to_string())+ " path node.");};
    virtual const std::set<std::shared_ptr<InflectionWord>>& getInflections() const {throw std::invalid_argument("No InflectionWords defined for " +std::string(getPathType()._to_string())+ " path node.");};
    virtual const std::shared_ptr<CategoryWord> getCategoryWord() const {throw std::invalid_argument("No CategoryWord defined for " +std::string(getPathType()._to_string())+ " path node.");};


    typedef std::set<std::shared_ptr<PathNode>> Set;
    typedef std::vector<std::shared_ptr<PathNode>> Vector;


    struct HashFunctor {
        size_t operator()(const ieml::structure::PathNode::Ptr& a) const;
    };

    struct EqualityFunctor {
        bool operator()(const ieml::structure::PathNode::Ptr& a, const ieml::structure::PathNode::Ptr& b) const {
            return *a == *b;
        }
    };


private:
    virtual int comp(const PathNode& a) const = 0;
};

class RootPathNode : public PathNode {
public:
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

private:
    virtual int comp(__attribute__((unused)) const PathNode& a) const override {return 0;};
};

class ParadigmPathNode : public PathNode {
public:
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

private:
    virtual int comp(__attribute__((unused)) const PathNode& a) const override {return 0;};
};

class ParadigmIndexPathNode : public PathNode {
public:
    ParadigmIndexPathNode(size_t index) : index_(index) {};
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

    virtual size_t getIndex() const override {return index_;};

private:
    virtual int comp(const PathNode& a) const override {
        if (getIndex() == a.getIndex()) return  0;
        if (getIndex() <  a.getIndex()) return -1;
        else                            return  1;
    };

    const size_t index_;
};



class RoleNumberPathNode : public PathNode {
public:
    RoleNumberPathNode(RoleType role_type) : role_type_(role_type) {};
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

    virtual RoleType getRoleType() const override {return role_type_;};
private:
    virtual int comp(const PathNode& a) const override {
        if (getRoleType() == a.getRoleType()) return  0;
        if (getRoleType() <  a.getRoleType()) return -1;
        else                            return  1;
    };

    const RoleType role_type_;
};


class JunctionPathNode : public PathNode {
public:
    JunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : junction_type_(junction_type) {}
    virtual std::string to_string() const override;

    virtual std::shared_ptr<JunctionWord> getJunctionType() const override {return junction_type_;};
    virtual const std::set<std::shared_ptr<Word>> getWords() const override {return {junction_type_};};

private:
    virtual int comp(const PathNode& a) const override {
        if (*getJunctionType() == *a.getJunctionType()) return  0;
        if (*getJunctionType() <  *a.getJunctionType()) return -1;
        else                                            return  1;
    };

    const std::shared_ptr<JunctionWord> junction_type_;
};

class PhraseJunctionPathNode : public JunctionPathNode {
public:
    PhraseJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class AuxiliaryJunctionPathNode : public JunctionPathNode {
public:
    AuxiliaryJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class InflectionJunctionPathNode : public JunctionPathNode {
public:
    InflectionJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class CategoryJunctionPathNode : public JunctionPathNode {
public:
    CategoryJunctionPathNode(std::shared_ptr<JunctionWord> junction_type) : JunctionPathNode(junction_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class JunctionIndexPathNode : public PathNode {
public:
    JunctionIndexPathNode(size_t index) : index_(index) {}
    virtual std::string to_string() const override;

    virtual size_t getIndex() const override {return index_;};
private:
    virtual int comp(const PathNode& a) const override {
        if (getIndex() == a.getIndex()) return  0;
        if (getIndex() <  a.getIndex()) return -1;
        else                    return  1;
    };

    const size_t index_;
};

class PhraseJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    PhraseJunctionIndexPathNode(size_t index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class AuxiliaryJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    AuxiliaryJunctionIndexPathNode(size_t index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class InflectionJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    InflectionJunctionIndexPathNode(size_t index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class CategoryJunctionIndexPathNode : public JunctionIndexPathNode {
public:
    CategoryJunctionIndexPathNode(size_t index) : JunctionIndexPathNode(index) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
};

class AuxiliaryPathNode : public PathNode {
public:
    AuxiliaryPathNode(std::shared_ptr<AuxiliaryWord> auxiliary_type) : auxiliary_type_(auxiliary_type) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;
    virtual const std::set<std::shared_ptr<Word>> getWords() const override {return {auxiliary_type_};};

    virtual std::shared_ptr<AuxiliaryWord> getAuxialiryType() const override {return auxiliary_type_;};
private:
    virtual int comp(const PathNode& a) const override {
        if (*getAuxialiryType() == *a.getAuxialiryType()) return  0;
        if (*getAuxialiryType() <  *a.getAuxialiryType()) return -1;
        else                                        return  1;
    };

    const std::shared_ptr<AuxiliaryWord> auxiliary_type_;
};

class InflectionPathNode : public PathNode {
public:
    InflectionPathNode(const std::set<std::shared_ptr<InflectionWord>>& inflections) : inflections_(inflections) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

    virtual const std::set<std::shared_ptr<Word>> getWords() const override {
        return std::set<std::shared_ptr<Word>>(inflections_.begin(), inflections_.end());
    };

    virtual const std::set<std::shared_ptr<InflectionWord>>& getInflections() const override {return inflections_;};

private:
    virtual int comp(const PathNode& a) const override {
        if (getInflections().size() != a.getInflections().size()) return (getInflections().size() < a.getInflections().size() ? -1 : 1);

        auto it_a = a.getInflections().begin();
        for (auto it_self = getInflections().begin(); it_self != getInflections().end(); it_self++) {
            if (**it_self != **it_a) return (**it_self < **it_a ? -1 : 1);
            it_a++;
        }
        // they are equal
        return 0;
    }

    const std::set<std::shared_ptr<InflectionWord>> inflections_;
};

/*
 * WordPathNode
 *
 * The terminal path to a category Word
 */
class WordPathNode : public PathNode {
public:
    WordPathNode(std::shared_ptr<CategoryWord> word) : word_(word) {}
    virtual bool accept_next(const PathNode& next) const override;
    virtual PathType getPathType() const override;
    virtual std::string to_string() const override;

    virtual const std::set<std::shared_ptr<Word>> getWords() const override {
        return {word_};
    };

    virtual const std::shared_ptr<CategoryWord> getCategoryWord() const override {return word_;};
private:
    virtual int comp(const PathNode& a) const override {
        if (*getCategoryWord() == *a.getCategoryWord()) return  0;
        if (*getCategoryWord() <  *a.getCategoryWord()) return -1;
        else                                            return  1;
    };

    const std::shared_ptr<CategoryWord> word_;
};

}


namespace std {
template<>
struct hash<ieml::structure::PathNode>
{
    size_t operator()(const ieml::structure::PathNode& s) const noexcept
    {
        return hash<string>{}(s.to_string());
    }
};
}