#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "RenderObject.h"
#include "Structure.h"

class ObjectManager;

class SceneObject : public RenderObject
{
public:
	// Initialization
	virtual void Initialize(const Structure::Generic& _genericContainer);
	SceneObject();

	// Updates
	virtual void FixedUpdate() { return; }
	virtual void Update() { return; }

	// Functionality
	static void AssignObjectManager(ObjectManager& _objectManager) { sp_objectManager = &_objectManager; }
	inline const Structure::Vector2<int>& GetConstPositionRef() const { return m_position; }
	inline const SpriteInfo& GetConstSpriteInfoRef() const { return m_spriteInfo; }
	inline bool IsActive() { return m_isActive; }
	void SetPosition(const Structure::Vector2<int>& _position);

	// Destruction
	virtual void Denitialize();
	virtual ~SceneObject() { return; }

protected:
	// Member Variables
	Structure::Vector2<int> m_position;
	Structure::Vector2<float> m_positionF;

	// Static Variables
	static ObjectManager* sp_objectManager;

private:
	// Member Variables
	bool m_isActive;
};

#endif SCENE_OBJECT_H