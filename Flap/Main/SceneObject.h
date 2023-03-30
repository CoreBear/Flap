#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "RenderObject.h"
#include "Structure.h"

class ObjectManager;

class SceneObject : public RenderObject
{
public:
	// Static Initialization
	static void AssignObjectManager(ObjectManager& _objectManager) { sp_objectManager = &_objectManager; }

	// Initialization
	virtual void Initialize(const Structure::Generic& _genericContainer);
	SceneObject();

	// Updates
	virtual void FixedUpdate() { return; }
	virtual void Update() { return; }

	// Functionality
	inline bool IsActive() { return m_isActive; }
	void SetPosition(const Structure::Vector2<int>& _position);

	// Destruction
	virtual void Denitialize();
	virtual ~SceneObject() override { return; }

protected:
	// Static Variables
	static ObjectManager* sp_objectManager;

	// Member Variables
	Structure::Vector2<float> m_position;

private:
	// Member Variables
	bool m_isActive;
};

#endif SCENE_OBJECT_H