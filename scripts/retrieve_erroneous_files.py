import subprocess
import os
import json
import argparse
import hashlib

ROOT_LIBIEML = os.path.dirname(os.path.dirname(__file__))

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--output_folder", default=os.path.join(ROOT_LIBIEML, 'assets/erroneous_files'))
    parser.add_argument("--host", default='ec2-user@dev2.intlekt.io')
    parser.add_argument("--credential", default=os.path.join(os.path.dirname(ROOT_LIBIEML), 'credentials/intlekt-2.pem'))

    args = parser.parse_args()

    os.makedirs(args.output_folder, exist_ok=True)

    stdout, stderr = subprocess.Popen(f"ssh -i {args.credential} {args.host} ./ieml-project/retrieve_errors.sh", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()

    files = json.loads(stdout.decode('utf8'))

    for f in files:
        content = f['content']

        hash = hashlib.sha224(content.encode('utf8')).hexdigest()

        with open(os.path.join(args.output_folder, f"{hash}.ieml"), 'w') as fp:
            fp.write(content) 


