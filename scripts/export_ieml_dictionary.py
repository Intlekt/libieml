import argparse
from ieml.iemlv1.ieml_database import IEMLDatabase
from ieml.iemlv1.constants import AUXILIARY_CLASS, VERB_CLASS
import numpy as np
import re
import sys


identifier_regex = re.compile("""^[a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ][ 0-9a-zA-ZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ\-']*$""")


inflections = {
    "E:O:.O:.-": 'verb',

    "E:O:O:.": 'verb',
    "E:M:O:.": 'verb',
    "E:.b.O:O:.-": 'verb',
    "E:.b.O:M:.-": 'verb',
    "E:.-O:O:.-t.o.-'": 'verb',
    "E:F:.-t.o.-'": 'verb',
    "E:.-'O:O:.-M:.-'t.o.-',": 'verb',

    "E:.O:O:.-": 'noun',
    "E:M:.-d.u.-'": 'noun',
    "E:.-n.E:U:.+F:.-'": 'noun',
    "E:I:.-t.u.-'": 'noun',
    "E:S:.j.-'M:O:.-O:.-',": 'noun',
    "E:.-',b.-S:.U:.-'y.-'O:.-',_": 'noun'
}

auxiliaries = {
    "E:M:.d.+M:O:.-": 4,
    "E:S:+B:M:.": 5,
    "E:M:.-O:.-t.o.-'": 5,
    "E:.O:O:.O:.-t.o.-'": 5,
    "E:T:M:.": 6,
    "E:.-F:.M:M:.-l.-'": 6,
    "E:.-'h.o.d.-U:.-'M:O:.-',": 7,
    "E:.-b.O:M:.-'E:.-b.O:M:.-',": 7,
    "E:O:M:.": 8,
    "E:.A:.O:M:.-": 8,
    "E:.O:.M:O:.-": 8,
    "E:U:S:+T:.-M:M:.o.-'": 8
}

junctions = {
    "E:M:.-k.u.-'",
    "E:.-O:O:.-k.u.-'",
    "E:.-O:M:.-k.u.-'",
    "E:.-M:M:.u.-k.u.-'",
    "E:.-M:M:.a.-k.u.-'",
    "E:.-S:.m.-l.-'O:O:.-'k.u.-',"
}


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument("ieml_db_folder", type=str)

    args = parser.parse_args()


    db = IEMLDatabase(args.ieml_db_folder)
    dic = db.get_dictionary()

    desc = db.get_descriptors()
    def _translations(m):
        translations = ""
        for l, vv in desc.get_descriptor(m)['translations'].items():
            skipped = []
            tlang = ""
            for v in vv:
                repl_v = v.replace('(', '-').replace(')', '-')
                if identifier_regex.match(repl_v):
                    tlang += f"""\t{l}:{repl_v}\n"""
                else:
                    skipped.append(v)
            if skipped:
                print(f"// SKIPPED TRANSLATIONS '{l}' {skipped}")

            if not tlang:
                print(f"// NO '{l}' TRANSLATIONS !")
            translations += tlang
        return translations

    tail = ''
    for root in dic.scripts[dic.roots_idx == 1]:
        decl_type = "component"
        extr_args = ''

        if root.grammatical_class == AUXILIARY_CLASS:
            if root in inflections:
                decl_type = "inflection"
                extr_args = f"\t{inflections[root].upper()}\n"
            elif root in auxiliaries:
                decl_type = "auxiliary"
                extr_args = f"\t{auxiliaries[root]}\n"
            elif root in junctions:
                decl_type = 'junction'
            else:
                msg = f"SKIPPING AUX. PARADIGM: {root}, not a inflection not an auxiliary not a junction\n"
                print(msg, end='', file=sys.stderr)
                tail += "// " + msg
                continue
        else: continue

        print(f"""// {root}""")
        # print(f"""// {desc.get_descriptor(root)['translations']}""")
        
        for m in root.singular_sequences:
            print(f"""@{decl_type} \n{_translations(m)}{extr_args}\t"{m}".\n""")
        print('\n')

    for root in dic.scripts[dic.roots_idx == 1]:
        if root.grammatical_class == AUXILIARY_CLASS:
            continue

        print(f"""// {root}""")
        # print(f"""// {desc.get_descriptor(root)['translations']}""")
        for m in root.singular_sequences:
            print(f"""@word "{m}".""")
            print(f"""@component \n{_translations(m)}\t(0 ~{"verb" if m.grammatical_class == VERB_CLASS else "noun"} #"{m}").\n""")
        print('\n')

    print(tail)