# spitest.py
# A brief demonstration of the Raspberry Pi SPI interface, using the Sparkfun
# Pi Wedge breakout board and a SparkFun Serial 7 Segment display:
# https://www.sparkfun.com/products/11629

import time
import spidev
import sys


# We only have SPI bus 0 available to us on the Pi
bus = 0

# Device is the chip select pin. Set to 0 or 1, depending on the connections
device = 1

# Enable SPI
spi = spidev.SpiDev()

# Open a connection to a specific bus and device (chip select pin)
spi.open(bus, device)

# Set SPI speed and mode
spi.max_speed_hz = 4000000
spi.mode = 0


def reset():
    # RESET
    msg = [0xFF]
    result = spi.xfer(msg)


def toggle_led():
    # TOGGLE LED
    msg = [0x31]
    result = spi.xfer(msg)
    # print(result)


def get_pitch():
    result = spi.xfer3([0x12, 0x12])
    return result   


def toggle_direction(forward):
    if forward:
        direction = 0x01
    else:
        direction = 0x00

    # SET PITCH
    msg = [0x11, direction, 0x00, 0x40]
    result = spi.xfer(msg)
    # print(result)


def loop():
    old_val = [-100, -200]
    forward = True
    reset()

    try:
        while True:

            val = get_pitch()
            print(val)

            if val == old_val[0] and val == old_val[1]:
                forward = not forward
                toggle_direction(forward)

            time.sleep(0.1)
            old_val[1] = old_val[0]
            old_val[0] = val

    except KeyboardInterrupt:
        print("Ctrl-C pressed!")
        # RESET
        msg = [0xFF]
        result = spi.xfer(msg)
        print(result)
        # RESET
        msg = [0xFF]
        result = spi.xfer(msg)
        print(result)
        sys.exit(0)


loop()
