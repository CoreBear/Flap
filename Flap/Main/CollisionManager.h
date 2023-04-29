#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "Consts.h"

#include <mutex>

class BufferCell;
class SharedGame;
class SceneObject;

class CollisionManager final
{
public:
	// Initialization
	CollisionManager(SharedGame& _sharedGame);
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
	SharedGame& mr_sharedGame;
	std::unique_lock<std::mutex> m_frameBufferUniqueLock;

	// Functionality
	void UpdateCell(BufferCell& _bufferCell);
	CollisionType UpdateColliders(BufferCell& _bufferCell);
};

#endif COLLISION_MANAGER_H