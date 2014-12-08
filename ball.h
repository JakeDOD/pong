#ifndef _BALL_H_
#define _BALL_H_

#include <stdio.h>
#include <stdlib.h>

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <memory>

#include "HT1632.h"
#include "paddle.h"

#define PI 				3.141592
#define RANDOM_NUMBER	((double)std::rand() / (double)RAND_MAX)

class CBall {
public:
	CBall(const int x, const int y, const int radius);

	void SetAngle(const double angle);

	void Draw(HT1632LEDMatrixPtr_t& matrix);

	// Returns -1 for left hit, 1 for right hit
	int UpdatePosition(const CPaddlePtr_t& leftPaddle, const CPaddlePtr_t& rightPaddle);

private:
	double m_x;
	double m_y;
	double m_angle;
	double m_dx;
	double m_dy;
	int m_radius;
	double m_speed;
};

typedef std::shared_ptr<CBall> CBallPtr_t;

#endif // _BALL_H_