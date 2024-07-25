import os

import serial as s
import sys
import re
pattern = r"Temp:(\d+\.\d+)"

sen = s.Serial(sys.argv[1], baudrate=115200)

while True:
    try:
        sensor_reading = str(sen.readline())
        if 'Temp' in sensor_reading:
            match = re.search(pattern, sensor_reading)
            if match:
                temperature = match.group(1)
    except:
        sen.close()