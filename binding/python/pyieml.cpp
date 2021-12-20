#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <string>
#include <iostream>

#include "IemlParser.h"
#include "SyntaxError.h"
#include "relation/Composition.h"
#include "ParserJsonSerializer.h"



namespace py = pybind11;

std::vector<const ieml::parser::SyntaxError *> parse(const std::string& input) {
    ieml::parser::IEMLParser parser(input);
    parser.parse();
    return parser.getSyntaxErrors();
}


PYBIND11_MODULE(pyieml, m) {
    py::class_<ieml::parser::IEMLParser>(m, "Parser")
        .def(py::init<const std::string &>())
        .def(py::init<const std::vector<std::string> &, const std::vector<std::string> &>())
        .def("parse", &ieml::parser::IEMLParser::parse)
        .def("errors", &ieml::parser::IEMLParser::getSyntaxErrors, py::return_value_policy::reference)
        .def("to_json", [](const ieml::parser::IEMLParser &e) {
            return ieml::parser::parserToJson(e).dump();
        })
        .def("composition_graph_json", [](const ieml::parser::IEMLParser &e) {
            ieml::relation::CompositionNode::Register register_;
            auto wregister = e.getContext()->getWordRegister();
            auto cregister = e.getContext()->getCategoryRegister();
            auto graph = ieml::relation::buildCompositionRelationGraph(register_, cregister, wregister);
            return ieml::parser::binaryGraphToJson(graph, cregister, wregister).dump();
        });

    py::class_<ieml::parser::SyntaxError>(m, "SyntaxError")
        .def("__repr__", [](const ieml::parser::SyntaxError &e) {
            return "<SyntaxError \"" + e.to_string() + "\">";
        })
        .def("to_json", [](const ieml::parser::SyntaxError &e) {
            return ieml::parser::syntaxErrorToJson(e).dump();
        })
        .def_property_readonly("message", &ieml::parser::SyntaxError::getMessage)
        .def_property_readonly("range", &ieml::parser::SyntaxError::getCharRange, py::return_value_policy::reference);

    py::class_<ieml::parser::CharRange>(m, "CharRange")
        .def("__repr__", [](const ieml::parser::CharRange &e) {
            return "<CharRange \"" + e.to_string() + "\">";
        })
        .def("to_json", [](const ieml::parser::CharRange &e) {
            return ieml::parser::charRangeToJson(e).dump();
        })
        .def_property_readonly("line_start", &ieml::parser::CharRange::getLineStart)
        .def_property_readonly("line_end", &ieml::parser::CharRange::getLineEnd)
        .def_property_readonly("char_start", &ieml::parser::CharRange::getCharStart)
        .def_property_readonly("char_end", &ieml::parser::CharRange::getCharEnd);



    m.doc() = "Python wrapper for libieml";
}
