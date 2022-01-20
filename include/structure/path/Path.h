#pragma once

#include "structure/Constants.h"
#include "utils.h"
#include "structure/path/PathNode.h"
#include "structure/WordRegister.h"

#include <string>
#include <memory>
#include <set>


namespace ieml::structure {


class Path {
/**
 * @brief Path nodes in sequence
 * 
 * /{role number} /{junction[]}? /{auxiliary}? /{junction[]}? /{inflection[]}? /{junction[]}? /{category} ...
 * 
 * 
 * 1) Checks if nodes are valid vector of PathNode ?
 *      - valid transitions e.g. {role number} -> {junction[]}
 *      
 * 
 * 
 */

public:
    Path(std::shared_ptr<PathNode> node, std::shared_ptr<Path> next): node_(std::move(node)), next_(std::move(next)) {}
    Path(std::shared_ptr<PathNode> node): node_(std::move(node)), next_(nullptr) {}
    void check();
    std::string to_string() const;
    std::shared_ptr<PathNode> getNode() const {return node_;}
    std::shared_ptr<Path> getNext() const {return next_;}

    static std::shared_ptr<Path> from_string(const std::string& s, const WordRegister& ctx);
    
    bool operator==(const Path& r) const {
        return compare(r) == 0;
    };
    bool operator!=(const Path& r) const {return compare(r) != 0;};

    bool operator< (const Path& r) const {return compare(r) <  0;};
    bool operator> (const Path& r) const {return compare(r) >  0;};
    bool operator<=(const Path& r) const {return compare(r) <= 0;};
    bool operator>=(const Path& r) const {return compare(r) >= 0;};

private:
    const std::shared_ptr<PathNode> node_;
    const std::shared_ptr<Path> next_;

    int compare(const Path& r) const {
        if (*node_ == *r.node_) {
            if (next_ == nullptr && r.next_ == nullptr) return 0;
            if (next_ == nullptr) return -1;
            if (r.next_ == nullptr) return 1;
            return next_->compare(*r.next_);
        } else return *node_ <  *r.node_ ? -1 : 1;
    }
};

}

