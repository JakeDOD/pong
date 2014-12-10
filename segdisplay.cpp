#include "segdisplay.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>

// LED Display Driver Commands:
#define SYSTEM_SETUP	(0x2 << 4)
#define DISPLAY_SETUP	(0x8 << 4)
#define DISPLAY_DIMMING	(0xE << 4)

CSegDisplay::~CSegDisplay() {
	// close the file descriptor
	if (m_fd > 0) {
		close(m_fd);
	}
}

int CSegDisplay::Init() {
	// The location of the i2c device
	char i2c_device[] = "/dev/i2c-1";

	// A buffer to hold our character output
	char buffer[1];

	// Open i2c device
	m_fd = open(i2c_device, O_RDWR);
	if (m_fd < 0) {
		perror("open i2c display");
		fprintf(stderr, "Faild to open i2c display file descriptor\n");
		return -1;
	}

	// Set slave address
	if (ioctl(m_fd, I2C_SLAVE, 0x70) < 0) {
		perror("ioctl");
		fprintf(stderr, "Failed to set i2c slave address of the display\n");
		return -1;
	}

	// Turn on oscillator
	// Set up the commmand to send to the display
	buffer[0] = SYSTEM_SETUP | (0x1);

	// Send the command and check the result code
	if (write(m_fd, buffer, 1) != 1) {
		perror("system setup command");
		fprintf(stderr, "Failed to send setup command to the display\n");
		return -1;
	}

	// Turn on Display, No Blink
	buffer[0] = DISPLAY_SETUP | (0x1);

	// Send the command and check the result code
	if (write(m_fd, buffer, 1) != 1) {
		perror("display setup command");
		fprintf(stderr, "Failed to send display setup command to the display\n");
		return -1;
	}

	// Set Brightness
	buffer[0] = DISPLAY_DIMMING | (0xF);

	// Send the command and check the result code
	if (write(m_fd, buffer, 1) != 1) {
		perror("dimming command");
		fprintf(stderr, "Failed to send the display dimming command to the display\n");
		return -1;
	}
}

int CSegDisplay::UpdateDisplay(const char* displayString, const unsigned length, const index, const bool bDisplayColon) {

	char buffer[17];

	// convert the string before writing to the display
	convertStr(buffer, displayString, length, index, bDisplayColon);

	// Set slave address
	if (ioctl(m_fd, I2C_SLAVE, 0x70) < 0) {

		// If there was an error, attempt to reinitialize the display
		Init();

		usleep(100);

		// Reattempt to use the device
		if (ioctl(m_fd, I2C_SLAVE, 0x70) < 0) {
			perror("ioctl");
			fprintf(stderr, "Failed to set i2c slave address of the display\n");
			return -1;
		}
	}

	// Send the command and check the result code
	if (write(m_fd, buffer, 17) != 17) {
		perror("Update i2c Display");
		return -2;
	}
}

// Convert the given character to a display-able character
char CSegDisplay::getChar(const char ch) {
	if (ch == '.') {
		return numberTable[0];
	}

	// degree symbol
	if (ch == '*') {
		return 0x63;
	}

	if (ch == ' ') {
		return 0x00;
	}

	if (ch == '-') {
		return 0x40;
	}

	if ((ch >= '0') && (ch <= '9')) {
		return numberTable[ch - 47];
	} else if ((ch >= 'A') && (ch <= 'Z')) {
		return numberTable[ch - 54];
	} else if ((ch >= 'a') && (ch <= 'z')) {
		return numberTable[ch - 86];
	} else {
		return 0x00;
	}
}

// Convert a string to a display-able string
void CSegDisplay::convertStr(char* output, const char* str, const unsigned size, const unsigned index, const bool bWithColon) {
	memset(output, 0, size);

	unsigned i = index;

	char ch;

	ch = str[i % size];
	output[1] = getChar(ch);

	i++;

	ch = str[i % size];

	// check if it is a '.'
	if (ch == '.') {
		output[1] |= getChar(ch);

		i++;
		ch = str[i % size];
	}

	output[3] = getChar(ch);

	i++;

	if (bWithColon) {
		output[5] = 0x02;
	}

	ch = str[i % size];

	// check if it is a '.'
	if (ch == '.') {
		output[3] |= getChar(ch);
		
		i++;
		ch = str[i % size];
	}

	output[7] = getChar(ch);

	i++;

	ch = str[i % size];

	// check if it is a '.'
	if (ch == '.') {
		output[7] |= getChar(ch);
		
		i++;
		ch = str[i % size];
	}

	output[9] = getChar(ch);

	i++;

	ch = str[i % size];

	// check if it is a '.'
	if (ch == '.') {
		output[9] |= getChar(ch);
		
		i++;
	}
		
}