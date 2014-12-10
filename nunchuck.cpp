#include "nunchuck.h"
#include <cstring>
#include <stdio.h>

CNunchuck::CNunchuck(const char deviceAddr[]) {

	// Initialize the member variable
	strcpy(m_deviceAddr, deviceAddr);
}

CNunchuck::~CNunchuck() {
	// close the file descriptor
	if (m_fd > 0) {
		close(m_fd);
	}
}

int CNunchuck::Init() {
	m_IsInitialized = false;

	char buf[2];

	if ((m_fd = open(m_deviceAddr, O_RDWR)) < 0) {
		perror("open");
	    return -1;
	}

	if (ioctl(m_fd, I2C_SLAVE, NUNCHUCK_ADDR) < 0) {
        close(m_fd);
        perror("ioctl");
        return -1;
	}

	buf[0] = 0xF0;
	buf[1] = 0x55;

	if (write(m_fd, buf, 2) != 2) {
        close(m_fd);
        perror("write");
        return -1;
	}

	buf[0] = 0xFB;
	buf[1] = 0x00;

	if (write(m_fd, buf, 2) != 2) {
        close(m_fd);
        perror("write");
        return -1;
	}

	return 0;
}

int CNunchuck::ReadID() {
	m_buf[0] = 0xFA;

	if (write(m_fd, m_buf, 1) < 0) {
        perror("write");
        close(m_fd);
        return -1;
	}

	if (read(m_fd, m_buf, 6) < 0) {
        perror("read");
        close(m_fd);
        return -2;
	}

	m_IsInitialized = true;

	// for debug only
	printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", m_buf[0], m_buf[1], m_buf[2], m_buf[3], m_buf[4], m_buf[5]);

	return 0;
}

int CNunchuck::sendRequest() {
    char buf[1];

    buf[0] = 0x00;

    if (write(m_fd, buf, 1) < 0) {
    	fprintf(stderr, "There was an error sending a request\n");
        close(m_fd);
        return -1;
    }

    return 0;
}

int CNunchuck::readData() {

	// clear the buffer
	for (int i = 0; i < 6; i++) {
		m_buf[i] = 0;
	}

	if (ioctl(m_fd, I2C_SLAVE, NUNCHUCK_ADDR) < 0) {
        close(m_fd);
        perror("ioctl");
        return -1;
	}

	m_buf[0] = 0x00;

    if (write(m_fd, m_buf, 1) < 0) {
    	fprintf(stderr, "There was an error sending a request\n");
        close(m_fd);
        return -1;
    }

   	// wait for nunchuck
    usleep(100);

    if (read(m_fd, m_buf, 6) != 6) {
    	fprintf(stderr, "There was a read error\n");
        return -1;
    }

    return 0;
}

// Return a stuct with the current controller info
void CNunchuck::GetControls(Controls& controls) {

	if (!m_IsInitialized) {
		if (ReadID() < 0) {
			return;
		}
	}

	if (readData() < 0) {
        
		Init();

		// Delay
		usleep(100);

		// Reattempt to connect
		if (readData() < 0) {
			perror("readData");
			return;
		}
	}

	// fill the struct with the returned data
	controls.Joystick_X = m_buf[0];
	controls.Joystick_Y = m_buf[1];
	controls.Accel_X = ACCEL_X(m_buf[2], m_buf[5]);
	controls.Accel_Y = ACCEL_X(m_buf[3], m_buf[5]);
	controls.Accel_Z = ACCEL_X(m_buf[4], m_buf[5]);
	controls.Button_C = (BUTTON_C(m_buf[5]) ? false : true);
	controls.Button_Z = (BUTTON_Z(m_buf[5]) ? false : true);
	controls.IsValid = true;

	return;
}