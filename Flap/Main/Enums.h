#ifndef ENUMS_H
#define ENUMS_H

namespace Enums
{
	enum class CellState { Collision, Empty, Food, Snake };
	enum class CollisionType { HeadToBody, HeadToFruit, HeadToHead };
	enum class InputName { Accept, Down, Left, Pause, Right, Up, NumberOfNames, NA };
	enum class InputPressState { Click, Dead, Held, PressedThisFrame, Released };
	enum class InputSceneType { Game, GTO, OTG, Menu };
	enum class ObjectType { Avatar, Food, Snake, NumberOfTypes, NA };
	enum class MenuName { Exit, LocalMultiplayer, Main, Multiplayer, Network, Options, Pause, Results, SinglePlayer, Welcome, NumberOfMenus, NA };
	enum class SpawnState { Selected, Spawned, WaitingSelection };
}

#endif ENUMS_H