#ifndef ENUMS_H
#define ENUMS_H

namespace Enums
{
	enum class Dimension { Column, Row, NumberOfDimensions };
	enum class Direction { Down, Left, Right, Up, NumberOfDirections, NA };
	enum class InputPressState { Click, Dead, Held, PressedThisFrame, Released };
	enum class ObjectType { Avatar, Food, Snake, NumberOfTypes, NA };
}

#endif ENUMS_H