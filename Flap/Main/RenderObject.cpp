#pragma region Includes
#include "RenderObject.h"

#include "Consts.h"
#pragma endregion

#pragma region Destruction
RenderObject::~RenderObject()
{
	for (int animationKeyFrameIndex = Consts::NO_VALUE, spriteHeightIndex; animationKeyFrameIndex < m_spriteInfo.m_numberOfAnimationKeyFrames; animationKeyFrameIndex++)
	{
		for (spriteHeightIndex = Consts::NO_VALUE; spriteHeightIndex < m_spriteInfo.m_spriteHeight; spriteHeightIndex++)
		{
			delete m_spriteInfo.mppp_sprite[animationKeyFrameIndex][spriteHeightIndex];
		}

		delete[] m_spriteInfo.mppp_sprite[animationKeyFrameIndex];
	}

	delete[] m_spriteInfo.mppp_sprite;
}
#pragma endregion