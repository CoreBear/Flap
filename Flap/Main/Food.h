#ifndef FOOD_H
#define FOOD_H

#include "SceneObject.h"

namespace Structure { struct CollisionRenderInfo; };
namespace Structure { struct Generic; };

class Food final : public SceneObject
{
public:
	// Initialization
	inline Food() { m_collisionRenderInfo.m_objectType = Enums::ObjectType::Food; }
	Food(const Food&) = delete;
	Food& operator=(const Food&) = delete;
	void Initialize(const Structure::Generic* const _genericContainer) override;

	// Updates
	void FixedUpdate() override { WriteIntoFrameBufferCell(m_collisionRenderInfo); }

	// Functionality
	inline bool Collision_IsDead(const Structure::CollisionRenderInfo& _collisionRenderInfo, bool _collidedWithSelf = false) { Denitialize(); return true; }

private:
	// Member Variables
	Structure::CollisionRenderInfo m_collisionRenderInfo;

	// Functionality
	void UpdateValue(int _value);
};

#endif FOOD_H