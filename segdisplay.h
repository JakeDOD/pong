#ifndef _SEGDISPLAY_H_
#define _SEGDISPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <memory>

class CSegDisplay {
public:
	CSegDisplay(void);

	void Init(void);

	void UpdateDisplay(char leftScore, char rightScore);

private:
	int m_fd;
};

typedef std::shared_ptr<CSegDisplay> CSegDisplayPtr_t;

#endif // _SEGDISPLAY_H_