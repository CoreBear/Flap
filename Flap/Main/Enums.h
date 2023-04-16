#ifndef ENUMS_H
#define ENUMS_H

namespace Enums
{
	enum class Color { Black, Blue, Green, LightBlue, Magenta, Red, White, Yellow, NumberOfColors };
	enum class CellState { Collision, Empty, Food, Snake };
	enum class CollisionType { HeadToBody, HeadToFruit, HeadToHead };
	enum class GameState { Game, ExitApp, ExitToMain, ExitToResults, LoadGame, Menu, PauseGame, ResumeGame, SaveGame, SaveHighScores, StartGameLocal, StartGameSingle };
	enum class InputName { Accept, Down, Left, Pause, Right, Up, NumberOfNames, NA };
	enum class InputPressState { Click, Dead, Held, PressedThisFrame, Released };
	enum class ObjectType { Avatar, Food, NoTouchy, Snake, NumberOfTypes, NA };
	class MenuName { public: static constexpr int Exit = 0, LocalMultiplayer = 1, Main = 2, Multiplayer = 3, Network = 4, Options = 5, Pause = 6, ResultsMulti = 7, ResultsSingle = 8, SinglePlayer = 9, Welcome = 10, NumberOfMenus = 11, NA = 11; };
	class MenuReturn : public MenuName { public: static constexpr int NA = 11, ExitApp = 12, ExitToMain = 13, LoadGame = 14, PlayGameLocal = 15, PlayGameSingle = 16, Resume = 17, Return = 18, SaveGame = 19, SaveHighScores = 20, Search = 21; };
	enum class SpawnState { Selected, Spawned, WaitingSelection };
}

#endif ENUMS_H