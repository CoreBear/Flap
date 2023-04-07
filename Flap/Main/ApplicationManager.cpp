#pragma region Includes
#include "CollisionRenderWriteIntoBuffer.h"
#include "Consts.h"
#include "Enums.h"
#include "GameManager.h"
#include "GameThreadBase.h"
#include "InputManager.h"
#include "SharedCollisionRender.h"
#include "SharedGame.h"
#include "SharedInput.h"

#include <mutex>
#include <thread>
#include <Windows.h>
#pragma endregion

#pragma region Leak Detection
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
#pragma endregion

#pragma region Prototypes
void ManagerThreadEntry(GameThreadBase** const _gameThreadBase, int _threadIndex, SharedGame* _sharedGame);
void SetupConsole(COORD& _bufferSizeCR, HANDLE& _outputWindowHandle);
#pragma endregion

int main()
{
	// Memory leak detection code
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// This function call will set a breakpoint at the location of a leaked block
	// Set the parameter to the identifier for a leaked block
	_CrtSetBreakAlloc(-1);

	// Scope required so std containers go out of scope and clean up their memory
	{
		COORD bufferSizeCR;
		HANDLE outputWindowHandle;

		SetupConsole(bufferSizeCR, outputWindowHandle);

		SharedCollisionRender sharedCollisionRender(bufferSizeCR);
		SharedGame sharedGame;
		SharedInput sharedInput;

		// NOTE/WARNING: CollisionRenderWriteIntoBuffer requires Scene to be created first
		enum class ThreadType { Game, Input, CollisionRenderWriteIntoBuffer, NumberOfTypes };

		// Generate thread objects
		GameThreadBase** gameThreadBases = new GameThreadBase * [static_cast<int>(ThreadType::NumberOfTypes)]
		{
			new GameManager(outputWindowHandle, sharedCollisionRender, sharedGame, sharedInput),
				new InputManager(sharedGame, sharedInput),
				new CollisionRenderWriteIntoBuffer(sharedCollisionRender, sharedGame)
		};

		std::thread threads[static_cast<int>(ThreadType::NumberOfTypes)];

		// Start threads and detach (if applicable)
		for (int threadIndex = Consts::NO_VALUE; threadIndex < static_cast<int>(ThreadType::NumberOfTypes); threadIndex++)
		{
			threads[threadIndex] = std::thread(ManagerThreadEntry, gameThreadBases, threadIndex, &sharedGame);

			if (threadIndex == static_cast<int>(ThreadType::CollisionRenderWriteIntoBuffer))
			{
				threads[threadIndex].detach();
			}
		}

		// Wait for threads
		for (int threadIndex = Consts::NO_VALUE; threadIndex < static_cast<int>(ThreadType::NumberOfTypes); threadIndex++)
		{
			if (threadIndex != static_cast<int>(ThreadType::CollisionRenderWriteIntoBuffer))
			{
				threads[threadIndex].join();
			}
		}

		// Delete thread pointers
		for (int threadIndex = Consts::NO_VALUE; threadIndex < static_cast<int>(ThreadType::NumberOfTypes); threadIndex++)
		{
			delete gameThreadBases[threadIndex];
		}

		// Delete thread container
		delete[] gameThreadBases;
	}

	_CrtDumpMemoryLeaks();
	return 0;
}
void ManagerThreadEntry(GameThreadBase** const _gameThreadBase, int _threadIndex, SharedGame* _sharedGame)
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