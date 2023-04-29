#ifndef TEXTLINE_H
#define TEXTLINE_H

#include "Structure.h"

class TextLine final
{
public:
	// Member Variables
	const char* m_text;
	int m_textLineEndColum;
	Structure::Vector2<int> m_position;

	// Initialization
	TextLine() = delete;
	TextLine(const char* _text, int _row);
	TextLine(const TextLine&) = delete;
	TextLine& operator=(const TextLine&) = delete;

private:
	// Static Variables
	static const char* s_walker;
	static int s_textLineLength;

	// Functionality
	int GetTextLineLength(const char* _text);
};

#endif TEXTLINE_H