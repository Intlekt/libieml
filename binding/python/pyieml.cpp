#include <pybind11/pybind11.h>

#include "IemlParser.h"
#include "RecognitionError.h"



namespace py = pybind11;

std::vector<const ieml::parser::SyntaxError *> parse(const std::string& input) {
    ieml::parser::IEMLParser parser(input);
    parser.parse();
    return parser.getSyntaxErrors();
}


PYBIND11_MODULE(pyieml, m) {
    py::class_<ieml::parser::SyntaxError>(m, "SyntaxError")
        .def("to_json", &ieml::parser::SyntaxError::toJson);


    m.doc() = "Python wrapper for libieml";
    m.def("parse", &parse, "Parse an IEML string and return the syntax errors.");
}
