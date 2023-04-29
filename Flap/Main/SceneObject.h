#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "Enums.h"
#include "Structure.h"

class BufferCell;
class ObjectManager;
class SharedGame;
class SharedRender;

class SceneObject
{
public:
	// Static Initialization
	static void AssignObjectManager(ObjectManager& _objectManager) { sp_objectManager = &_objectManager; }
	static void AssignSharedGame(SharedGame& _sharedGame) { sp_sharedGame = &_sharedGame; }
	static void AssignSharedRender(SharedRender& _sharedRender) { sp_sharedRender = &_sharedRender; }

	// Initialization
	virtual void Initialize(const Structure::Generic* const _genericContainer);
	SceneObject(const SceneObject&) = delete;
	SceneObject& operator=(const SceneObject&) = delete;

	// Updates
	virtual void FixedUpdate() { return; }
	virtual void Update() { return; }

	// Functionality
	virtual bool Collision_IsDead(const Structure::CollisionRenderInfo& _collisionRenderInfo, bool _collidedWithSelf = false) = 0;
	inline Enums::SpawnState GetSpawenState() const { return m_spawnState; }
	inline virtual void Pause() { return; }
	inline virtual void Resume() { return; }
	void SetPosition(const Structure::Vector2<int>& _position);
	inline void SetSpawnState(Enums::SpawnState _spawnState) { m_spawnState = _spawnState; }

	// Destruction
	void Denitialize();
	virtual void Destroy(bool _cleanScene);
	virtual ~SceneObject() { return; }

protected:
	// Static Variables
	static ObjectManager* sp_objectManager;
	static SharedGame* sp_sharedGame;

	// Member Variables
	Structure::Vector2<int> m_position;			

	// Initialization
	SceneObject() : m_spawnState(Enums::SpawnState::WaitingSelection) { return; }

	// Functionality
	bool CheckPositionValidity(Structure::Vector2<int>& _position);
	void WriteIntoFrameBufferCell(Structure::CollisionRenderInfo& _collisionRenderInfo);

private:
	// Member Variables
	BufferCell* mp_bufferCell;
	static SharedRender* sp_sharedRender;
	Enums::SpawnState m_spawnState;
};

#endif SCENE_OBJECT_H