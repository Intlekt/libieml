from ieml.iemlv1.ieml_database import IEMLDatabase
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("ieml_db_folder", type=str)

    args = parser.parse_args()


    db = IEMLDatabase(args.ieml_db_folder)
    dic = db.get_dictionary()

    with open('assets/dictionary_scripts.txt', 'w') as fp:
        for s in sorted(dic.scripts):
            fp.write(str(s) + '\n')
