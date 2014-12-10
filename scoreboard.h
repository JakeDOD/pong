#ifndef _SCOREBOARD_H_
#define _SCOREBOARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include "segdisplay.h"

class CScoreBoard : private CSegDisplay {
public:
	// Initialize the scoreboard
	int Init(void);
	
	void CScoreBoard::UpdateScore(const char leftScore, const char rightScore);
};

typedef std::shared_ptr<CScoreBoard> CScoreBoardPtr_t;

#endif // _SCOREBOARD_H_