#include "paddle.h"

CPaddle::CPaddle(const int x, const int y, const int size)
	: m_x(x)
	, m_y(y)
	, m_size(size) {
}

void CPaddle::Draw(HT1632LEDMatrixPtr_t& matrix) {
	// Draw a line to form a paddle anchored to top of paddle
	int y2 = m_y + m_size - 1;

	// At this point we shouldn't have to worry about the paddle being out of bounds
	matrix->drawLine(m_x, m_y, m_x, y2, 1);
}

void CPaddle::MoveYPosBy(const int y) {

	// Move the paddle
	m_y += y;

	// Check that the paddle is within the bounds
	if (m_y < 0) {
		m_y = 0;
	} else if (m_y > 15 - (m_size - 1)) {
		m_y = 15 - (m_size - 1);
	}
}