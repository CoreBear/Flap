#ifndef SPAWN_MANAGER_H
#define SPAWN_MANAGER_H

#include "Structure.h"

#include <random>

class ObjectManager;
namespace Enums { enum class ObjectType; }
class SharedGame;
class SharedRender;

class SpawnManager final
{
public:
	// Initialization
	SpawnManager(ObjectManager& _objectManager, SharedGame& _sharedGame, SharedRender& _sharedRender);
	SpawnManager(const SpawnManager&) = delete;
	SpawnManager& operator=(const SpawnManager&) = delete;

	// Updates
	void FixedUpdate();

	// Functionality
	void Start();

private:
	// Member Variables
	Structure::Generic m_genericContainer;
	int m_numberOfPlayersIndex;
	int m_reusableIterator;
	int m_spawnTargetFrame;
	ObjectManager& mr_objectManager;
	std::random_device m_random;
	SharedGame& mr_sharedGame;
	SharedRender& mr_sharedRender;
	Structure::Vector2 m_randomPosition;

	// Functionality
	void GenerateRandomPosition();
	void SpawnFood();// Object(Enums::ObjectType _objectType);
	void SpawnNoTouchy();
};

#endif SPAWN_MANAGER_H