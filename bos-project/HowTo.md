# BOS Project

The project structure consists of an ST sensor-tile connected to a host computer.
The host computer also connects with a radio module running Contiki-ng OS.

### Sensing and Transmission
The sensor-tile records sensor readings in plain-text format (at the moment).
It then transmits them over UART to the host computer.
The host computer reads the sensor readings using the `Encoder.py` script.

`Encoder.py` is responsible for several tasks.

 - Reading sensor values from the sensor-tile.
 - Preprocess the lengthy response from the sensor-tile to filter out the _temperature_ readings.
 - Encrypt the _temperature_  reading using a one-pad key.
 - Pass over the encrypted data to the radio module over serial interface.

When running the `Encoder.py` script, follow the command given below.

> `Encoder.py`

Running this command will output the generated one-pad key and the raw and encrypted temperature reading.
Copy the key to use in the decryption process.

### Reception
On another host computer, a radio module listens to the channel for the encrypted temperature readings.
This operation is handled by the `Decoder.py` script.

`Decoder.py` is responsible for several tasks.

- Reading the encrypted data from the radio module over serial interface.
- Decrypt the readings using the provided one-pad key.
- Print out the raw temperature reading.

When running the `Decoder.py` script, follow the command given below.

> `Decoder.py`

Running this command will output the raw temperature readings captured from the sensor-tile in a possibly a different host computer. 
