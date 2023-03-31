#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Consts.h"
#include "Enums.h"

#include <list>

namespace Structure
{
	struct Generic
	{

	};
	template<typename T> struct Vector2
	{
	public:
		// Member Variables
		float m_normalizationResult;
		T m_x;
		T m_y;

		// Initialization
		Vector2() : m_normalizationResult(Consts::NO_VALUE_F), m_x(Consts::NO_VALUE), m_y(Consts::NO_VALUE) { return; }
		Vector2(T _x, T _y) : m_normalizationResult(Consts::NO_VALUE_F), m_x(_x), m_y(_y) { return; }
	};
	class CollisionRenderInfo
	{
	public:
		// Member Variables
		const Enums::ObjectType m_objectType;
		const short m_color;
		Vector2<int>& mr_position;

		// Initialization
		inline CollisionRenderInfo(Enums::ObjectType _objectType, Vector2<int>& _position) : m_objectType(_objectType), m_color(10), mr_position(_position) { return; }

		// Destruction
		virtual ~CollisionRenderInfo() { return; }
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
		const std::list<Vector2<int>>& mr_listOfBodyPositions;

		// Initialization
		inline SnakeCollisionRenderInfo(std::list<Vector2<int>>& _listOfBodyPositions, Enums::ObjectType _objectType, Vector2<int>& _position) : CollisionRenderInfo(_objectType, _position), mr_listOfBodyPositions(_listOfBodyPositions) { return; }
	};
}

#endif STRUCTURE_H