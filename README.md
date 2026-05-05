# Power Relay Control

This repo stores the firmware for a Relay Power Control Pico Device I made to toggle a power supply on and off through a CLI. 

Adafruit sells a Power Relay Module that can toggle three of the four outlets on and off using a GPIO port on the side.

![Picture of Power Relay taken from the Adafruit store.](https://cdn-shop.adafruit.com/970x728/2935-13.jpg)
*Picture of Power Relay taken from [the Adafruit store.](https://www.adafruit.com/product/2935)*

## Firmware

You need to have the Pico SDK C and `picotool` CLI installed and set up to compile this code. It should work for any RP2040-based device. I used an [RP2040-Zero.](https://www.waveshare.com/product/rp2040-zero.htm) for this project. 

### Compiling
TODO: Flesh out instructions to compile.
``` bash
cd build
cmake ..
make
```

### Flashing
TODO: Flesh out instructions to flash.

If your pico does not automount the boot drive already, follow these instructions. If it does automount, simply drag and drop the uf2 file into the boot drive. 
``` bash
# Create a mount point (only needed once)
sudo mkdir -p /mnt/pico 

# Put the Pico into BOOTSEL mode:
# Hold the BOOTSEL button while plugging it in via USB

# Confirm the device shows up (look for a new drive, e.g. /dev/sda1)
lsblk

# Or use picotool
picotool reboot -fu  # forces USB boot mode (if already connected)

cd build

# Mount the Pico (replace sdXY with your actual device, e.g. sda1)
sudo mount /dev/sdXY /mnt/pico  

# Copy the firmware
sudo cp power-relay-control.uf2 /mnt/pico

# Unmount (this safely ejects and triggers flashing)
sudo umount /mnt/pico
```

## CLI

I made a python script that will connect to the device and let you toggle the state or see the current status. 

TODO: Still in development.

### Dependancies

You will need `CRC` and `pyserial`. The library also uses `struct` and `argparse` but they should be built in python libraries.

``` bash
pip install pyserial crc
```

### Example Usuage

## Future Add ons

I am considering designing a simple PCB to reduce the amount of soldering needed. Right now, I soldered two long wires to the RP2040-Zero.