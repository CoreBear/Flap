#pragma region Includes
#include "AtomicMemory.h"

#include "Consts.h"
#include "Enums.h"
#pragma endregion

#pragma region Initialization
AtomicMemory::AtomicMemory(const Structure::Vector2<int>& _bufferSize) : mpp_renderBufferSwapper(nullptr)
{
	m_bufferSize = _bufferSize.m_x * _bufferSize.m_y;
	m_numberOfCharactersToErase = m_bufferSize - Consts::OFF_BY_ONE;

	m_numberOfWindowColumns = _bufferSize.m_x;
	m_numberOfWindowRows = _bufferSize.m_y;

	mpp_renderBuffer = new char[m_bufferSize];
	memset(mpp_renderBuffer, ' ', m_numberOfCharactersToErase);

	mpp_renderBufferScratch = new char[m_bufferSize];
	memset(mpp_renderBufferScratch, ' ', m_numberOfCharactersToErase);
}
#pragma endregion

#pragma region Public Functionality
void AtomicMemory::AddSpriteToScratch(const char** _sprite, int _spriteHeight, const Structure::Vector2<int>& _position)
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < _spriteHeight; m_reusableIterator++)
	{
		memcpy(&mpp_renderBufferScratch[((_position.m_y * m_numberOfWindowColumns) + (m_reusableIterator * m_numberOfWindowColumns)) + _position.m_x], _sprite[m_reusableIterator], strlen(_sprite[m_reusableIterator]));
	}
}
void AtomicMemory::SwapBuffersAndClearScratch()
{
	mpp_renderBufferSwapper = mpp_renderBuffer;
	mpp_renderBuffer = mpp_renderBufferScratch;
	mpp_renderBufferScratch = mpp_renderBufferSwapper;

	memset(mpp_renderBufferScratch, ' ', m_numberOfCharactersToErase);
}
#pragma endregion

#pragma region Destruction
AtomicMemory::~AtomicMemory() 
{
	delete[] mpp_renderBuffer;
	delete[] mpp_renderBufferScratch;
}
#pragma endregion