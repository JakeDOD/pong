#ifndef _PONG_H_
#define _PONG_H_

#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include "ball.h"
#include "paddle.h"
#include "nunchuck.h"
#include "HT1632.h"

class CPong {
public:
	CPong(void);

	// Initialize the game
	int Init(void);

	// Play the intro sequence
	void PlayIntro(void);

	// Call this to update the state of the game objects
	int Animate(void);

	// Call this to draw the objects on the screen
	void DrawFrame(void);

private:
	void DisplayWin(void);

	// Elements which make up the game Pong
	CPaddlePtr_t m_playerOne;
	CPaddlePtr_t m_playerTwo;
	CBallPtr_t m_ball;

	// The display
	HT1632LEDMatrixPtr_t m_matrix;

	// The pong controllers
	CNunchuckPtr_t m_controllerOne;
	CNunchuckPtr_t m_controllerTwo;

	// The game state variables
	bool m_IsPaused;
	int m_scoreOne;
	int m_scoreTwo;
};

#endif // _PONG_H_