import os

import serial as s
import sys
import signal
import re

ser = None
try:
    ser = s.Serial('/dev/tty.usbserial-MFV6KF3B', baudrate=115200)
except s.SerialException as e:
    print("\033[91mOperation failed for\033[1m", '/dev/tty.usbserial-MFV6KF3B'.split("/")[2], "\033[0m")
    exit()


def handler(signum, frame):
    ser.close()
    print('\033[94m', os.getpid(), "\033[0mstopped and cleaned!")
    exit()


signal.signal(signal.SIGABRT, handler)

attempted = False
key = [1,2,5,3]
while True:
    try:
        reading = ser.readline()
        reading = reading.decode('utf-8')
        match = re.search(r':\s*(\d+)\s+(\d+)\s+(\d+)\s+(\d+)', reading)
        if match:
            temperature = [int(match.group(1)), int(match.group(2)), int(match.group(3)), int(match.group(4))]
            plain = [(num - k) % 10 for num, k in zip(temperature, key)]
            temperature = "".join(str(x) for x in plain)
            temperature = temperature[:2] + "." + temperature[2:]
            print('Temperature:', temperature)

    except Exception as e:  # Sometimes serial is not setup, so we retry once
        print("Error: {0}".format(e))
        ser = s.Serial('/dev/tty.usbserial-MFV6KF3B', baudrate=115200)
        if attempted:
            ser.close()
            exit()
        else:
            attempted = True

