# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-c -O2 -std=c++0x -Wall -lwiringPi

all: pong

update:
	git pull origin
	make pong

level2:
	sudo ./pong &
	sudo ./pong &

pong: game.o HT1632.o paddle.o ball.o nunchuck.o pong.o segdisplay.o scoreboard.o
	$(CC) -lwiringPi game.o HT1632.o paddle.o ball.o nunchuck.o pong.o segdisplay.o scoreboard.o -o pong

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