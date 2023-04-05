#pragma region Includes
#include "CollisionRenderObjectToBuffer.h"
#include "Consts.h"
#include "GameThreadBase.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "SharedCollisionRender.h"
#include "SharedInput.h"

#include <thread>
#include <Windows.h>
#pragma endregion

#pragma region Variables
// Static
static bool gs_applicationIsRunning;
#pragma endregion

#pragma region Prototypes
void ManagerThreadEntry(GameThreadBase** const _gameThreadBase, int _threadIndex);
void SetupConsole(COORD& _bufferSizeCR, HANDLE& _outputWindowHandle);
#pragma endregion

int main()
{
	gs_applicationIsRunning = true;
	
	COORD bufferSizeCR;
	HANDLE outputWindowHandle;

	SetupConsole(bufferSizeCR, outputWindowHandle);

	SharedCollisionRender sharedCollisionRender(bufferSizeCR);
	SharedInput sharedInput;

	enum class ThreadType { CollisionRenderObjectToBuffer, Input, Scene, NumberOfTypes };

	// Generate managers
	GameThreadBase** gameThreadBases = new GameThreadBase * [static_cast<int>(ThreadType::NumberOfTypes)]
	{
		new CollisionRenderObjectToBuffer(sharedCollisionRender),
		new InputManager(sharedInput),
		new SceneManager(outputWindowHandle, sharedCollisionRender, sharedInput)
	};

	std::thread threads[static_cast<int>(ThreadType::NumberOfTypes)];

	// Start manager threads and detach (if applicable)
	for (int threadIndex = Consts::NO_VALUE; threadIndex < static_cast<int>(ThreadType::NumberOfTypes); threadIndex++)
	{
		threads[threadIndex] = std::thread(ManagerThreadEntry, gameThreadBases, threadIndex);

		if (threadIndex == static_cast<int>(ThreadType::Input))
		{
			threads[threadIndex].detach();
		}
	}

	// Join manager threads (if applicable)
	for (int threadIndex = Consts::NO_VALUE; threadIndex < static_cast<int>(ThreadType::NumberOfTypes); threadIndex++)
	{
		if (threadIndex != static_cast<int>(ThreadType::Input))
		{
			threads[threadIndex].join();
		}
	}

	// Delete manager pointers
	for (int threadIndex = Consts::NO_VALUE; threadIndex < static_cast<int>(ThreadType::NumberOfTypes); threadIndex++)
	{
		delete gameThreadBases[threadIndex];
	}

	// Delete manager container
	delete[] gameThreadBases;

	return 0;
}
void ManagerThreadEntry(GameThreadBase** const _gameThreadBase, int _threadIndex)
{
	while (gs_applicationIsRunning)
	{
		_gameThreadBase[_threadIndex]->Update();
	}
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