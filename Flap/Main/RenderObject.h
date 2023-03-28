#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

class RenderObject
{
public:
	struct SpriteInfo
	{
	public:
		const char*** mppp_sprite;
		short m_color;
		int m_animationKeyFrameIndexToRender;
		int m_numberOfAnimationKeyFrames;
		int m_spriteHeight;
	};

	// Destruction
	virtual ~RenderObject();

protected:
	// Member Variables
	SpriteInfo m_spriteInfo;
};

#endif RENDER_OBJECT_H