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

        
        j = json.loads(parser.to_json())
        with open(os.path.join(SRC_ROOT, "build/output.json"), 'w') as fp:
            json.dump(j, fp, indent=4)
        # print(json.dumps(json.loads(j), indent=4))


if __name__ == '__main__':
    unittest.main()
