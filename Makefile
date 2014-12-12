
CC=g++

CFLAGS=-c -O2 -std=c++0x -Wall -lwiringPi -lbcm2835

all: pong

# Get any updates from GitHub and compile the source
update: pull pong

# Compile the source and install pong so that it runs at boot
install: pong
	# Add a line to the /etc/rc.local file so that pong runs at boot
	sudo sh install_pong.sh

# Pull changes from GitHub
pull:
	git pull origin

# Requires to be run with sudo
level2:
	# This is more of a joke than anything, and the results are unpredictable
	sudo ./pong &
	sudo ./pong &

pong: game.o HT1632.o paddle.o ball.o nunchuck.o pong.o segdisplay.o scoreboard.o
	$(CC) -lwiringPi -lbcm2835 game.o HT1632.o paddle.o ball.o nunchuck.o pong.o segdisplay.o scoreboard.o -o pong

game.o: game.cpp
	$(CC) $(CFLAGS) game.cpp

HT1632.o: HT1632.cpp
	$(CC) $(CFLAGS) HT1632.cpp

paddle.o: paddle.cpp
	$(CC) $(CFLAGS) paddle.cpp

ball.o: ball.cpp
	$(CC) $(CFLAGS) ball.cpp

nunchuck.o: nunchuck.cpp
	$(CC) $(CFLAGS) nunchuck.cpp

pong.o: pong.cpp
	$(CC) $(CFLAGS) pong.cpp

segdisplay.o: segdisplay.cpp
	$(CC) $(CFLAGS) segdisplay.cpp

scoreboard.o: scoreboard.cpp
	$(CC) $(CFLAGS) scoreboard.cpp

clean:
	rm -rf *o pong