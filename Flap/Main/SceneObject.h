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
	SceneObject(Enums::ObjectType _objectType);

	// Updates
	virtual void FixedUpdate() { return; }
	virtual void Update() { return; }

	// Functionality
	virtual void Collision(const SceneObject& _otherCollidingObject) = 0;
	inline const Structure::CollisionRenderInfo& GetCollisionRenderInfoRef() const { return m_collisionRenderInfo; }
	inline bool IsActive() { return m_isActive; }
	void SetPosition(const Structure::Vector2<int>& _position);

	// Destruction
	virtual void Denitialize();

protected:
	// Static Variables
	static ObjectManager* sp_objectManager;

	// Member Variables
	Structure::CollisionRenderInfo m_collisionRenderInfo;
	Structure::Vector2<float> m_realPosition;

private:
	// Member Variables
	bool m_isActive;
};

#endif SCENE_OBJECT_H