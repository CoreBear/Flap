#pragma region Includes
#include "SceneObject.h"

#include "ObjectManager.h"
#pragma endregion

#pragma region Static Initialization
ObjectManager* SceneObject::sp_objectManager = nullptr;

SceneObject::SceneObject(Structure::CollisionRenderInfo* _collisionRenderInfo, Structure::Generic* _collisionPackage) : mp_collisionRenderInfo(_collisionRenderInfo), mp_collisionPackage(_collisionPackage)
{
	m_isActive = false;
}
#pragma endregion

#pragma region Initialization
void SceneObject::Initialize(const Structure::Generic& _genericContainer)
{
	m_isActive = true;

	sp_objectManager->AddToSceneObjects(this);
}
#pragma endregion

#pragma region Public Functionality
void SceneObject::SetPosition(const Structure::Vector2& _position)
{
	m_position.m_x = _position.m_x;
	m_position.m_y = _position.m_y;
}
#pragma endregion

#pragma region Destruction
void SceneObject::Denitialize()
{
	m_isActive = false;

	sp_objectManager->RemoveFromSceneObjects(this);
}
SceneObject::~SceneObject() 
{ 
	delete mp_collisionPackage;
	delete mp_collisionRenderInfo; 
}
#pragma endregion