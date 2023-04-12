#ifndef SHARED_RENDER_H
#define SHARED_RENDER_H

#include <mutex>
#include <Windows.h>

class BufferCell;

class SharedRender final
{
public:
	// Member Variables
	int m_bufferSize;	// Has to be up here to initialize before other things
	bool m_somethingToRender;
	BufferCell* const mp_frameBuffer;
	const COORD m_bufferHW;
	std::mutex m_frameBufferMutex;

	// Initialization
	SharedRender(const COORD& _bufferSize);
	SharedRender(const SharedRender&) = delete;
	SharedRender& operator=(const SharedRender&) = delete;

	// Functionality
	void ResetFrameBuffer();
	void ResetFrameBufferSynced();
	
	// Destruction
	~SharedRender();

private:
	int m_reusableIterator;
};

#endif SHARED_RENDER_H