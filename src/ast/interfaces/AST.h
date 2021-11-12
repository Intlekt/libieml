#pragma once


namespace ieml::AST {

class CharRange {

public:
    CharRange(size_t line_start, size_t line_end, size_t char_idx_line_start, size_t char_idx_line_end) : 
        line_start_(line_start), 
        line_end_(line_end), 
        char_idx_line_start_(char_idx_line_start), 
        char_idx_line_end_(char_idx_line_end) {};

private:
    const size_t line_start_;
    const size_t line_end_;
    const size_t char_idx_line_start_;
    const size_t char_idx_line_end_;
};

class AST {
public:
    AST(CharRange char_range) : char_range_(char_range) {};

    virtual std::string to_string() const = 0;

private:
    const CharRange char_range_;


};
}
