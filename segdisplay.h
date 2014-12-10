#ifndef _SEGDISPLAY_H_
#define _SEGDISPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <memory>

class CSegDisplay {
public:
	virtual ~CSegDisplay(void);
	virtual int Init(void);

	// Set the score on the display
	int UpdateDisplay(const char* displayString, const unsigned length, const index = 0, const bool bDisplayColon = false);

private:
	char getChar(const char ch);
	void convertStr(char* output, const char* str, const unsigned size, const unsigned index, const bool bWithColon);

	int m_fd;
};

typedef std::shared_ptr<CSegDisplay> CSegDisplayPtr_t;

// LED Display Alphabet
static const unsigned char numberTable[] = {
	0x80,	/* . */
	0x3F,	/* 0 */
	0x06,	/* 1 */
	0x5B,	/* 2 */
	0x4F,	/* 3 */
	0x66,	/* 4 */
	0x6D,	/* 5 */
	0x7D,	/* 6 */
	0x07,	/* 7 */
	0x7F,	/* 8 */
	0x6F,	/* 9 */
	0x77,	/* A */
	0x7C,	/* b */
	0x39,	/* C */
	0x5E,	/* d */
	0x79,	/* E */
	0x71,	/* F */
	0x6F,	/* g */
	0x76,	/* H */
	0x30,	/* I */
	0x1E,	/* J */
	0x76, 	/* K */
	0x38, 	/* L */
	0x76,	/* M */
	0x54, 	/* n */
	0x3F,	/* O */
	0x73,	/* P */
	0x67,	/* q */
	0x50,	/* r */
	0x6D,	/* S */
	0x78,	/* t */
	0x3E,	/* U */
	0x1C,	/* v */
	0x76,	/* W */
	0x76,	/* X */
	0x6E,	/* Y */
	0x5B	/* Z */
};

#endif // _SEGDISPLAY_H_