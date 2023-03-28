#pragma region Includes
#include "SceneObject.h"

#include "ObjectManager.h"
#pragma endregion

#pragma region Initialization
ObjectManager* SceneObject::sp_objectManager = nullptr;

void SceneObject::Initialize(const Structure::Generic& _genericContainer)
{
	m_isActive = true;

	sp_objectManager->AddToSceneObjects(this);
}
SceneObject::SceneObject()
{
	m_isActive = false;
}
#pragma endregion

#pragma region Public Functionality
void SceneObject::SetPosition(const Structure::Vector2<int>& _position)
{
	m_position = _position;
	m_positionF.m_x = static_cast<float>(_position.m_x);
	m_positionF.m_y = static_cast<float>(_position.m_y);
}
#pragma endregion

#pragma region Destruction
void SceneObject::Denitialize()
{
	m_isActive = false;

	sp_objectManager->RemoveFromSceneObjects(this);
}
#pragma endregion