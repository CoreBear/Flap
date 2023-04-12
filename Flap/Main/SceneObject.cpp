#pragma region Includes
#include "SceneObject.h"

#include "ObjectManager.h"
#pragma endregion

#pragma region Static Initialization
ObjectManager* SceneObject::sp_objectManager = nullptr;

SceneObject::SceneObject(Structure::RenderInfo* _renderInfo) : m_spawnState(Enums::SpawnState::WaitingSelection), mp_renderInfo(_renderInfo)
{
	return;
}
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

#pragma region Destruction
void SceneObject::Denitialize()
{
	sp_objectManager->RemoveFromSceneObjects(this);
}
SceneObject::~SceneObject() 
{ 
	delete mp_renderInfo; 
}
#pragma endregion