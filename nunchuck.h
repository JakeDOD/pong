#ifndef _NUNCHUCK_H_
#define _NUNCHUCK_H_

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <memory>

#define I2C_DEV "/dev/i2c-1"

#define NUNCHUCK_ADDR 0x52

#define MASK_BUTTON_Z 0x01
#define MASK_BUTTON_C 0x02
#define MASK_ACCEL_X 0x0C
#define MASK_ACCEL_Y 0x40
#define MASK_ACCEL_Z 0xC0

#define BUTTON_Z(a) (a & MASK_BUTTON_Z)
#define BUTTON_C(a) ((a & MASK_BUTTON_C) >> 1)

#define ACCEL_X(a, b) ((a << 2) | ((b & MASK_ACCEL_X) >> 2))
#define ACCEL_Y(a, b) ((a << 2) | ((b & MASK_ACCEL_X) >> 4))
#define ACCEL_Z(a, b) ((a << 2) | ((b & MASK_ACCEL_X) >> 6))

struct Controls {

	Controls(void) {
		IsValid = false;
	}

	char Joystick_X;
	char Joystick_Y;
	int Accel_X;
	int Accel_Y;
	int Accel_Z;
	bool Button_C;
	bool Button_Z;
	bool IsValid;
};

class CNunchuck {
public:
	CNunchuck(char deviceAddr[]);
	inline ~CNunchuck(void) { close(m_fd); }

	int Init(void);

	int ReadID(void);

	// Return a stuct with the current controller info
	void GetControls(Controls& controls);

private:
	// Private worker functions
	int sendRequest(void);
	int readData(void);

	char m_deviceAddr[50];

	int m_fd;
	char m_buf[6];
	bool m_IsInitialized;
};

typedef std::shared_ptr<CNunchuck> CNunchuckPtr_t;

#endif // _NUNCHUCK_H_