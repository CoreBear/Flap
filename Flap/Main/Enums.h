#ifndef ENUMS_H
#define ENUMS_H

namespace Enums
{
	enum class Color { Black, Blue, Green, LightBlue, Magenta, Red, White, Yellow, NumberOfColors };
	enum class CellState { Collision, Empty, Food, Snake };
	enum class CollisionType { HeadToBody, HeadToFruit, HeadToHead };
	enum class InputName { Accept, Down, Left, Pause, Right, Up, NumberOfNames, NA };
	enum class InputPressState { Click, Dead, Held, PressedThisFrame, Released };
	enum class InputType { HighScore, Normal };
	class MenuName 
	{ 
	public: 
		static constexpr int
		ClientSearch = 0,		// false
		Exit = 1,				// true
		HighScore = 2,			// false
		LocalMultiplayer = 3, 	// false
		Main = 4, 				// true
		Multiplayer = 5, 		// true
		NewHighScore = 6, 		// false
		Network = 7,			// true
		Options = 8, 			// false
		Pause = 9,				// true
		ResultsMulti = 10, 		// false
		ServerSearch = 11,		// false
		SinglePlayer = 12, 		// false
		Welcome = 13,			// false
		NumberOfMenus = 14,
		NA = 14; 
	};
	class MenuReturn : public MenuName 
	{ 
	public: 
		static constexpr int
		ExitApp = 15, 
		ExitToMain = 16, 
		HighScoreToMain = 17,
		Join = 18,
		LoadGame = 19, 
		PlayGameLocal = 20, 
		PlayGameSingle = 21, 
		ResumeGame = 22, 
		Return = 23, 
		RunAsClient = 24,
		RunAsServer = 25,
		SaveGame = 26, 
		SaveHighScores = 27, 
		ShowHighScores = 28,
		StartNetworking = 29,
		StopHost = 30,
		StopNetworking = 31;
	};
	class GameActivity : public MenuReturn
	{
	public:
		static constexpr int
			ExitToResults = 32,
			Game = 33,
			Menu = 34,
			PauseGame = 35;
	};
	enum class ObjectType { Avatar, Food, NoTouchy, Snake, NumberOfTypes, NA };
	enum class SpawnState { Selected, Spawned, WaitingSelection };
}

#endif ENUMS_H