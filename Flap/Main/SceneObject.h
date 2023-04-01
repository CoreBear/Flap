#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "Enums.h"
#include "Structure.h"

class ObjectManager;

class SceneObject
{
public:
	// Static Initialization
	static void AssignObjectManager(ObjectManager& _objectManager) { sp_objectManager = &_objectManager; }

	// Initialization
	virtual void Initialize(const Structure::Generic& _genericContainer);
	SceneObject(Structure::CollisionRenderInfo* _collisionRenderInfo, Structure::Generic* _collisionPackage);

	// Updates
	virtual void FixedUpdate() { return; }
	virtual void Update() { return; }

	// Functionality
	virtual void Collision(const SceneObject& _otherCollidingObject) = 0;
	inline const Structure::Generic* GetCollisionPackagePtr() const { return mp_collisionPackage; }
	inline const Structure::CollisionRenderInfo& GetCollisionRenderInfoRef() const { return *mp_collisionRenderInfo; }
	inline bool IsActive() { return m_isActive; }
	void SetPosition(const Structure::Vector2& _position);

	// Destruction
	virtual void Denitialize();
	~SceneObject();

protected:
	// Static Variables
	static ObjectManager* sp_objectManager;

	// Member Variables
	Structure::CollisionRenderInfo* mp_collisionRenderInfo;
	Structure::Generic* mp_collisionPackage;
	Structure::Vector2 m_position;							

private:
	// Member Variables
	bool m_isActive;
};

#endif SCENE_OBJECT_H