#pragma region Includes
#include "Bird.h"

#include "Consts.h"
#include "SharedMemory.h"
#pragma endregion

#pragma region Static Initialization
const float Bird::s_flyForwardSpeed = static_cast<float>(1 * Consts::FIXED_DELTA_TIME_F);
#pragma endregion

#pragma region Initialization
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

	m_dragVelocity.m_y = Consts::NO_VALUE_F;
	m_dragVelocity.m_x = Consts::NO_VALUE_F;
	m_velocity.m_x = s_flyForwardSpeed;
	m_velocity.m_y = Consts::NO_VALUE_F;
}
#pragma endregion

#pragma region Updates
void Bird::FixedUpdate() 
{
	Structure::Vector2<float> a = -m_velocity.NormalizeReturn();
	float b = m_velocity.SquareMagnitudeReturn();
	float c = static_cast<float>(Consts::MULTIPLICATIVE_HALF_F * m_velocity.SquareMagnitudeReturn());

	// Update drag
	m_dragVelocity = -m_velocity.NormalizeReturn() * m_velocity.SquareMagnitudeReturn();

	// Add drag
	m_velocity += m_dragVelocity;

	// Apply gravity
	m_velocity.m_y += Consts::GRAVITY_FORCE_CONVERTED_F;

	// Convert to per frame
	m_velocity *= Consts::FIXED_DELTA_TIME_F;

	// Apply velocity
	m_position.m_x += m_velocity.m_x;
	m_position.m_y += m_velocity.m_y;

	// Update sprite position for the renderer
	m_spriteInfo.m_position.m_x = static_cast<int>(m_position.m_x);
	m_spriteInfo.m_position.m_y = static_cast<int>(m_position.m_y);
}
#pragma endregion