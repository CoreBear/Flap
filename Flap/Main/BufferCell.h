#ifndef BUFFER_CELL_H
#define BUFFER_CELL_H

#include "Structure.h"

class BufferCell final
{
public:
	// Static Variables
	static constexpr int MAX_NUMBER_OF_OBJECTS_IN_CELL = 2;

	// Member Variables
	Structure::CollisionRenderInfo* mp_collisionRenderInfo[MAX_NUMBER_OF_OBJECTS_IN_CELL];
	char m_character;
	int m_objectInCellIndex;
	unsigned short m_colorBFGround;
	Structure::Vector2<int> m_position;
	void* mp_voidSceneObject[MAX_NUMBER_OF_OBJECTS_IN_CELL];

	// Initialization
	BufferCell() = default;
	BufferCell(const BufferCell&) = delete;
	BufferCell& operator=(const BufferCell&) = delete;

	// Functionality
	void ResetCell();
};

#endif BUFFER_CELL_H