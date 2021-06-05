import sys
import subprocess
import os
import tempfile
import pwd

BOOT_PATH = '/etc/init.d'
BOOT_FILE = 'iot-hydroponics'

# todo uninstall arguments

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
        boot_file_path = os.path.join(pwd.getpwuid(os.getuid()).pw_dir, '.config/autostart/hydro-app.desktop')

        if not os.path.exists(boot_file_path):
            # get python3 path
            python3_paths = subprocess.run(['whereis', 'python3'], stdout=subprocess.PIPE).stdout.decode('ascii').split()
            if len(python3_paths) <= 1:
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
            with open(boot_file_path, 'w+') as f:
                f.write(start_script)
            

        else:
            print('Not installing, app is already installed.')
        
            # Register script to run at startup
            subprocess.run('sudo update-rc.d {} defaults'.format(BOOT_FILE).split())
    else:
        pass
    
    if args_len == 0:
        #install dependencies
        if requireRebuild or not initialized:
            print('Updates found, acquiring dependencies and rebuilding electron\n')

            print('Acquiring dependencies...')
            subprocess.run('yarn')
            
            # rebuild electron
            print('Rebuilding electron...')
            subprocess.run(['yarn', 'elc-rebuild'])

        # open app
        print('Opening App...')
        subprocess.Popen(['yarn', 'electron-dev'])
    else:
        if args[0] == 'stop' or args[0] == 'kill':
            print('Killing all node and electron processes...\n')
            subprocess.run(['killall', 'node'])
            subprocess.run(['killall', 'electron'])
        elif args[0] == 'uninstall':
            # check if bootfile exists. If found, proceed uninstall process
            if not os.path.exists(os.path.join(BOOT_PATH, BOOT_FILE)):
                print('Boot file is not installed. Nothing to uninstall!')
                exit(0)
            
            print('Boot file is installed')
            response = input('Proceed uninstall? [y/n]: ').lower()
            if response != 'y':
                print('\nQuitting')
                exit(0)
                
            # unregister, then clean up
            subprocess.run('sudo update-rc.d -f  {} remove'.format(BOOT_FILE).split())
            subprocess.run('sudo rm {}'.format(os.path.join(BOOT_PATH, BOOT_FILE)).split())

if __name__ == '__main__':
    # changes working directory to current path of python script
    script_path = os.path.realpath(__file__)[:-len(os.path.basename(__file__))]
    os.chdir(script_path)
    
    main(sys.argv[1:])
