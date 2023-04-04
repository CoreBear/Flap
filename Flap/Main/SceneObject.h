#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "Enums.h"
#include "Structure.h"

class ObjectManager;

class SceneObject
{
public:
	static void AssignObjectManager(ObjectManager& _objectManager) { sp_objectManager = &_objectManager; }

	// Initialization
	virtual void Initialize(const Structure::Generic* const _genericContainer);
	SceneObject(const SceneObject&) = delete;
	SceneObject& operator=(const SceneObject&) = delete;

	// Updates
	virtual void FixedUpdate() { return; }
	virtual void Update() { return; }

	// Functionality
	virtual void Collision(const Structure::Generic* const _otherCollisionPackage, const Structure::Vector2& _collisionCellCR) = 0;
	inline const Structure::Generic& GetCollisionPackageRef() const { return m_collisionPackage; }
	inline const Structure::RenderInfo& GetRenderInfoRef() const { return *mp_renderInfo; }
	inline bool IsActive() const { return m_isActive; }
	void SetPosition(const Structure::Vector2& _position);

	// Destruction
	virtual void Denitialize();
	~SceneObject();

protected:
	// Static Variables
	static ObjectManager* sp_objectManager;

	// Member Variables
	Structure::Generic m_collisionPackage;
	Structure::RenderInfo* const mp_renderInfo;
	Structure::Vector2 m_position;							

	// Initialization
	SceneObject(Structure::RenderInfo* _renderInfo);

private:
	// Member Variables
	bool m_isActive;
};

#endif SCENE_OBJECT_H