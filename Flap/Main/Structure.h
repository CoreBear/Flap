#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Consts.h"
#include "Enums.h"

namespace Structure
{
	struct Generic
	{

	};
	struct Input
	{
	public:
		Enums::InputPressState m_inputPressState;
		unsigned short m_input;
	};
	template<typename T>
	struct Vector2
	{
	public:
		// Member Variables
		T m_x;
		T m_y;

		// Initialization
		Vector2() : m_x(Consts::NO_VALUE), m_y(Consts::NO_VALUE) { return; }
		Vector2(T _x, T _y) : m_x(_x), m_y(_y) { return; }
	};
}

#endif STRUCTURE_H