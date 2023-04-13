#pragma region Includes
#include "TextLine.h"

#include "Consts.h"
#pragma endregion

#pragma region Static Initialization
const char* TextLine::s_walker = nullptr;
int TextLine::s_textLineLength = Consts::NO_VALUE;
#pragma endregion

#pragma region Initializaiton
TextLine::TextLine(const char* _text, int _row) : m_text(_text)
{
	m_position.m_x = Tools::CenterText_ReturnStartColumn(GetTextLineLength(_text));
	m_position.m_y = _row;

	m_textLineEndColum = m_position.m_x + s_textLineLength;
}
#pragma endregion

#pragma region Private Functionality
int TextLine::GetTextLineLength(const char* _text)
{
	s_walker = _text;

	s_textLineLength = Consts::NO_VALUE;

	while (*s_walker != '\0')
	{
		++s_textLineLength;
		++s_walker;
	}

	return s_textLineLength;
}
#pragma endregion