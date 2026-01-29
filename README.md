# Power Relay Control

This repo stores the firmware for a Relay Power Control Pico Device I made to toggle a power supply on and off through a CLI. 

Adafruit sells a Power Relay Module that can toggle three of the four outlets on and off using a GPIO port on the side.

![Picture of Power Relay taken from the Adafruit store.](https://cdn-shop.adafruit.com/970x728/2935-13.jpg)
*Picture of Power Relay taken from [the Adafruit store.](https://www.adafruit.com/product/2935)*

## Firmware

You need to have the Pico SDK C and `picotool` CLI installed and set up to compile this code. It should work for any RP2040-based device. I used an [RP2040-Zero.](https://www.waveshare.com/product/rp2040-zero.htm) for this project. 

TODO: Add instructions to compile.

## CLI

TODO: Still in development.

## Future Add ons

I am considering designing a simple PCB to reduce the amount of soldering needed. Right now, I soldered two long wires to the RP2040-Zero.