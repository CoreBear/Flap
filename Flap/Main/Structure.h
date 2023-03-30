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
	struct Input
	{
	public:
		Enums::InputPressState m_inputPressState;
		int m_inputIndex;;
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
	struct SpriteInfo
	{
	public:
		// Container
		struct BodyNode
		{
		public:
			// Member Variables
			Enums::Direction m_directionToNextPosition;
			int m_numberOfConnectorsToNextNode;
			Vector2<int> m_position;

			// Initialization
			BodyNode() : // Used for the head
				m_directionToNextPosition(Enums::Direction::NA), 
				m_numberOfConnectorsToNextNode(Consts::NO_VALUE)
			{
				m_position.m_x = Consts::NO_VALUE;
				m_position.m_y = Consts::NO_VALUE;
			}
			BodyNode(int _x, int _y) :	// Used for a kink/link/turn
				m_directionToNextPosition(Enums::Direction::NA),
				m_numberOfConnectorsToNextNode(Consts::NO_VALUE)
			{
				m_position.m_x = _x;
				m_position.m_y = _y;
			}
		};

		// Member Variables
		short m_color;
		std::list<BodyNode*> m_bodyNodes;
		std::list<BodyNode*>::const_iterator m_lastNode;

		// Initialization
		SpriteInfo() : m_color(10)
		{
			// Create new head
			m_bodyNodes.push_back(new BodyNode);
		}

		// Functionality
		void AddNewBodyNode(BodyNode& _bodyNode)
		{
			m_lastNode = m_bodyNodes.end();

			_bodyNode.m_numberOfConnectorsToNextNode = _bodyNode.m_position.m_x - (*m_lastNode)->m_position.m_x;

			// If no differences on the x-axis
			if (_bodyNode.m_numberOfConnectorsToNextNode == Consts::NO_VALUE)
			{
				// Then it's a difference on the y-axis
				_bodyNode.m_numberOfConnectorsToNextNode = _bodyNode.m_position.m_y - (*m_lastNode)->m_position.m_y;

				// If value is negative
				if (_bodyNode.m_numberOfConnectorsToNextNode < Consts::NO_VALUE)
				{
					_bodyNode.m_directionToNextPosition = Enums::Direction::Up;
					_bodyNode.m_numberOfConnectorsToNextNode = -_bodyNode.m_numberOfConnectorsToNextNode;
				}

				// If value is positive
				else
				{
					_bodyNode.m_directionToNextPosition = Enums::Direction::Down;
				}
			}
			else
			{

				// If value is negative
				if (_bodyNode.m_numberOfConnectorsToNextNode < Consts::NO_VALUE)
				{
					_bodyNode.m_directionToNextPosition = Enums::Direction::Left;
					_bodyNode.m_numberOfConnectorsToNextNode = -_bodyNode.m_numberOfConnectorsToNextNode;
				}

				// If value is positive
				else
				{
					_bodyNode.m_directionToNextPosition = Enums::Direction::Right;
				}
			}

			m_bodyNodes.push_back(&_bodyNode);
		}

		// Destruction
		~SpriteInfo()
		{
			while (m_bodyNodes.empty() == false)
			{
				delete m_bodyNodes.front();
				m_bodyNodes.pop_front();
			}
		}
	};
}

#endif STRUCTURE_H