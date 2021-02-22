import sys
import subprocess
import os
import tempfile

BOOT_PATH = '/etc/init.d'
BOOT_FILE = 'iot-hydroponics'

# todo uninstall arguments

def main(args):
    args_len = len(args)
    
    # pull repo
    subprocess.run(['git', 'pull'])
    
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
        boot_file_path = os.path.join(BOOT_PATH, BOOT_FILE)
        tmp_dir_path = os.path.join(tempfile.gettempdir(), 'iot-hydroponics')
        tmp_file_path = os.path.join(tmp_dir_path, 'script.sh')

        if not os.path.exists(boot_file_path):
            # read script template
            with open('./core_files/iot-hydroponics', 'r+') as file:
                boot_script = file.read().format(os.path.join(os.getcwd(), 'core.py'))
            
            
            # write to tmp directory
            if not os.path.exists(tmp_dir_path):
                os.makedirs(tmp_dir_path)
            with open(tmp_file_path, 'w+') as file:
                file.write(boot_script)

            # copies script to boot startup path
            subprocess.run(['sudo', 'cp', tmp_file_path, boot_file_path])
            
            # makes script executable
            subprocess.run('sudo chmod 755 {}'.format(boot_file_path).split())
            
            # Register script to run at startup
            subprocess.run('sudo update-rc.d {} defaults'.format(BOOT_FILE).split())
    else:
        pass
    
    if args_len == 0:
        #install dependencies
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
                
            subprocess.run('sudo update-rc.d -f  {} remove'.format(BOOT_FILE).split())
            subprocess.run('sudo rm {}'.format(os.path.join(BOOT_PATH, BOOT_FILE)).split())

if __name__ == '__main__':
    # changes working directory to current path of python script
    script_path = os.path.realpath(__file__)[:-len(os.path.basename(__file__))]
    os.chdir(script_path)
    
    main(sys.argv[1:])
