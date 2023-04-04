#ifndef ENUMS_H
#define ENUMS_H

namespace Enums
{
	enum class CellState { Collision, Empty, Food, Snake };
	enum class CollisionType { HeadToBody, HeadToFruit, HeadToHead };
	enum class Direction { Down, Left, Right, Up, NumberOfDirections, NA };
	enum class InputPressState { Click, Dead, Held, PressedThisFrame, Released };
	enum class ObjectType { Avatar, Food, Snake, NumberOfTypes, NA };
	enum class SpawnState { Selected, Spawned, WaitingSelection };
}

#endif ENUMS_H