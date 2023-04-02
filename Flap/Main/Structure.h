#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Consts.h"
#include "Enums.h"

#include <list>

namespace Structure
{
	struct Generic
	{
	public:
		Enums::ObjectType m_objectType;
		int m_int;
		int m_int2;
	};
	struct Vector2
	{
	public:
		// Member Variables
		int m_x;
		int m_y;

		// Initialization
		Vector2() : m_x(Consts::NO_VALUE), m_y(Consts::NO_VALUE) { return; }
		Vector2(int _x, int _y) : m_x(_x), m_y(_y) { return; }

		// Functionality
		inline void operator=(const Vector2& _rhs) { m_x = _rhs.m_x; m_y = _rhs.m_y; }
		inline bool operator==(const Vector2& _rhs) { return (m_x == _rhs.m_x && m_y == _rhs.m_y); }
	};
	class CollisionRenderInfo
	{
	public:
		// Member Variables
		char m_character;
		const Enums::ObjectType OBJECT_TYPE;
		const short COLOR;
		const Vector2& POSITION;

		// Initialization
		inline CollisionRenderInfo(Enums::ObjectType _objectType, Vector2& _position) :
			m_character(Consts::EMPTY_SPACE_CHAR), 
			OBJECT_TYPE(_objectType),
			COLOR(10),
			POSITION(_position)
		{ 
			return; 
		}

		// Destruction
		virtual ~CollisionRenderInfo() = default;
	};
	struct Input
	{
	public:
		Enums::InputPressState m_inputPressState;
		int m_inputIndex;
	};
	class SnakeCollisionRenderInfo : public CollisionRenderInfo
	{
	public:
		// Member Variables
		const std::list<Vector2>& LIST_OF_BODY_POSITIONS;

		// Initialization
		inline SnakeCollisionRenderInfo(std::list<Vector2>& _listOfBodyPositions, Enums::ObjectType _objectType, Vector2& _position) : CollisionRenderInfo(_objectType, _position), LIST_OF_BODY_POSITIONS(_listOfBodyPositions) { return; }
	};
}

#endif STRUCTURE_H