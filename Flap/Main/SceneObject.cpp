#pragma region Includes
#include "SceneObject.h"

#include "BufferCell.h"
#include "Consts.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "SharedGame.h"
#pragma endregion

#pragma region Static Initialization
ObjectManager* SceneObject::sp_objectManager = nullptr;
SharedGame* SceneObject::sp_sharedGame = nullptr;
#pragma endregion

#pragma region Initialization
void SceneObject::Initialize(const Structure::Generic* const _genericContainer)
{
	sp_objectManager->AddToSceneObjects(this);
}
#pragma endregion

#pragma region Public Functionality
void SceneObject::SetPosition(const Structure::Vector2<int>& _position)
{
	m_position = _position;
}
#pragma endregion

#pragma region Protected Functionality
bool SceneObject::CheckPositionValidity(Structure::Vector2<int>& _position)
{
	// NOTE: Notice the negation
	return !(_position.m_x < Consts::NO_VALUE || _position.m_y < Consts::NO_VALUE || _position.m_x == sp_sharedGame->m_gameAreaHeightWidth.m_x || _position.m_y == sp_sharedGame->m_gameAreaHeightWidth.m_y);
}
void SceneObject::WriteIntoFrameBufferCell(Structure::CollisionRenderInfo& _collisionRenderInfo)
{
	mp_bufferCell = &sp_sharedGame->mp_frameBuffer[(_collisionRenderInfo.m_position.m_y * sp_sharedGame->FRAME_BUFFER_HEIGHT_WIDTH.m_x) + _collisionRenderInfo.m_position.m_x];

	mp_bufferCell->mp_collisionRenderInfo[mp_bufferCell->m_objectInCellIndex] = &_collisionRenderInfo;
	mp_bufferCell->mp_voidSceneObject[mp_bufferCell->m_objectInCellIndex++] = reinterpret_cast<void*>(this);
}
#pragma endregion

#pragma region Destruction
void SceneObject::Denitialize()
{
	sp_objectManager->RemoveFromSceneObjects(this);
}
void SceneObject::Destroy(bool _cleanScene)
{
	if (_cleanScene == false)
	{
		sp_sharedGame->AddAvailableSpawnIndex(m_position.m_x, m_position.m_y);
	}
}
#pragma endregion