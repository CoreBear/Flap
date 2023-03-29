#ifndef ENUMS_H
#define ENUMS_H

namespace Enums
{
	enum class Dimension { Column, Row, NumberOfDimensions };
	enum class InputPressState { Click, Dead, Held, PressedThisFrame, Released };
	enum class ObjectType { Bird, NumberOfTypes };
}

#endif ENUMS_H