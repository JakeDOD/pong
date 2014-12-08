#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "wiringPi.h"
#include "pong.h"

int main(int argc, char const *argv[]) {
	
	// Set up gpio library
	wiringPiSetupGpio();

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