#ifndef ENUMS_H
#define ENUMS_H

namespace Enums
{
	enum class Color { Black, Blue, Green, LightBlue, Magenta, Red, White, Yellow, NumberOfColors };
	enum class CellState { Collision, Empty, Food, Snake };
	enum class CollisionType { HeadToBody, HeadToFruit, HeadToHead };
	enum class HostType { Client, NA, Server };
	enum class InputName { Accept, Down, Left, Pause, Right, Up, NumberOfNames, NA };
	enum class InputPressState { Click, Dead, Held, PressedThisFrame, Released };
	enum class InputType { MenuCharInput, Normal, ToNorm };
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
		PlayGameMulti = 21,
		PlayGameSingle = 22, 
		ResumeGame = 23, 
		Return = 24, 
		RunAsClient = 25,
		RunAsServer = 26,
		SaveGame = 27, 
		SaveHighScores = 28, 
		ShowHighScores = 29,
		StartNetworking = 30,
		StopHost = 31,
		StopNetworking = 32;
	};
	class GameActivity : public MenuReturn
	{
	public:
		static constexpr int
			ExitToResults = 33,
			GameLocalOrServer = 34,
			GameMultiClient = 35,
			Menu = 36,
			PauseGame = 37;
	};
	enum class ObjectType { Avatar, Food, NoTouchy, Snake, NumberOfTypes, NA };
	enum class SpawnState { Selected, Spawned, WaitingSelection };
}

#endif ENUMS_H