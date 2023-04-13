#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Consts.h"
#include "DList.h"
#include "Enums.h"
#include "Tools.h"

namespace Structure
{
	// ???: Forward declaration is not working for some reason

	struct Vector2
	{
	public:
		// Member Variables
		int m_x;
		int m_y;

		// Initialization
		inline Vector2() : m_x(Consts::NO_VALUE), m_y(Consts::NO_VALUE) { return; }
		inline Vector2(int _x, int _y) : m_x(_x), m_y(_y) { return; }
		Vector2(const Vector2&) = default;

		// Functionality
		inline void operator=(const Vector2& _rhs) { m_x = _rhs.m_x; m_y = _rhs.m_y; }
		inline bool operator==(const Vector2& _rhs) { return (m_x == _rhs.m_x && m_y == _rhs.m_y); }
	};
	struct Generic
	{
	public:
		// Member Variables
		int m_int;

		// Initialization
		Generic() = default;
		Generic(const Generic&) = default;
		Generic& operator=(const Generic&) = delete;
	};
	struct CollisionRenderInfo
	{
	public:
		// Member Variables
		char m_char;
		Enums::ObjectType m_objectType;
		int m_value;
		unsigned short m_color;
		Vector2 m_position;

		// Initialization
		CollisionRenderInfo() = default;
		CollisionRenderInfo(const CollisionRenderInfo&) = default;
		CollisionRenderInfo& operator=(const CollisionRenderInfo&) = delete;
	};
	struct Input
	{
	public:
		// Member Variables
		Enums::InputPressState m_inputPressState;
		int m_inputIndex;

		// Initialization
		Input() = default;
		Input(const Input&) = default;
		Input& operator=(const Input&) = default;
	};
}

#endif STRUCTURE_H