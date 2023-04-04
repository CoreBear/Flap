#pragma region Includes
#include "BufferCell.h"

#include "SceneObject.h"
#pragma endregion

#pragma region Static Initialization
const DList<SceneObject*>::Const_Iterator BufferCell::NULL_ITERATOR;
#pragma endregion

#pragma region Public Functionality
void BufferCell::ResetCell()
{
	m_cellState = Enums::CellState::Empty;
	m_character = Consts::EMPTY_SPACE_CHAR;
	mp_objectsInCellIterators[Consts::NO_VALUE] = NULL_ITERATOR;
	mp_objectsInCellIterators[Consts::OFF_BY_ONE] = NULL_ITERATOR;
	m_objectInCellIndex = Consts::NO_VALUE;
	m_colorBFGround = NULL;
}
#pragma endregion