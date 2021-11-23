#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <string>
#include <iostream>

#include "IemlParser.h"
#include "RecognitionError.h"



namespace py = pybind11;

std::vector<const ieml::parser::SyntaxError *> parse(const std::string& input) {
    ieml::parser::IEMLParser parser(input);
    parser.parse();
    return parser.getSyntaxErrors();
}


PYBIND11_MODULE(pyieml, m) {
    py::class_<ieml::parser::IEMLParser>(m, "Parser")
        .def(py::init<const std::string &>())
        .def("parse", &ieml::parser::IEMLParser::parse)
        .def("errors", &ieml::parser::IEMLParser::getSyntaxErrors, py::return_value_policy::reference)
        .def("to_json", [](const ieml::parser::IEMLParser &e) {
            return e.toJson().dump();
        });

    py::class_<ieml::parser::SyntaxError>(m, "SyntaxError")
        .def("__repr__", [](const ieml::parser::SyntaxError &e) {
            return "<SyntaxError \"" + e.to_string() + "\">";
        })
        .def("to_json", [](const ieml::parser::SyntaxError &e) {
            return e.toJson().dump();
        });


    m.doc() = "Python wrapper for libieml";
}
