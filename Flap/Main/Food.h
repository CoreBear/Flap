#ifndef FOOD_H
#define FOOD_H

#include "SceneObject.h"

namespace Structure { struct CollisionRenderInfo; };
namespace Structure { struct Generic; };

class Food final : public SceneObject
{
public:
	// Initialization
	Food();
	Food(const Food&) = delete;
	Food& operator=(const Food&) = delete;
	void Initialize(const Structure::Generic* const _genericContainer) override;

	// Updates
	void FixedUpdate() override { WriteIntoFrameBufferCell(m_collisionRenderInfo); }

	// Functionality
	inline bool Collision_IsDead(const Structure::CollisionRenderInfo& _collisionRenderInfo, bool _collidedWithSelf = false) { Denitialize(true); return true; }

private:
	// Member Variables
	BufferCell* mp_bufferCell;
	Structure::CollisionRenderInfo m_collisionRenderInfo;

	// Functionality
	void UpdateValue(int _value);
};

#endif FOOD_H