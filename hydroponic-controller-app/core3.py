from enum import auto
import sys
import subprocess
import os
import tempfile
import pwd
from datetime import date, datetime
import requests
import json
import re
from pathlib import Path

REPO_URL = 'https://raw.githubusercontent.com/cmasterx/IoT-Smart-Hydroponics-Greenhouse-Research/{branch}/{project}/{path}'
PROJECT_NAME = 'hydroponic-controller-app'

TARGET_BRANCH = 'development'

def request_repo_get(path:str, branch:str='release', project:str=PROJECT_NAME, repo_url:str=REPO_URL) -> requests.Response:
    final_url = repo_url.format(project=project, branch=branch, path=path)
    print(final_url)
    return requests.get(final_url)

def get_app_version(version):
    nums = re.sub("[^0-9]", " ", version).split(' ')
    res = []
    for s in nums:
        try:
            foo = int(s)
            res.append(foo)
        except:
            pass

    # this function needs to return an array of numbers with at least length of two
    while len(res) < 2:
        res.append(0)

    return res
            

def update(branch=None, force=False):

    subprocess.run(['git', 'pull'])
    subprocess.run(['yarn'])
        

def main(args):

    update()
    # subprocess.run('yarn elc-rebuild'.split())
    subprocess.Popen(['yarn', 'electron-dev', '&'], stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
    
    # check if installed. If not, then install
    autostart_dir = os.path.join(Path.home(), '.config/autostart', 'hydro-starter.desktop')
    if (not os.path.exists(autostart_dir)
            or not os.path.isfile(autostart_dir)
        ):

        with open('hydro-core/hydro-starter.desktop', 'r') as f:
            file_data = f.read()

        if file_data is not None and len(file_data) > 0:
            py_exec_path = sys.executable
            script_path = os.path.realpath(__file__)
            args = ''
            with open(autostart_dir, 'w') as f:
                f.write(file_data.format(exec_path=py_exec_path, script_path=script_path, args=args))
    

if '__main__' in __name__:

    # changes working directory to path of the file
    script_path = os.path.realpath(__file__)[:-len(os.path.basename(__file__))]
    os.chdir(script_path)

    main(sys.argv[1:])