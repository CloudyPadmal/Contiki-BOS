import os

import serial as s
import sys
import signal
import random
from time import sleep

import re
pattern = r"Temp:(\d+\.\d+)"

ser = None
try:
    sen = s.Serial('/dev/tty.usbmodemFFFFFFFEFFFF1', baudrate=115200)
    ser = s.Serial('/dev/tty.usbserial-MFV6KDL2', baudrate=115200)

except s.SerialException as e:
    print("\033[91mOperation failed for\033[1m", '/dev/tty.usbserial-MFV6KDL2'.split("/")[2], "\033[0m")
    exit()


def handler(signum, frame):
    ser.close()
    sen.close()
    print('\033[94m', os.getpid(), "\033[0mstopped and cleaned!")
    exit()


signal.signal(signal.SIGABRT, handler)

attempted = False
key = [1,2,5,3]

while True:
    try:
        sensor_reading = str(sen.readline())
        if 'Temp' in sensor_reading:
            match = re.search(pattern, sensor_reading)
            if match:
                temperature = float(match.group(1))
                formatted_temperature = "{:.2f}".format(temperature)
                transmit_data = str(formatted_temperature).replace('.', '')
                print('Temperature :', formatted_temperature)

                plain = [int(i) for i in transmit_data]
                cipher = [(num + k) % 10 for num, k in zip(plain, key)]

                temperature = ",".join(str(x) for x in cipher)
                ser.write((temperature + '\n').encode())
        sleep(0.2)
    except Exception as e:  # Sometimes serial is not setup, so we retry once
        print("Error: {0}".format(e))
        sen = s.Serial('/dev/tty.usbmodemFFFFFFFEFFFF1', baudrate=115200)
        ser = s.Serial('/dev/tty.usbserial-MFV6KDL2', baudrate=115200)
        if attempted:
            ser.close()
            exit()
        else:
            attempted = True

