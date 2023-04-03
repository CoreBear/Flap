#ifndef DLIST_H
#define DLIST_H

#include "Consts.h"

template<typename Type>
class DList
{
public:
	// Container
	class Node
	{
	public:
		// Member Variables
		Node* m_next;
		Node* m_previous;
		Type m_value;

		// Initialization
		Node(Type _value, Node* const _next = nullptr, Node* const _previous = nullptr) : m_next(_next), m_previous(_previous), m_value(_value) { return; }
	};
	class Iterator
	{
	public:
		// Initialization
		inline Iterator(Node* const _node = nullptr) : m_reusableIterator(Consts::NO_VALUE), m_current(_node) { return; }
		inline Iterator(const Iterator& _rhs) : m_reusableIterator(Consts::NO_VALUE), m_current(nullptr) { *this = _rhs; }

		// Functionality
		Iterator& operator=(const Iterator& _rhs)
		{
			if (this != &_rhs)
			{
				m_current = _rhs.m_current;
			}

			return *this;
		}
		Iterator& operator++()	// Pre-fix
		{
			if (m_current != nullptr)
			{
				m_current = m_current->m_next;
			}

			return *this;
		}
		Iterator operator++(int)	// Post-fix
		{
			Iterator copy(m_current);

			if (m_current != nullptr)
			{
				m_current = m_current->m_next;
			}

			return copy;
		}
		Iterator& operator--()	// Pre-fix
		{
			if (m_current != nullptr)
			{
				m_current = m_current->m_previous;
			}

			return *this;
		}
		Iterator operator--(int)	// Post-fix
		{
			Iterator copy(m_current);

			if (m_current != nullptr)
			{
				m_current = m_current->m_previous;
			}

			return copy;
		}
		Iterator& operator+=(int _numberOfIncrements)
		{
			for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < _numberOfIncrements; m_reusableIterator++)
			{
				if (m_current != nullptr)
				{
					m_current = m_current->m_next;
				}
				else
				{
					break;
				}
			}

			return *this;
		}
		inline bool operator==(const Iterator& _rhs) { return (m_current == _rhs.m_current); }
		inline bool operator!=(const Iterator& _rhs) { return (m_current != _rhs.m_current); }
		inline Type& operator*() { return m_current->m_value; }
		inline Type* operator->() { return &m_current->m_value; }

	protected:
		// Member Variables
		Node* m_current;

	private:
		int m_reusableIterator;
	};
	class Const_Iterator final : public Iterator
	{
	public:
		// Functionality
		inline Const_Iterator(Node* const _node = nullptr) : Iterator(_node) { return; }
		inline Const_Iterator(const Iterator& _rhs) : Iterator(_rhs) { return; }
		Const_Iterator& operator=(const Const_Iterator& _rhs)
		{
			if (this != &_rhs)
			{
				Iterator::m_current = _rhs.m_current;
			}

			return *this;
		}

		Const_Iterator& operator++()	// Pre-fix
		{
			if (Iterator::m_current != nullptr)
			{
				Iterator::m_current = Iterator::m_current->m_next;
			}

			return *this;
		}
		Const_Iterator operator++(int)	// Post-fix
		{
			Const_Iterator copy(Iterator::m_current);

			if (Iterator::m_current != nullptr)
			{
				Iterator::m_current = Iterator::m_current->m_next;
			}

			return copy;
		}
		Const_Iterator& operator--()	// Pre-fix
		{
			if (Iterator::m_current != nullptr)
			{
				Iterator::m_current = Iterator::m_current->m_previous;
			}

			return *this;
		}
		Const_Iterator operator--(int)	// Post-fix
		{
			Const_Iterator copy(Iterator::m_current);

			if (Iterator::m_current != nullptr)
			{
				Iterator::m_current = Iterator::m_current->m_previous;
			}

			return copy;
		}
		Const_Iterator& operator+=(int _numberOfIncrements)
		{
			for (Iterator::m_reusableIterator = Consts::NO_VALUE; Iterator::m_reusableIterator < _numberOfIncrements; Iterator::m_reusableIterator++)
			{
				if (Iterator::m_current != nullptr)
				{
					Iterator::m_current = Iterator::m_current->m_next;
				}
				else
				{
					break;
				}
			}

			return *this;
		}
		inline const Type& operator*() const { return Iterator::m_current->m_value; }
		inline const Type* operator->() const { return &Iterator::m_current->m_value; }
	};

	// Initialization
	inline DList() : m_head(nullptr), m_tail(nullptr) { return; }
	inline DList(const DList& _rhs) : m_head(nullptr), m_tail(nullptr) { *this = _rhs; }
	DList& operator=(const DList& _rhs)
	{
		// Make sure it's not copying itself
		if (this != &_rhs)
		{
			// Delete previously allocated memory
			Clear();

			Node* temp = _rhs.m_head;

			// While there's a node in the other list
			while (temp != nullptr)
			{
				PushBack(temp->m_value);

				temp = temp->m_next;
			}
		}

		return *this;
	}

	// Functionality
	inline Iterator Begin() const { return Iterator(m_head); }
	void Clear()
	{
		// If not an empty list
		if (m_head != nullptr)
		{
			// While more than one (1) node in the list, delete from the tail
			while (m_tail != m_head)
			{
				// Move towards the head and delete the node that was just left
				m_tail = m_tail->m_previous;
				delete m_tail->m_next;
			}

			// Delete the last remaining node
			delete m_tail;

			// Reset poitners
			m_head = nullptr;
			m_tail = nullptr;
		}
	}
	inline Iterator End() const { return Iterator(); }
	inline Iterator GetTail() const { return Iterator(m_tail); }
	inline bool IsEmpty() { return (m_head == nullptr); }
	void PushBack(Type _value)
	{
		Node* const newNode = new Node(_value);

		// If not an empty list, add node to the tail
		if (m_tail != nullptr)
		{
			m_tail->m_next = newNode;
			newNode->m_previous = m_tail;
		}

		// If an empty list, make the node the head
		else
		{
			m_head = newNode;
		}

		m_tail = newNode;
	}
	bool Remove(Type _value)
	{
		Node* temp = m_head;

		// While still within the list
		while (temp != nullptr)
		{
			// If node was found
			if (temp->m_value == _value)
			{
				// If removing head
				if (temp == m_head)
				{
					// If more than one thing in list
					if (m_head != m_tail)
					{
						m_head = m_head->m_next;
						m_head->m_previous = nullptr;
					}

					// If only one thing in list
					else
					{
						// Reset poitners
						m_head = nullptr;
						m_tail = nullptr;
					}
				}

				// If removing tail
				else if (temp == m_tail)
				{
					m_tail = m_tail->m_previous;
					m_tail->m_next = nullptr;
				}

				// If removing middle
				else
				{
					// Previous node is pointing around this node to next
					temp->m_previous->m_next = temp->m_next;

					// Next node is pointing around this node to previous
					temp->m_next->m_previous = temp->m_previous;
				}

				// Remove this node
				delete temp;

				// Notify the system that something was removed
				return true;
			}

			// If the node wasn't found, move to the next
			temp = temp->m_next;
		}

		// Notify the system that nothing was removed
		return false;
	}

private:
	// Member Variables
	Node* m_head;
	Node* m_tail;
};

#endif DLIST_H