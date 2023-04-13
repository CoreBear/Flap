#ifndef ENUMS_H
#define ENUMS_H

namespace Enums
{
	enum class Color { Black, Blue, Green, LightBlue, Magenta, Red, White, Yellow, NumberOfColors };
	enum class CellState { Collision, Empty, Food, Snake };
	enum class CollisionType { HeadToBody, HeadToFruit, HeadToHead };
	enum class GameState { Game, ExitApp, ExitToMain, ExitToResults, Menu, PauseGame, ResumeGame, StartGame};
	enum class InputName { Accept, Down, Left, Pause, Right, Up, NumberOfNames, NA };
	enum class InputPressState { Click, Dead, Held, PressedThisFrame, Released };
	enum class ObjectType { Avatar, Food, Snake, NumberOfTypes, NA };
	class MenuName { public: static constexpr int Exit = 0, LocalMultiplayer = 1, Main = 2, Multiplayer = 3, Network = 4, Options = 5, Pause = 6, Results = 7, SinglePlayer = 8, Welcome = 9, NumberOfMenus = 10, NA = 10; };
	class MenuReturn : public MenuName { public: static constexpr int NA = 10, ExitApp = 11, ExitToMain = 12, PlayGame = 13, Resume = 14, Return = 15, Search = 16; };
	enum class SpawnState { Selected, Spawned, WaitingSelection };
}

#endif ENUMS_H