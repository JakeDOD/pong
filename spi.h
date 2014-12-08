
#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

// Do the SPI configuration
int initSPI(void);

// Write data via SPI
void writeSPIData(const int spi_fd, const uint16_t data, const uint8_t length);

#endif // SPI_H_