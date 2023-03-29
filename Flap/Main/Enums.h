#ifndef ENUMS_H
#define ENUMS_H

namespace Enums
{
	enum class Dimension { Column, Row, NumberOfDimensions };
	enum class ObjectType { Bird, NumberOfTypes };
	enum class ButtonPressState { Click, Dead, Held, PressedThisFrame, Released };
}

#endif ENUMS_H