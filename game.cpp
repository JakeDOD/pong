#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <bcm2835.h>
#include <wiringPi.h>

#include "pong.h"

#define BCM2835_GPIO_FSEL_INPT 0 
#define BCM2835_GPIO_FSEL_ALT0 4 

// Initialize the i2c-0 bus
int initI2c0() {

	if (!bcm2835_init()) {
		return -1;
	}

	bcm2835_gpio_fsel(0, BCM2835_GPIO_FSEL_INPT); 
	bcm2835_gpio_fsel(1, BCM2835_GPIO_FSEL_INPT); 
	bcm2835_gpio_fsel(28, BCM2835_GPIO_FSEL_INPT); 
	bcm2835_gpio_fsel(29, BCM2835_GPIO_FSEL_INPT); 

	bcm2835_gpio_fsel(28, BCM2835_GPIO_FSEL_ALT0); 
	bcm2835_gpio_set_pud(28, BCM2835_GPIO_PUD_UP); 
	bcm2835_gpio_fsel(29, BCM2835_GPIO_FSEL_ALT0); 
	bcm2835_gpio_set_pud(29, BCM2835_GPIO_PUD_UP);

	return 0;
}

int main(int argc, char const *argv[]) {

	// Initialize the i2c-0 bus
	if(initI2c0() != 0) {
		fprintf(stderr, "Failed to initialize i2c-0! Check that the bcm2835 library has been installed.\n");
		return -1;
	}

	// Make sure that linux has enough time to do the init
	usleep(1000);
	
	// Set up gpio library
	if (wiringPiSetupGpio() < 0) {
		fprintf(stderr, "Unable to setup GPIO using WiringPi: %s\n", strerror(errno));
		fprintf(stderr, "Check that the WiringPi library has been installed.\n");
		return -1;
	}

	// The Pong game
	CPong pong;

	if (pong.Init() < 0) {
		fprintf(stderr, "There was an error while initializing the game!\n");
		return -1;
	}

	pong.PlayIntro();

	// The game loop
	while (true) {
		
		if (pong.Animate() < 0) {
			fprintf(stderr, "There was an error communicating with the controllers!\n");
			return -1;
		}

		pong.DrawFrame();

		usleep(100 * 1000);
	}

	return 0;
}