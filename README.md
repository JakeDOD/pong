# Pong

A simple implementation of Pong using two Wii Nunchucks, a 16x12 LED matrix display, and a 7-segment display using a Raspberry Pi Model B.

# Setup

To play our game of Pong, you will need the following components:
- 2 Wii Nunchucks
- 2 [Wii Nunchuck break out connectors](https://www.adafruit.com/products/345 "Nunchucky")
- 1 [16x24 LED Matrix Panel](https://www.adafruit.com/products/555 "Adafruit Website")
- 1 [7-Segment i<sup>2</sup>c Display](http://www.adafruit.com/products/880 "Adafruit Website")

You will also need the BCM285 library as well as the WiringPi library which can be found at the following links:
- [BCM2835 Library Installation Instructions](http://www.airspayce.com/mikem/bcm2835/)
- [WiringPi Library Installation Instructions](http://wiringpi.com/download-and-install/)

To play Pong one Wii Nunchuck is connected to the i<sup>2</sup>c pins of the P5 header on the RPi Model B and instructions on how to do so can be found using the following links:
- [Connecting to the P5 header](http://raspi.tv/2013/the-leaning-header-of-pi5a-how-best-to-solder-a-header-on-p5) (We recommend that the pins are connected at an angle)
- [The P5 Header Pinout](http://elinux.org/RPi_Low-level_peripherals#P5_header)

The 7-segment display is connected to i2c-1 along with the other Wii Nunchuck. The LED Matrix is connected with the following configuration:

LED Matrix Connection | RPi Connection
--------------------- | --------------
DATA                  | 24
WRITE                 | 25
CHIP SELECT           | 18

## Installation
To install Pong, download the compile and install the source:
```
git clone https://github.com/JakeDOD/pong.git
cd pong
sudo make install
```
Pong will then run every time the Raspberry Pi reboots, so an HDMI display is not required to be connected. To play pong without installing the game, replace ```make install``` with ```make``` and then type ```sudo ./pong``` to run the game.

To check for updates, use: (No need to reinstall)
```
make update
```

# How To Play
The game will start automatically, just use ```Up``` and ```Down``` on the joysticks to move the paddles and ```C``` to pause the game. Press ```C + Z``` on either nunchuck to restart the game.

### Bonus Level (just kidding)
Start the game using ```sudo make level2``` to start the game in level 2 mode. **NOTE: Nothing is guaranteed to work and this is not actually part of the game, just have fun!**