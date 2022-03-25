#include "structure/link/function/WordDomain.h"
#include "structure/WordRegister.h"


using namespace ieml::structure;



std::unordered_set<std::string> WordDomain::getVariableNameSet() const {
    std::unordered_set<std::string> res;
    for (const auto& it : *this) {
        res.insert(it.first);
    }
    return res;
}

std::vector<WordCondition::VariableValuation> WordDomain::getValues(WordRegister& wreg) const {
    size_t size = 1;
    std::unordered_map<std::string, std::vector<Word::Ptr>> sets;
    for (const auto& it: *this) {
        std::vector<Word::Ptr> line;
        for (auto ss: it.second->singular_sequences()) {
            if (wreg.word_is_defined(ss))
                line.push_back(wreg.get_word_from_script(ss));
        }
        if (line.size() == 0) {
            // one product operand is empty -> empty
            return {};
        }

        size *= line.size();
        sets.insert({it.first, line});
    }

    std::vector<std::unordered_map<std::string, Word::Ptr>> bins(size);

    size_t curr_ratio = 1;
    for (const auto& it: *this) {
        const auto& ss_v = sets[it.first];
        for (size_t i = 0; i < size; ++i) {
            const auto& item = ss_v[(i / curr_ratio) % ss_v.size()];
            bins[i].insert({it.first, item});
        }

        curr_ratio *= ss_v.size();
    }

    return bins;
}