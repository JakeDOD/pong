#ifndef _PADDLE_H_
#define _PADDLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include "HT1632.h"

class CPaddle {
public:
	CPaddle(const int x, const int y, const int size);

	// Draw the paddle on the matrix
	void Draw(HT1632LEDMatrixPtr_t& matrix);

	void MoveYPosBy(const int y);

	// public for simplicity
	int m_y;
	int m_size;

private:
	int m_x;	
};

typedef std::shared_ptr<CPaddle> CPaddlePtr_t;

#endif // _PADDLE_H_