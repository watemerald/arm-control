import serial
import time
import json
import logging
import logging.config
import os

config_path = "logging.json"

if os.path.exists(config_path):
    with open(config_path, 'rt') as f:
        config = json.load(f)
    logging.config.dictConfig(config)
else:
    logging.basicConfig(level=default_level)

logger = logging.getLogger(__name__)

def back_and_forth(ser, a=10, b=180):
    while True:
        ser.write(bytes(str(a) + 're', 'utf-8'))
        status = ser.readline()
        logger.info(status)
        ser.write(bytes(str(b) + 're', 'utf-8'))
        status = ser.readline()
        logger.info(status)

def main():
    try:
        with open("config.json") as f:
            text = f.read()
            config = json.loads(text)
    except FileNotFoundError as e:
        print("No config file found, default is used")
        config = {"port": "/dev/ttyUSB0"}

    # logging.basicConfig(filename="status.log", level=logging.INFO)

    logger.info("Started control function on port{}".format(config["port"]))
    ser = serial.Serial(config["port"])

    back_and_forth(ser)

if __name__ == '__main__':
    main()
