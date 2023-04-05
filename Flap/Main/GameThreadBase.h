#ifndef GAME_THREAD_BASE_H
#define GAME_THREAD_BASE_H

class GameThreadBase
{
public:
	// Initialization
	GameThreadBase(const GameThreadBase&) = delete;
	GameThreadBase& operator=(const GameThreadBase&) = delete;

	// Updates
	virtual void Update() { return; }

	// Destruction
	virtual ~GameThreadBase() = default;

protected:
	// Initialization
	GameThreadBase() = default;
};

#endif GAME_THREAD_BASE_H