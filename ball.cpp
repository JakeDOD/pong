#include "ball.h"

#include <cstdio>

#define SPEED_INC	0.1

CBall::CBall(const int x, const int y, const int radius) {
	m_x = x;
	//m_y = y;
	m_radius = radius;
	m_speed = 0.5;

	std::srand(std::time(0));

	// Randomize the starting position
	m_y = (int)((RANDOM_NUMBER * 13) + 3);

	// Set a random starting angle
	SetAngle(((RANDOM_NUMBER > 0.5) ? 1 : -1) * ((RANDOM_NUMBER * 90.0) + 15) * PI / 180.0);
}

void CBall::SetAngle(const double angle) {
	m_angle = angle;
	
	// Round to two decimal places
	m_dx = cos(angle);
	m_dy = sin(angle);

	printf("%lf\n", m_angle);
}

// Collision detection doesn't take into account the radius yet
int CBall::UpdatePosition(const CPaddlePtr_t& leftPaddle, const CPaddlePtr_t& rightPaddle) {
	// The position of the ball can be a floating point value,
	// the position will be rounded for drawing
	m_x += m_dx * m_speed;
	m_y += m_dy * m_speed;

	// round the position of the ball for drawing
	char x = (char)floor(m_x + 0.5);
	char y = (char)floor(m_y + 0.5);

	// Check if we've hit the boundaries
	if (x <= 1) {
		// we have hit the left side
		if ((x <= 1) && (y >= leftPaddle->m_y) && (y <= (leftPaddle->m_y + leftPaddle->m_size - 1))) {
			// we have hit a paddle
			// change directions
			m_dx = 0 - m_dx;
			m_speed += SPEED_INC;

		} else if (x == 0) {
			printf("Left side hit\n");
			// right point!
			return -1;
		}

	} else if (x >= 22) {
		// we have hit the right side
		if ((x >= 22) && (y >= rightPaddle->m_y) && (y <= (rightPaddle->m_y + rightPaddle->m_size - 1))) {
			// we have not hit a paddle
			// change directions
			m_dx = 0 - m_dx;
			m_speed += SPEED_INC;
		} else if (x == 23) {
			// left point!
			return 1;
		}
	}

	if ((y <= 0) || (y >= 15)) {
		// We have hit the top or bottom
		m_dy = 0 - m_dy;
	}

	return 0;
}

void CBall::Draw(HT1632LEDMatrixPtr_t& matrix) {
	// We assume the ball is within the bounds at this point
	// Just draw the ball

	// round the position of the ball for drawing
	char x = (char)floor(m_x + 0.5);
	char y = (char)floor(m_y + 0.5);

	if (m_radius == 1) {
		matrix->drawPixel(x, y, 1);
	} else {
		matrix->fillCircle(x, y, m_radius, 1);
	}
}