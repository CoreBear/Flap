#ifndef TOOLS_H
#define TOOLS_H

#include "Consts.h"
#include "SharedRender.h"

class Tools final
{
public:
	// Functionality
	inline static void AssignSharedRender(SharedRender& _sharedRender) { sp_sharedRender = &_sharedRender; }
	inline static int CenterText_ReturnStartColumn(const char* _textLine) { return static_cast<int>((sp_sharedRender->m_bufferHW.X / 2) - strlen(_textLine) / 2); }
	inline static int CenterText_ReturnStartColumn(int _textLineLength) { return (sp_sharedRender->m_bufferHW.X / 2) - _textLineLength / 2; }
	inline static int CharToInt(char _char) { return static_cast<int>(static_cast<int>(_char) - CHAR_OFFSETTER); }
	static char IntToChar(int _number);				// NOTE: Negative numbers are converted to positive numbers. Use IntToString for this functionality.
	static const char* IntToString(int _number);
	static int StringToInt(const char* _string);

private:
	// Static Variables
	static int s_reusableIterator_1;
	static int s_reusableIterator_2;
	static constexpr int CHAR_OFFSETTER = static_cast<int>('0');
	static SharedRender* sp_sharedRender;
};

#endif TOOLS_H