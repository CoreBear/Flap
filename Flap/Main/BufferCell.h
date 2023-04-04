#ifndef BUFFER_CELL_H
#define BUFFER_CELL_H

#include "Enums.h"
#include "DList.h"

class SceneObject;

class BufferCell final
{
public:
	// Static Variables
	static const DList<SceneObject*>::Const_Iterator NULL_ITERATOR;
	static constexpr int MAX_NUMBER_OF_OBJECTS_IN_CELL = 2;

	// Member Variables
	Enums::CellState m_cellState;
	char m_character;
	DList<SceneObject*>::Const_Iterator mp_objectsInCellIterators[MAX_NUMBER_OF_OBJECTS_IN_CELL];
	int m_objectInCellIndex;
	short m_colorBFGround;

	// Initialization
	BufferCell() = default;
	BufferCell(const BufferCell&) = delete;
	BufferCell& operator=(const BufferCell&) = delete;

	// Functionality
	void ResetCell();
};

#endif BUFFER_CELL_H