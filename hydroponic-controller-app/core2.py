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

    cfg = None
    with open('hydro-cfg/config.json', 'r') as f:
        cfg = json.load(f)

    if branch is None:
        branch = cfg['source-branch']

    print('The branch is {}'.format(branch))
    print(cfg['branches'])
    r = requests.get(REPO_URL.format(project=PROJECT_NAME, branch=cfg['branches'][branch], path='hydro-core/config.json'))
    print('The data in the request is: {}'.format(r.text))
    repo_info = r.json()
    r.close()
    repo_version = get_app_version(repo_info['app-version'])
    this_version = get_app_version(cfg['app-version'])

    if (force
        or repo_version[0] > this_version[0]
        or (repo_version[0] == this_version[0] and repo_version[1] > this_version[1])):
        
        # do update
        with request_repo_get('binaries/hydro-app.AppImage') as r:
            r.raise_for_status()
            with open('hydro-app.AppImage', 'w') as f:
                for chunk in r.iter_content(chunk=8192):
                    f.write(chunk)
            
        cfg['version'] = repo_info['version']
        with open('/hydro-cfg/config.json', 'w') as f:
            json.dump(cfg, f)

        return True
    else:
        # nothing
        return False
        
    

def main(args):

    if not os.path.exists('hydro-cfg/init'):
        print('Hydroponics Desktop App is not installed. Let\'s fix that! Installing...')

        if not os.path.exists('hydro-cfg') or not os.path.isdir('hydro-cfg'):
            os.mkdir('hydro-cfg')

        print('-> Downloading a couple files...')


        try:
            request_url = REPO_URL.format(project=PROJECT_NAME, branch='dev/hydroponic-controller-app', path='hydro-core/config.json')
            print(request_url)
            r = requests.get(request_url)
            print('I am here')
            with open('hydro-cfg/config.json', 'w') as f:
                f.write(r.text)
        except:
            print('Uh oh... I can\'t connect to the internet at the moment. Please check if there is an internet connection.')
            exit(1)

        update(force=True)
        
        with request_repo_get('hydro-core/hydro-starter.desktop') as r:
            with open(os.path.join(Path.home(), '.config', 'autostart', 'hydro-starter.desktop'), 'w') as f:
                f.write(r.text.format(exec_path=sys.executable, script_path=os.path.realpath(__file__), args='start boot-start'))
        
        with open('hydro-cfg/init', 'w') as f:
            f.write('Installed on {dt}\n', datetime.now().isoformat())
    
    pass

if '__main__' in __name__:

    # changes working directory to path of the file
    script_path = os.path.realpath(__file__)[:-len(os.path.basename(__file__))]
    os.chdir(script_path)

    main(sys.argv[1:])