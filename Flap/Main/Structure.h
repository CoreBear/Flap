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
	class CollisionRenderInfo final
	{
	public:
		// Container
		struct Node
		{
		public:
			// Member Variables
			const Node* m_previous;
			Structure::Vector2<int> m_position;

			// Initialization
			Node() : m_previous(nullptr)
			{
				m_position.m_x = Consts::NO_VALUE;
				m_position.m_y = Consts::NO_VALUE;
			}
			Node(const Node* _previous) : m_previous(_previous)
			{
				m_position.m_x = Consts::NO_VALUE;
				m_position.m_y = Consts::NO_VALUE;
			}
		};

		// Member Variables
		std::list<Node*> m_nodes;
		Enums::ObjectType m_objectType;
		short m_color;

		// Initialization
		CollisionRenderInfo(Enums::ObjectType _objectType) : m_objectType(_objectType), m_color(10)
		{
			// Create new head
			m_nodes.push_back(new Node);

			// It's also the new tail
			m_tailIterator = m_nodes.begin();
		}

		// Updates
		void UpdatePosition(const Vector2<float>& _position)
		{
			m_convertedPosition.m_x = static_cast<int>(_position.m_x);
			m_convertedPosition.m_y = static_cast<int>(_position.m_y);

			UpdatePosition(m_convertedPosition);
		}
		void UpdatePosition(const Vector2<int>& _position)
		{
			// Starting from the tail, forward
			for (m_nodeIterator = m_tailIterator; m_nodeIterator != m_nodes.begin(); --m_nodeIterator)
			{
				// Next position is previous node's current position
				(*m_nodeIterator)->m_position = (*m_nodeIterator)->m_previous->m_position;
			}

			// Update first node's position
			(*m_nodeIterator)->m_position.m_x = _position.m_x;
			(*m_nodeIterator)->m_position.m_y = _position.m_y;
		}

		// Functionality
		void AddTail()
		{
			// Create new tail
			m_nodes.push_back(new Node(*m_tailIterator));

			// Move iterator back to it
			++m_tailIterator;
		}

		// Destruction
		~CollisionRenderInfo()
		{
			while (m_nodes.empty() == false)
			{
				delete m_nodes.front();
				m_nodes.pop_front();
			}
		}

	private:
		// Member Variables
		std::list<Node*>::iterator m_nodeIterator;
		std::list<Node*>::iterator m_tailIterator;
		Structure::Vector2<int> m_convertedPosition;
	};
	struct Input
	{
	public:
		Enums::InputPressState m_inputPressState;
		int m_inputIndex;;
	};
}

#endif STRUCTURE_H