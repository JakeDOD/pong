#include "pong.h"

#include <cstdio>

// The connections for the matrix display
#define DATA	24	// was 13
#define WR 		25 	// was 5
#define CS 		18	// was 6

// Initialize the Pong game
CPong::CPong()
	: m_IsPaused(false)
	, m_scoreOne(0)
	, m_scoreTwo(0)
	, m_controllerOne(new CNunchuck("/dev/i2c-1"))
	, m_controllerTwo(new CNunchuck("/dev/i2c-0"))
	, m_matrix(new HT1632LEDMatrix(DATA, WR, CS))
	, m_scoreBoard(new CScoreBoard())
	, m_playerOne(new CPaddle(0, 8, 5))
	, m_playerTwo(new CPaddle(23, 8, 5))
	, m_ball(new CBall(12, 8)) {
}

// Initialize the game
int CPong::Init() {

	// Set up the display
	m_matrix->begin(HT1632_COMMON_16NMOS);

	// Initialize the scoreboard
	if (m_scoreBoard->Init() < 0) {
		return -1;
	}

	// Initialize the nunchuck controllers
	if ((m_controllerOne->Init() < 0) || (m_controllerTwo->Init() < 0)) {
		return -2;
	}

	if ((m_controllerOne->ReadID() < 0) || (m_controllerTwo->ReadID() < 0)) {
		return -3;
	}

	return 0;
}

// Play intro sequence (blocking)
void CPong::PlayIntro() {

	// draw some text!
	m_matrix->setTextSize(1);    // size 1 == 8 pixels high
	m_matrix->setTextColor(1);   // 'lit' LEDs

	m_matrix->setCursor(0, 0);   // start at top left, with one pixel of spacing
	m_matrix->print("PLAY");
	m_matrix->writeScreen();

	int i = 0;
	for (i = 0; i < 4; ++i) {
		usleep(500000);

		m_matrix->clearScreen();
		m_matrix->setCursor(0, 0);   // start at top left, with one pixel of spacing
		m_matrix->print("PLAY");
		m_matrix->setCursor(0, 8);   // next line, 8 pixels down
		m_matrix->print("PONG");
		m_matrix->writeScreen();

		usleep(500000);

		m_matrix->clearScreen();
		m_matrix->setCursor(0, 0);   // start at top left, with one pixel of spacing
		m_matrix->print("PLAY");
		m_matrix->writeScreen();
	}

	usleep(500000);

	m_matrix->clearScreen();
	m_matrix->setCursor(0, 0);   // start at top left, with one pixel of spacing
	m_matrix->print("PLAY");
	m_matrix->setCursor(0, 8);   // next line, 8 pixels down
	m_matrix->print("PONG");
	m_matrix->writeScreen();
}

// Update the state of the game objects
int CPong::Animate() {

	// Start by getting the current status of the controllers
	Controls nunchuckOne;
	m_controllerOne->GetControls(nunchuckOne);
	Controls nunchuckTwo;
	m_controllerTwo->GetControls(nunchuckTwo);

	// The current controllers are not valid (there was an error)
	if ((nunchuckOne.IsValid == false) || (nunchuckTwo.IsValid == false)) {
		return 0;
	}

	// Check if the controllers are pausing/unpausing the game
	if ((nunchuckOne.Button_C) || (nunchuckTwo.Button_C)) {

		bool isValidPress = false;

		// dobounce the input by checking that the input remains valid for 3 polls
		for (char i = 0; i < 3; i++) {
			usleep(10000);

			// get the controller input
			Controls leftNunchuck;
			m_controllerOne->GetControls(leftNunchuck);
			Controls rightNunchuck;
			m_controllerTwo->GetControls(rightNunchuck);

			if ((nunchuckOne.Button_C && nunchuckOne.IsValid) || (nunchuckTwo.Button_C && nunchuckTwo.IsValid)) {
				// still pressing C button
				isValidPress = true;
			} else {
				isValidPress = false;
				break;
			}
		}

		if (isValidPress) {
			m_IsPaused = !m_IsPaused;
		}

		// We can return early if the game is paused
		if (m_IsPaused) {
			return 0;
		}
	}

	if (m_IsPaused) {
		//printf("Game is paused!\n");
		return 0;
	}

	// Update the positions of the paddles
	if (nunchuckOne.Joystick_Y > 180) {
		// move the paddle up
		m_playerOne->MoveYPosBy(-1);
	} else if (nunchuckOne.Joystick_Y < 100) {
		// move the paddle down
		m_playerOne->MoveYPosBy(1);
	}

	if (nunchuckTwo.Joystick_Y > 180) {
		// move the paddle up
		m_playerTwo->MoveYPosBy(-1);
	} else if (nunchuckTwo.Joystick_Y < 100) {
		// move the paddle down
		m_playerTwo->MoveYPosBy(1);
	}

	// Update the position of the ball
	int retValue = m_ball->UpdatePosition(m_playerOne, m_playerTwo);
	if (retValue == -1) {

		// playerOne has missed the ball
		m_scoreTwo++;
		m_scoreBoard->UpdateScore(m_scoreOne, m_scoreTwo);

		// Create a new ball
		m_ball.reset(new CBall(12, 8));
		m_ball->UpdatePosition(m_playerOne, m_playerTwo);

	} else if (retValue == 1) {

		// playerTwo has misseed the ball
		m_scoreOne++;
		m_scoreBoard->UpdateScore(m_scoreOne, m_scoreTwo);

		// Create a new ball
		m_ball.reset(new CBall(12, 8));
		m_ball->UpdatePosition(m_playerOne, m_playerTwo);
	}

	// Check the scores of the players for a win
	if ((m_scoreOne == 10) || (m_scoreTwo == 10)) {
		m_IsPaused = true;
	}

	return 0;
}

void CPong::DisplayWin() {
	m_matrix->clearScreen();
	m_matrix->setTextSize(1);    // size 1 == 8 pixels high
	m_matrix->setTextColor(1);   // 'lit' LEDs
	m_matrix->setCursor(0, 0);   // start at top left, with one pixel of spacing

	if (m_scoreOne == 10) {
		m_matrix->print("PL 1");
	} else if (m_scoreTwo == 10) {
		m_matrix->print("PL 2");
	}

	m_matrix->setCursor(0, 8);   // next line, 8 pixels down
	m_matrix->print("WINS");
	m_matrix->writeScreen();

	bool gameOver = true;
	while (gameOver) {

		// Start by getting the current status of the controllers
		Controls nunchuckOne;
		m_controllerOne->GetControls(nunchuckOne);
		Controls nunchuckTwo;
		m_controllerTwo->GetControls(nunchuckTwo);

		// wait for conformation controls
		if ((nunchuckOne.IsValid && nunchuckOne.Button_C && nunchuckOne.Button_Z)
			|| (nunchuckTwo.IsValid && nunchuckTwo.Button_C && nunchuckTwo.Button_Z)) {

			// reset the game
			m_playerOne.reset(new CPaddle(0, 8, 5));
			m_playerTwo.reset(new CPaddle(23, 8, 5));
			m_ball.reset(new CBall(12, 8));
			m_scoreOne = 0;
			m_scoreTwo = 0;
			m_scoreBoard->UpdateScore(m_scoreOne, m_scoreTwo);
			gameOver = false;
		}

		usleep(250);
	}
}

// The drawing function
void CPong::DrawFrame() {

	if (m_IsPaused) {
		// Check for a win before continuing
		if ((m_scoreOne == 10) || (m_scoreTwo == 10)) {
			DisplayWin();
		}
	} else {
		// First reset the screen
		m_matrix->clearScreen();

		// Draw the paddles
		m_playerOne->Draw(m_matrix);
		m_playerTwo->Draw(m_matrix);

		// Draw the ball
		m_ball->Draw(m_matrix);

		// Last draw the screen
		m_matrix->writeScreen();
	}
}