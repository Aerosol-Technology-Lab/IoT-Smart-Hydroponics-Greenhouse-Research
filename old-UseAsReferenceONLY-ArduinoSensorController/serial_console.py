import serial
import json
import time

from serial.serialutil import Timeout

def read_until(ser, terminator='\n'):
    buffer = bytearray()
    terminator_length = len(terminator)
    terminator = terminator.encode()
    
    while len(buffer) < terminator_length or buffer[-terminator_length:] != terminator:
        buffer += ser.read()

    return buffer


def main():
    print('Starting')
    arduino = serial.Serial('/dev/ttyUSB0', 57600, timeout=1)
    arduino.flush()
    print('Ready!')

    # while True:
        # data = read_until(arduino, '\r\n')
        # print('> ' + data.decode('utf-8'))

    while (True):
       toSend = input()

       dataSender = {"com": toSend}
       printStatement = json.dumps(dataSender) + '\0'
       arduino.write(printStatement.encode())
       print('$ [{}]'.format(printStatement))
       
       time.sleep(0.1)
       received = read_until(arduino, '\0')
       print('> {}'.format(received.decode('utf-8')))
       print('BIN: {}', received)

       time.sleep(1)
       

if '__main__' in __name__:
    main()
