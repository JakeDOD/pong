#include "scoreboard.h"

int CScoreBoard::Init() {

	// Initialize the base class
	if (CSegDisplay::Init() < 0) {
		return -1;
	}

	// Set the score to 0-0
	UpdateScore(0, 0);
}

void CScoreBoard::UpdateScore(const char leftScore, const char rightScore) {
	char displayStr[6];
	const unsigned size = 6;

	// Format the display string to be "03:01"
	sprintf(displayStr, "%02d%02d", leftScore, rightScore);

	UpdateDisplay(displayStr, size, 0, true);
}