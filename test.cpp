#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "wiringPi.h"

#include "HT1632.h"

/* 
This is a basic demo program showing how to write to a HT1632
These can be used for up to 16x24 LED matrix grids, with internal memory
and using only 3 pins - data, write and select.
Multiple HT1632's can share data and write pins, but need unique CS pins.
*/

#define DATA 13
#define WR 5
#define CS 6

void testMatrix(HT1632 matrix) {
  for (int i=0; i<24*16; i++) {
    matrix.setPixel(i);
    matrix.writeScreen();
  }
  
  // blink!
  matrix.blink(true);
  usleep(2000);
  matrix.blink(false);
  
  // Adjust the brightness down 
  for (int8_t i=15; i>=0; i--) {
   matrix.setBrightness(i);
   usleep(100);
  }
  // then back up
  for (uint8_t i=0; i<16; i++) {
   matrix.setBrightness(i);
   usleep(100);
  }

  // Clear it out
  for (int i=0; i<24*16; i++) {
    matrix.clrPixel(i);
    matrix.writeScreen();
  }
}

void demoMatrix(HT1632LEDMatrix matrix) {
  for (uint8_t y=0; y<matrix.height(); y++) {
    for (uint8_t x=0; x< matrix.width(); x++) {
      matrix.setPixel(x, y);
      matrix.writeScreen();
    }
  }
  // blink!
  matrix.blink(true);
  usleep(2000000);
  matrix.blink(false);
  
  // Adjust the brightness down 
  for (int8_t i=15; i>=0; i--) {
   matrix.setBrightness(i);
   usleep(100000);
  }
  // then back up
  for (uint8_t i=0; i<16; i++) {
   matrix.setBrightness(i);
   usleep(100000);
  }

  for (uint8_t y=0; y<matrix.height(); y++) {
    for (uint8_t x=0; x< matrix.width(); x++) {
      matrix.clrPixel(x, y);
      matrix.writeScreen();
    }
  }
}

int main(int argc, char const *argv[])
{
  wiringPiSetupGpio();

  HT1632LEDMatrix matrix = HT1632LEDMatrix(DATA, WR, CS);

  matrix.begin(HT1632_COMMON_16NMOS);
  matrix.fillScreen();
  
  usleep(100000);
  matrix.clearScreen();

  while (1) {
    //testMatrix(matrix);
    demoMatrix(matrix);
  }

  return 0;
}