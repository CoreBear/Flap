#ifndef TOOLS_H
#define TOOLS_H

// Forward Declarations
class SharedCollisionRender;

class Tools final
{
public:
	// Functionality
	inline static void AssignSharedCollisionRender(SharedCollisionRender& _sharedCollisionRender) { sp_sharedCollisionRender = &_sharedCollisionRender; }
	static int CenterText_ReturnStartColumn(const char* _text);

private:
	// Member Variables
	static SharedCollisionRender* sp_sharedCollisionRender;
};

#endif TOOLS_H