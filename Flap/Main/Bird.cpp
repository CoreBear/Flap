#pragma region Includes
#include "Bird.h"

#include "AtomicMemory.h"
#include "Consts.h"
#pragma endregion

#pragma region Initialization
const float Bird::s_flyForwardSpeed = static_cast<float>(10 * Consts::FIXED_DELTA_TIME_D);

Bird::Bird() 
{
	m_spriteInfo.m_numberOfAnimationKeyFrames = 3;
	m_spriteInfo.m_spriteHeight = 1;

	// Generate sprite key frame pointers
	m_spriteInfo.mppp_sprite = new const char** [m_spriteInfo.m_numberOfAnimationKeyFrames];

	for (int animationKeyFrameIndex = Consts::NO_VALUE; animationKeyFrameIndex < m_spriteInfo.m_numberOfAnimationKeyFrames; animationKeyFrameIndex++)
	{
		// Generate row pointers
		m_spriteInfo.mppp_sprite[animationKeyFrameIndex] = new const char* [m_spriteInfo.m_spriteHeight];
	}

	m_spriteInfo.mppp_sprite[0][0] = "\\@/";
	m_spriteInfo.mppp_sprite[1][0] = "-@-";
	m_spriteInfo.mppp_sprite[2][0] = "/@\\";

	m_spriteInfo.m_color = 10;

	m_spriteInfo.m_animationKeyFrameIndexToRender = 0;
}
void Bird::Initialize(const Structure::Generic& _genericContainer)
{
	SceneObject::Initialize(_genericContainer);
}
#pragma endregion

#pragma region Updates
void Bird::FixedUpdate() 
{
	// Fly forward
	m_positionF.m_x += s_flyForwardSpeed;

	// Apply gravity
	m_positionF.m_y += Consts::GRAVITY_FORCE_CONVERTED_F;

	m_position.m_x = static_cast<int>(m_positionF.m_x);
	m_position.m_y = static_cast<int>(m_positionF.m_y);
}
#pragma endregion