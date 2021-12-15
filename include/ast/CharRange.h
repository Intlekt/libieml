#pragma once


namespace ieml::AST {

class CharRange {
public:
    CharRange(const std::string& file_id, 
              const size_t line_start, 
              const size_t line_end, 
              const size_t char_idx_line_start, 
              const size_t char_idx_line_end) : 
        file_id_(file_id),
        line_start_(line_start), 
        line_end_(line_end), 
        char_idx_line_start_(char_idx_line_start), 
        char_idx_line_end_(char_idx_line_end) {};

    std::string getFileId() const {return file_id_;}
    size_t getLineStart() const {return line_start_;}
    size_t getLineEnd() const {return line_end_;}
    size_t getCharStart() const {return char_idx_line_start_;}
    size_t getCharEnd() const {return char_idx_line_end_;}

    std::string to_string() const {
        std::ostringstream os;
        os << "[" << file_id_ << "]";
        if (line_start_ == line_end_) {
            os << "l." << line_start_;
            if (char_idx_line_start_ == char_idx_line_end_) 
                os << ":" << char_idx_line_start_;
            else
                os << ":" << char_idx_line_start_ << "-" << char_idx_line_end_;
        } else {
            os << "l." << line_start_ << ":" << char_idx_line_start_ 
              << "-l." << line_end_ << ":" << char_idx_line_end_ ;
        }

        return os.str();
    }


private:
    const std::string file_id_;
    const size_t line_start_;
    const size_t line_end_;
    const size_t char_idx_line_start_;
    const size_t char_idx_line_end_;
};

}