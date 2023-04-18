#ifndef ENUMS_H
#define ENUMS_H

namespace Enums
{
	enum class Color { Black, Blue, Green, LightBlue, Magenta, Red, White, Yellow, NumberOfColors };
	enum class CellState { Collision, Empty, Food, Snake };
	enum class CollisionType { HeadToBody, HeadToFruit, HeadToHead };
	enum class GameState { Game, ExitApp, ExitToMain, ExitToResults, LoadGame, Menu, PauseGame, ResumeGame, SaveGame, SaveHighScores, ShowHighScores, StartGameLocal, StartGameSingle };
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
		Network = 5, 
		NewHighScore = 6, 
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
		NA = 12, 
		ExitApp = 13, 
		ExitToMain = 14, 
		LoadGame = 15, 
		PlayGameLocal = 16, 
		PlayGameSingle = 17, 
		Resume = 18, 
		Return = 19, 
		SaveGame = 20, 
		SaveHighScores = 21, 
		ShowHighScores = 22,
		Search = 23; 
	};
	enum class ObjectType { Avatar, Food, NoTouchy, Snake, NumberOfTypes, NA };
	enum class SpawnState { Selected, Spawned, WaitingSelection };
}

#endif ENUMS_H