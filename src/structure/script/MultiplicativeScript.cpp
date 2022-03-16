#include "structure/script/MultiplicativeScript.h"
#include "structure/script/PrimitiveScript.h"
#include "structure/script/NullScript.h"
#include "structure/script/ScriptRegister.h"

#include <sstream>
#include <map>
#include <tuple>

using namespace ieml::structure;

std::string MultiplicativeScript::string_repr(const Children& children) {
    std::stringstream os;
    os << children[0]->to_string();
    os << children[1]->to_string();
    os << children[2]->to_string();
    os << LAYER_MARKS[children[0]->get_layer() + 1];

    const auto key = os.str();

    auto it = MultiplicativeScript::REMARKABLE_MULTIPLICATIONS_STRINGS.find(key);
    if (it != MultiplicativeScript::REMARKABLE_MULTIPLICATIONS_STRINGS.end()) return it->second;
    else return key;
}

std::u16string MultiplicativeScript::_canonical(const Children& children) {
    size_t size = children[0]->get_canonical().size() * 3;

    std::u16string res;
    res.reserve(size * 2); // u16 string

    res = res.append(children[0]->get_canonical());
    res = res.append(children[1]->get_canonical());
    res = res.append(children[2]->get_canonical());
    
    return res;
}

size_t MultiplicativeScript::_multiplicity(const Children& children) {
    const auto substance = children[0];
    const auto attribute = children[1];
    const auto mode      = children[2];

    return substance->get_multiplicity() * attribute->get_multiplicity() * mode->get_multiplicity();
}

Script::Set MultiplicativeScript::_build_singular_sequences(ScriptRegister& reg) const {
    if (get_multiplicity() == 1)
        return {this};

    Script::Set res;
    for (const auto& ss_s: children_[0]->singular_sequences()) {
        for (const auto& ss_a: children_[1]->singular_sequences()) {
            for (const auto& ss_m: children_[2]->singular_sequences()) {
                res.insert(reg.get_or_create_multiplication({ss_s, ss_a, ss_m}));
            }
        }
    }
    return res;
}

const Table<const Script*>* MultiplicativeScript::build_table_paradigm(ScriptRegister& reg) const {
    std::map<unsigned char, unsigned char> dim_map;
    // count the number of paradigmatic children
    // dim_map
    // keys : target position in the table (1st dim, 2nd dim and 3rd dim)
    // values : index of the variable in this paradigm (substance, attribute or mode)
    unsigned char last_dim = 0;
    for (unsigned char i = 0; i < 3; i++) {
        if (children_[i]->get_multiplicity() != 1) {
            dim_map.insert({last_dim, i});
            last_dim += 1;
        }
    }

    // if only one paradigmatic children, we recurse
    if (dim_map.size() == 1) {
        const auto pos_children = dim_map.find(0)->second;
        
        // Translate the child table to ours
        auto sub_table = reg.get_or_create_table(children_[pos_children]);

        auto tshape = sub_table->getShape();
        auto table_set_ndim = std::get<0>(tshape);
        auto nd_shapes = std::get<1>(tshape);

        if (table_set_ndim == 1) {
            // we have a multiplicative script as variable

            // translate the child ss as ours
            std::unordered_map<const Script*, const Script*> ss_map;
            for (const auto& ss: singular_sequences_) {
                auto e = dynamic_cast<const MultiplicativeScript*>(ss);
                ss_map.insert({e->children_[pos_children], ss});
            }

            std::vector<const Script*> values;
            std::vector<size_t> shape;
            std::vector<std::vector<const Script*>> headers;
            
            Children children(children_);

            for (unsigned char dim = 0; dim < sub_table->ndim(0); dim++) {
                shape.push_back(nd_shapes[0][dim]);

                std::vector<const Script*> headers_dim;
                for (const auto& h : sub_table->getHeaders(0, dim)) {
                    children[pos_children] = h;
                    headers_dim.push_back(reg.get_or_create_multiplication(children));
                }
                headers.push_back(headers_dim);
            }

            for (const auto& c : sub_table->getCells()) {
                values.push_back(ss_map.find(c)->second);
            }

            return TableNd<const Script*>::create(values, shape, headers, this);
        }

        // resulting nd_tables
        std::vector<const TableNd<const Script*>*> nd_tables;

        Children children(children_);
        // multiple set dim, so it is a TableSet
        for (auto child: sub_table->get_sub_tables()) {
            children[pos_children] = child->table_name();
            auto child_table_name = reg.get_or_create_multiplication(children);
            nd_tables.push_back(dynamic_cast<const TableNd<const Script*>*>(reg.get_or_create_table(child_table_name)));
        }

        return new TableSet<const Script*>(nd_tables, this);
    }

    // produce a table with ndim dimensions
    // 1st dim the rows
    // 2nd dim the columns
    // 3rd dim the tabs

    // fill-up remainings pos with the invariant
    std::set<unsigned char> invariants_pos{0, 1, 2};
    for (auto it : dim_map) invariants_pos.erase(it.second);
    std::map<unsigned char, unsigned char> invariant_map;

    for (auto pos: invariants_pos) {
        invariant_map.insert({last_dim, pos});
        last_dim += 1;
    }

    std::vector<const Script*> values;

    std::vector<size_t> shape;
    std::vector<std::vector<const Script*>> headers;

    std::vector<const Table<const Script*>*> tables;

    size_t total_size = 1;

    // std::map<unsigned char, unsigned char> dim_map;
    unsigned char dim;
    for (dim = 0; dim < dim_map.size(); dim++) {
        const auto table = reg.get_or_create_table(children_[dim_map[dim]]);
        tables.push_back(table);
        const size_t size = table->getCellsNumber();
        shape.push_back(size);
        total_size *= size;

        // build one header per shape per dim
        Children children(children_);
        std::vector<const Script*> headers_dim;
        for (const auto& c : table->getCells()) {
            children[dim_map[dim]] = c;
            headers_dim.push_back(reg.get_or_create_multiplication(children));
        }
        headers.push_back(headers_dim);

        // dim_map.insert({para_idx[dim], dim});
    }

    // map the para_idx with 1d, 2d or 3d. the remainings non variable are kept.
    std::vector<Children> values_children(total_size);
    size_t curr_ratio = 1;
    for (dim = 0; dim < 3; dim++) {
        if (dim >= dim_map.size()) {
            for (size_t i = 0; i < total_size; i ++)
                values_children[i][invariant_map[dim]] = children_[invariant_map[dim]];
        } else {
            const auto cells = tables[dim]->getCells();
            for (size_t i = 0; i < total_size; i ++) 
                values_children[i][dim_map[dim]] = cells[(i / curr_ratio) % cells.size()];
            
            curr_ratio *= cells.size();
        }
    }

    for (auto v_child: values_children)
        values.push_back(reg.get_or_create_multiplication(v_child));

    return TableNd<const Script*>::create(values, shape, headers, this);
}


const std::unordered_map<std::string, std::string> MultiplicativeScript::REMARKABLE_MULTIPLICATIONS_STRINGS = {
    {"U:U:.", "wo."},
    {"U:A:.", "wa."},
    {"U:S:.", "y."},
    {"U:B:.", "o."},
    {"U:T:.", "e."},

    {"A:U:.", "wu."},
    {"A:A:.", "we."},
    {"A:S:.", "u."},
    {"A:B:.", "a."},
    {"A:T:.", "i."},

    {"S:U:.", "j."},
    {"S:A:.", "g."},
    {"S:S:.", "s."},
    {"S:B:.", "b."},
    {"S:T:.", "t."},

    {"B:U:.", "h."},
    {"B:A:.", "c."},
    {"B:S:.", "k."},
    {"B:B:.", "m."},
    {"B:T:.", "n."},

    {"T:U:.", "p."},
    {"T:A:.", "x."},
    {"T:S:.", "d."},
    {"T:B:.", "f."},
    {"T:T:.", "l."}
};