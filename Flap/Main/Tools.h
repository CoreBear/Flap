#ifndef TOOLS_H
#define TOOLS_H

class SharedRender;

class Tools final
{
public:
	// Functionality
	inline static void AssignSharedRender(SharedRender& _sharedRender) { sp_sharedRender = &_sharedRender; }
	static int CenterText_ReturnStartColumn(int _textLineLength);

private:
	// Member Variables
	static SharedRender* sp_sharedRender;
};

#endif TOOLS_H