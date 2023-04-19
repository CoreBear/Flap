#ifndef SPAWN_MANAGER_H
#define SPAWN_MANAGER_H

#include "Structure.h"

class ObjectManager;
namespace Enums { enum class ObjectType; }
class SharedGame;

class SpawnManager final
{
public:
	// Initialization
	SpawnManager(ObjectManager& _objectManager, SharedGame& _sharedGame);
	SpawnManager(const SpawnManager&) = delete;
	SpawnManager& operator=(const SpawnManager&) = delete;

	// Updates
	void FixedUpdate();

	// Functionality
	void Start(bool _newGame);

private:
	// Member Variables
	Structure::Generic m_genericContainer;
	int m_numberOfPlayersIndex;
	int m_reusableIterator;
	int m_spawnTargetFrame;
	ObjectManager& mr_objectManager;
	SharedGame& mr_sharedGame;
	Structure::Vector2 m_randomPosition;

	// Functionality
	void SpawnFood();
	void SpawnNoTouchy();
};

#endif SPAWN_MANAGER_H