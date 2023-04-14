#ifndef NO_TOUCY_H
#define NO_TOUCY_H

#include "SceneObject.h"

namespace Structure { struct CollisionRenderInfo; };
namespace Structure { struct Generic; };

class NoTouchy final : public SceneObject
{
public:
	// Initialization
	void Initialize(const Structure::Generic* const _genericContainer) override;
	NoTouchy();
	NoTouchy(const NoTouchy&) = delete;
	NoTouchy& operator=(const NoTouchy&) = delete;

	// Updates
	void FixedUpdate() override { WriteIntoFrameBufferCell(m_collisionRenderInfo); }

	// Functionality
	inline bool Collision_IsDead(const Structure::CollisionRenderInfo& _collisionRenderInfo, bool _collidedWithSelf = false) { return false; }

private:
	// Member Variables
	Structure::CollisionRenderInfo m_collisionRenderInfo;
};

#endif NO_TOUCY_H