
#include <stdint.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi.h"

// Check the return value and if it is less than 0, print a message and exit the program
void checkForFailure(const int returnValue, const char* strFailureMessage) {
	if (returnValue < 0) {
		fprintf(stderr, "%s\n", strFailureMessage);
		exit(EXIT_FAILURE);
	}
}

// Do the SPI configuration
int initSPI(void) {
    int spi_fd, result;

    // The SPI device
    char spi_device[] = "/dev/spidev0.0";

	// Open SPI device
	spi_fd = open(spi_device, O_RDWR);
	checkForFailure(spi_fd, "Faild to open spi device");

	// Set SPI Mode_0
	int nMode = SPI_MODE_0;
	result = ioctl(spi_fd, SPI_IOC_WR_MODE, &nMode);
	checkForFailure(result, "Failed to set spi mode");

	// Set 8 bits per word
	int nBPW = 8;
	result = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &nBPW);
	checkForFailure(result, "Failed to set number of bits per word");

	// Set 100 kHz max frequency
	int nMaxFreq = 100000;
	result = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &nMaxFreq);

	return spi_fd;
}

// Write data via SPI
void writeSPIData(const int spi_fd, const uint16_t data, const uint8_t length) {

	// Switch the CS

	uint16_t shiftedData = data << (16 - length);

	struct spi_ioc_transfer spi;
	unsigned char data_out[2] = { ((shiftedData & 0xFF00) >> 8), (shiftedData & 0x00FF) };
	unsigned char data_in[2];

	// Init the struct with our configuration
	spi.tx_buf = (unsigned long)&data_out;
	spi.rx_buf = (unsigned long)&data_in;
	spi.len = 2;
	spi.delay_usecs = 0;
	spi.speed_hz = 100000;
	spi.bits_per_word = 8;
	spi.cs_change = 0;

	// Talk to the device!
	int result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
	if (result < 0) {
		printf("Failed to talk to the spi device. Error: %d\n", result);
	}

	// Switch CS
}