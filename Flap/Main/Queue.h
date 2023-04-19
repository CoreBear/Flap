#ifndef QUEUE_H
#define QUEUE_H

#include "DList.h"

template<typename Type>
class Queue final : public DList<Type>
{
public:
	// Functionality
	void PopFront()
	{
		if (DList<Type>::m_head != nullptr)
		{
			m_temp = DList<Type>::m_head;

			DList<Type>::m_head = DList<Type>::m_head->m_next;

			DList<Type>::m_head->m_previous = nullptr;

			delete m_temp;
		}
	}

private:
	DList<Type>::Node* m_temp;
};

#endif QUEUE_H