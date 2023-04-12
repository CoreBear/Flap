#pragma region Includes
#include "Consts.h"
#include "Defines.h"
#include "Enums.h"
#include "GameManager.h"
#include "GameThreadBase.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "SharedGame.h"
#include "SharedInput.h"
#include "SharedRender.h"

#include <mutex>
#include <thread>
#include <Windows.h>
#pragma endregion

#if LEAK_DETECTION
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif
#endif	// LEAK_DECTION

#pragma region Prototypes
void SetupConsole(COORD& _bufferSizeCR, HANDLE& _outputWindowHandle);
void ThreadEntry(GameThreadBase** const _gameThreadBase, int _threadIndex, SharedGame* _sharedGame);
#pragma endregion

int main()
{
#if LEAK_DETECTION
	// Memory leak detection code
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// This function call will set a breakpoint at the location of a leaked block
	// Set the parameter to the identifier for a leaked block
	_CrtSetBreakAlloc(-1);
#endif	// LEAK_DECTION

	// Scope required so std containers go out of scope and clean up their memory
	{
		COORD bufferSizeCR;
		HANDLE outputWindowHandle;

		SetupConsole(bufferSizeCR, outputWindowHandle);

		SharedGame sharedGame;
		SharedInput sharedInput;
		SharedRender sharedRender(bufferSizeCR);

		enum class ThreadType { Game, Input, Render, NumberOfTypes };

		// Generate thread objects
		GameThreadBase** gameThreadBases = new GameThreadBase * [static_cast<int>(ThreadType::NumberOfTypes)]
		{
			new GameManager(outputWindowHandle, sharedGame, sharedInput, sharedRender),
			new InputManager(sharedGame, sharedInput),
			new RenderManager(outputWindowHandle, sharedRender)
		};

		std::thread threads[static_cast<int>(ThreadType::NumberOfTypes)];

		// Start threads
		for (int threadIndex = Consts::NO_VALUE; threadIndex < static_cast<int>(ThreadType::NumberOfTypes); threadIndex++)
		{
			threads[threadIndex] = std::thread(ThreadEntry, gameThreadBases, threadIndex, &sharedGame);
		}

		// Wait for threads
		for (int threadIndex = Consts::NO_VALUE; threadIndex < static_cast<int>(ThreadType::NumberOfTypes); threadIndex++)
		{
			threads[threadIndex].join();
		}

		// Delete thread base pointers
		for (int threadIndex = Consts::NO_VALUE; threadIndex < static_cast<int>(ThreadType::NumberOfTypes); threadIndex++)
		{
			delete gameThreadBases[threadIndex];
		}

		// Delete thread container
		delete[] gameThreadBases;
	}

#if LEAK_DETECTION
	_CrtDumpMemoryLeaks();
#endif	// LEAK_DECTION

	return 0;
}
void SetupConsole(COORD& _bufferSizeCR, HANDLE& _outputWindowHandle)
{
	// https://learn.microsoft.com/en-us/windows/console/console-functions

	// Position window in top-left
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, -8, -1, 0, 0, SWP_SHOWWINDOW);

	// NOTE/WARNING: THE ORDER OF THESE TWO ARE IMPORTANT!
	_outputWindowHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	_bufferSizeCR = GetLargestConsoleWindowSize(_outputWindowHandle);

	// Generate max buffer and screen size
	SMALL_RECT windowRect
	{
		static_cast<SHORT>(Consts::NO_VALUE),
		static_cast<SHORT>(Consts::NO_VALUE),
		static_cast<SHORT>(_bufferSizeCR.X - Consts::OFF_BY_ONE),
		static_cast<SHORT>(_bufferSizeCR.Y - Consts::OFF_BY_ONE)
	};
	SetConsoleScreenBufferSize(_outputWindowHandle, _bufferSizeCR);
	SetConsoleWindowInfo(_outputWindowHandle, true, &windowRect);

	// Removes the minimize & maximize options
	{
		LONG windowInfo = GetWindowLong(consoleWindow, GWL_STYLE);
		windowInfo &= ~(WS_MINIMIZEBOX);
		windowInfo &= ~(WS_MAXIMIZEBOX);
		SetWindowLong(consoleWindow, GWL_STYLE, windowInfo);
	}

	// Hides cursor
	{
		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(_outputWindowHandle, &cci);
		cci.bVisible = false;
		SetConsoleCursorInfo(_outputWindowHandle, &cci);
	}
}
void ThreadEntry(GameThreadBase** const _gameThreadBase, int _threadIndex, SharedGame* _sharedGame)
{
	_sharedGame->m_gameStateMutex.lock();
	while (_sharedGame->m_gameState != Enums::GameState::ExitApp)
	{
		_sharedGame->m_gameStateMutex.unlock();
		_gameThreadBase[_threadIndex]->Update();
		_sharedGame->m_gameStateMutex.lock();
	}
	_sharedGame->m_gameStateMutex.unlock();
}