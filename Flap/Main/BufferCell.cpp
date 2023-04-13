#pragma region Includes
#include "BufferCell.h"
#pragma endregion

#pragma region Public Functionality
void BufferCell::ResetCell()
{
	mp_collisionRenderInfo[Consts::NO_VALUE] = nullptr;
	mp_collisionRenderInfo[Consts::OFF_BY_ONE] = nullptr;
	m_character = Consts::EMPTY_SPACE_CHAR;
	m_objectInCellIndex = Consts::NO_VALUE;
	m_colorBFGround = NULL;
	m_position.m_x = Consts::NO_VALUE;
	m_position.m_y = Consts::NO_VALUE;
	mp_voidSceneObject[Consts::NO_VALUE] = nullptr;
	mp_voidSceneObject[Consts::OFF_BY_ONE] = nullptr;
}
#pragma endregion