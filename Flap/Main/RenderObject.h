#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "Structure.h"

class RenderObject
{
public:
	// Functionality
	inline const Structure::SpriteInfo& GetSpriteInfoRef() const { return m_spriteInfo; }
	
	// Destruction
	virtual ~RenderObject();

protected:
	// Member Variables
	Structure::SpriteInfo m_spriteInfo;
};

#endif RENDER_OBJECT_H