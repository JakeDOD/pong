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

pong: game.o HT1632.o paddle.o ball.o nunchuck.o pong.o
	$(CC) -lwiringPi game.o HT1632.o paddle.o ball.o nunchuck.o pong.o -o pong

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

other:
	$(CC) $(CFLAGS) demo.cpp HT1632.cpp -o demo
	$(CC) $(CFLAGS) test.cpp HT1632.cpp -o test
	$(CC) $(CFLAGS) exercise.cpp HT1632.cpp -o exercise
	$(CC) $(CFLAGS) nunchuck_test3.c -o nunchuck_test

clean:
	rm -rf *o pong