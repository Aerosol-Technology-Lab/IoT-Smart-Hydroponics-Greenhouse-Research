import sys
import subprocess
import os
import tempfile
import pwd
from datetime import datetime

BOOT_FILE = 'iot-hydroponics'
NODE_PATH = '/home/charliew/.nvm/versions/node/v14.11.0/bin/node'
REPO_API_URL = 'https://api.github.com/repos/cmasterx/IoT-Smart-Hydroponics-Greenhouse-Research'
BOOT_FILE_PATH = os.path.join(pwd.getpwuid(os.getuid()).pw_dir, '.config/autostart/hydro-app.desktop')

def get_program_path(program_name: str):
    """ Gets the real path to the program

    Keyword arguments:
    program_name: name of the program to search

    returns: path of program where it is found, else None if cannot be found

    """
    res = subprocess.run(['which', program_name], stdout=subprocess.PIPE).stdout.decode('ascii').split()
    return res[0] if len(res) > 0 else None

def main(args):
    args_len = len(args)
    
    # checks if this script is ran before
    initialized = os.path.exists('./core_files/init')
    # creates file to remember that it has been ran before
    if not initialized:
        with open('./core_files/init', 'w+') as _:
            pass
        
    # pull repo
    pullRes = subprocess.run(['git', 'pull'], stdout=subprocess.PIPE).stdout.decode('ascii')
    requireRebuild = pullRes != 'Already up to date.\n'
    
    # check if raspberry pi
    uname = os.uname()
    isRaspi = True if uname.sysname == 'Linux' and uname.machine=='armv7l' else False

    # should install?
    if args_len and args[0] == 'install':
        if uname.sysname == 'Linux':
            response = input('This system is not dected to be a Raspberry Pi. Are you sure you want to install? [y/n]: ')
            install = response.lower() == 'y'
        else:
            print('System is not Linux. Cannot install boot time script')
    
    install = (install if 'install' in locals() else False) or isRaspi
    
    
    # install app to run on boot
    if install:
        if not os.path.exists(BOOT_FILE_PATH):
            # get python3 path
            python3_paths = subprocess.run(['which', 'python3'], stdout=subprocess.PIPE).stdout.decode('ascii').split()
            if len(python3_paths) < 1:
                print('Error: cannot find where python3 is installed.')
                exit(1)
            
            python3_app_path = python3_paths[1]
            
            # get this filepath
            corePath = os.path.realpath(__file__)
            
            # read boot script
            try:
                f = open('./core_files/iot-hydroponics', 'r')
                boot_script = f.read().format(python3_app_path=python3_app_path, corePath=corePath)
            except Exception as err:
                print(err)
                print('Error: Cannot find iot-hydroponics start script. Possible fix might be re-downloading this repository (or a hard reset pull)')
                exit(1)
            
            # saves boot script
            with open('./iot-hydroponics.sh', 'w+') as f:
                f.write(boot_script)
            subprocess.run('sudo chmod 755 ./iot-hydroponics.sh'.split())      # allows script to be executable
            

            # builds start script
            try:
                f = open('./core_files/hydro-app.desktop', 'r')
                start_script = f.read().format(script_path=os.path.realpath('./iot-hydroponics.sh'))
            except:
                print('Error: Cannot find auto-start config. Possible fix might be re-downloading this repository (or a hard reset pull)')
                exit(1)
            
            # adds script as start script
            with open(BOOT_FILE_PATH, 'w+') as f:
                f.write(start_script)
            

        else:
            print('Not installing, app is already installed.')
        
    else:
        pass
    
    if args_len == 0:
        # check node version
        node_path = get_program_path('node')
        if node_path is None:
            print('Error: Cannot find path to Node. Make sure Node version 14 is installed.')
            exit(1)
        
        NODE_VERSION_REF = 14
        current_node_version = subprocess.run([node_path, '-v'], stdout=subprocess.PIPE).stdout.decode('ascii').rstrip().split('.')
        current_node_version = int(current_node_version[0][1:])

        if current_node_version != NODE_VERSION_REF:
            print('Error: This app requires Node version v{}. You have Node version v{} installed.'.format(NODE_VERSION_REF, current_node_version))
            exit(1)

        
        yarn_path = subprocess.run('which yarn'.split(), stdout=subprocess.PIPE).stdout.decode('ascii').split()
        if len(yarn_path) < 1:
            print('Error: Cannot find path to yarn. Make sure yarn is installed.')
        yarn_path = yarn_path[0]
        
        #install dependencies
        if requireRebuild or not initialized:
            print('Updates found, acquiring dependencies and rebuilding electron\n')

            print('Acquiring dependencies...')
            # todo replace yarn with actual path
            subprocess.run(yarn_path, stdout=sys.stdout)
            
            # rebuild electron
            print('Rebuilding electron...')
            subprocess.run([yarn_path, 'elc-rebuild'])

        # open app
        print('Opening App...')
        subprocess.Popen([yarn_path, 'electron-dev'])
    elif args[0] == 'stop' or args[0] == 'kill':
        print('Killing all node and electron processes...\n')
        subprocess.run(['killall', 'node'])
        subprocess.run(['killall', 'electron'])
    elif args[0] == 'uninstall':
        force_uninstall = len(args) > 1 and args[1] == 'force'
        boot_file_exists = os.path.exists(BOOT_FILE_PATH)
        
        # check if bootfile exists. If found, proceed uninstall process
        if not boot_file_exists and not force_uninstall:
            print('Boot file is not installed. Nothing to uninstall.')
            exit(0)
        
        if boot_file_exists:
            print('Boot file detected.')
        else:
            print('Boot file not detected.')
        
        response = input('Proceed uninstall? [y/n]: ').lower()
        if response != 'y':
            print('\nQuitting')
            exit(0)

        try:    
            os.remove(BOOT_FILE_PATH)
        except Exception as err:
            with open('./error_log.txt', 'a+') as f:
                f.write('[{}] {}\n'.format(datetime.now(), str(err)))
                print('Error: Cannot delete the boot file. Error logged in {}'.format(f.name))

        try:
            os.remove('./iot-hydroponics.sh')
        except Exception as err:
            with open('./error_log.txt', 'a+') as f:
                f.write('[{}] {}\n'.format(datetime.now(), str(err)))
                print('Error: Cannot delete script file. Error logged in {}'.format(f.name))

if __name__ == '__main__':
    # change output to file
    
    # changes working directory to current path of python script
    script_path = os.path.realpath(__file__)[:-len(os.path.basename(__file__))]
    # sys.stdout = open(os.path.join(script_path, 'log.txt'),'w+')
    os.chdir(script_path)
    main(sys.argv[1:])
