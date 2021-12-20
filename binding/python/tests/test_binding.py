import unittest
import os
import json

SRC_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(__file__))))

import pyieml

class TestBinding(unittest.TestCase):

    def test_parser_to_json(self):
        
        with open(os.path.join(SRC_ROOT, "assets/examples/ieml-grammar.ieml")) as fp:
            s = fp.read()

        parser = pyieml.Parser(s)
        parser.parse()

        j = parser.to_json()

        # print(json.dumps(json.loads(j), indent=4))



    def test_graph_to_json(self):
        
        with open(os.path.join(SRC_ROOT, "assets/examples/ieml-grammar.ieml")) as fp:
            s = fp.read()

        parser = pyieml.Parser(s)
        parser.parse()

        j = parser.composition_graph_json()
        # print(j)
        # print(json.dumps(json.loads(j), indent=4))

    def test_multiple_files(self):
        parser = pyieml.Parser(["file_0", "file_1"], ["""@word "wa.". @word .""", """@word "e."."""])
        parser.parse()

        # print(parser.to_json())


if __name__ == '__main__':
    unittest.main()
