#pragma region Includes
#include "SceneObject.h"

#include "BufferCell.h"
#include "Consts.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "SharedRender.h"
#pragma endregion

#pragma region Static Initialization
ObjectManager* SceneObject::sp_objectManager = nullptr;
SharedRender* SceneObject::sp_sharedRender = nullptr;
#pragma endregion

#pragma region Initialization
void SceneObject::Initialize(const Structure::Generic* const _genericContainer)
{
	sp_objectManager->AddToSceneObjects(this);
}
#pragma endregion

#pragma region Public Functionality
void SceneObject::SetPosition(const Structure::Vector2& _position)
{
	m_position = _position;
}
#pragma endregion

#pragma region Protected Functionality
bool SceneObject::CheckPositionValidity(Structure::Vector2& _position)
{
	return (_position.m_x < Consts::NO_VALUE || _position.m_y < Consts::NO_VALUE || _position.m_x == sp_sharedRender->m_bufferHW.X || _position.m_y == sp_sharedRender->m_bufferHW.Y) ? false : true;

}
void SceneObject::WriteIntoFrameBufferCell(Structure::CollisionRenderInfo& _collisionRenderInfo)
{
	mp_bufferCell = &sp_sharedRender->mp_frameBuffer[(_collisionRenderInfo.m_position.m_y * sp_sharedRender->m_bufferHW.X) + _collisionRenderInfo.m_position.m_x];

	mp_bufferCell->mp_collisionRenderInfo[mp_bufferCell->m_objectInCellIndex] = &_collisionRenderInfo;
	mp_bufferCell->mp_voidSceneObject[mp_bufferCell->m_objectInCellIndex++] = reinterpret_cast<void*>(this);
}
#pragma endregion

#pragma region Destruction
void SceneObject::Denitialize(bool _removeFromSceneObjects)
{
	if (_removeFromSceneObjects)
	{
		sp_objectManager->RemoveFromSceneObjects(this);
	}
}
#pragma endregion