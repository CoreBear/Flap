#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "Consts.h"

class BufferCell;
class SharedRender;
class SceneObject;

class CollisionManager final
{
public:
	// Initialization
	inline CollisionManager(SharedRender& _sharedRender) : mr_sharedRender(_sharedRender) { return; }
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;

	// Updates
	void FixedUpdate();

private:
	// Static Variables
	enum class CollisionType { HeadOnOrSelf, ObjectOneSurvived, ObjectTwoSurvived };
	static constexpr int FIRST_OBJECT_INDEX = Consts::NO_VALUE;
	static constexpr int SECOND_OBJECT_INDEX = Consts::OFF_BY_ONE;

	// Member Variables
	int m_reusableIterator;
	SceneObject* mp_firstSceneObject;
	SceneObject* mp_secondSceneObject;
	SharedRender& mr_sharedRender;

	// Functionality
	void UpdateCell(BufferCell& _bufferCell);
	CollisionType UpdateColliders(BufferCell& _bufferCell);
};

#endif COLLISION_MANAGER_H