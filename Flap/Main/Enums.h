#ifndef ENUMS_H
#define ENUMS_H

namespace Enums
{
	enum class Color { Black, Blue, Green, LightBlue, Magenta, Red, White, Yellow, NumberOfColors };
	enum class CellState { Collision, Empty, Food, Snake };
	enum class CollisionType { HeadToBody, HeadToFruit, HeadToHead };
	enum class GameState { Game, ExitApp, ExitToMain, ExitToResults, HighScoreToMain, LoadGame, Menu, PauseGame, ResumeGame, RunAsClient, RunAsServer, SaveGame, SaveHighScores, ShowHighScores, StartGameLocal, StartGameSingle, StartNetworking, StopNetworking };
	enum class InputName { Accept, Down, Left, Pause, Right, Up, NumberOfNames, NA };
	enum class InputPressState { Click, Dead, Held, PressedThisFrame, Released };
	enum class InputType { HighScore, Normal };
	class MenuName 
	{ 
	public: 
		static constexpr int
		Exit = 0,
		HighScore = 1,
		LocalMultiplayer = 2, 
		Main = 3, 
		Multiplayer = 4, 
		NewHighScore = 5, 
		Network = 6,
		Options = 7, 
		Pause = 8,
		ResultsMulti = 9, 
		SinglePlayer = 10, 
		Welcome = 11,
		NumberOfMenus = 12,
		NA = 12; 
	};
	class MenuReturn : public MenuName 
	{ 
	public: 
		static constexpr int
		ExitApp = 13, 
		ExitToMain = 14, 
		HighScoreToMain = 15,
		LoadGame = 16, 
		PlayGameLocal = 17, 
		PlayGameSingle = 18, 
		Resume = 19, 
		Return = 20, 
		RunAsClient = 21,
		RunAsServer = 22,
		SaveGame = 23, 
		SaveHighScores = 24, 
		ShowHighScores = 25,
		StartNetworking = 26,
		StopNetworking = 27;
	};
	enum class ObjectType { Avatar, Food, NoTouchy, Snake, NumberOfTypes, NA };
	enum class SpawnState { Selected, Spawned, WaitingSelection };
}

#endif ENUMS_H