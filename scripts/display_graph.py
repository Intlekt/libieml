import pyieml
import json
import os
import graphviz
import tempfile

def to_graph(nodes, edges):
    dot = graphviz.Digraph('composition graph')

    for n in nodes:
        dot.node(str(n['id']), n['names']['EN'])


    for e in edges:
        dot.edge(str(e['subject']), str(e['object']), label=e['class'])

    return dot



if __name__ == '__main__':
    with open(os.path.dirname(__file__) + "/../assets/examples/ieml-grammar.ieml", 'r', encoding='utf8') as fp:
        parser = pyieml.Parser(fp.read())

    parser.parse()
    loaded = json.loads(parser.composition_graph_json())

    # graph = to_graph(loaded['nodes'], loaded['relations'])

    # graph.view(tempfile.mktemp('.gv'))  

    with open('graph.json', 'w', encoding='utf8') as fp:
        json.dump(loaded, fp, indent=True)