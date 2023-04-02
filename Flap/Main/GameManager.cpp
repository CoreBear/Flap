#pragma region Includes
#include "Consts.h"
#include "InputManager.h"
#include "Manager.h"
#include "NetworkManager.h"
#include "ObjectManager.h"
#include "CollisionRenderManager.h"
#include "SceneManager.h"
#include "SharedMemory.h"

#include <thread>
#include <Windows.h>
#pragma endregion

#pragma region Variables
// Static
static bool gs_applicationIsRunning;
#pragma endregion

#pragma region Prototypes
void ManagerThreadEntry(int _threadIndex, Manager** const _managers);
void SetupConsole(COORD& _bufferSizeCR, HANDLE& _outputWindowHandle);
#pragma endregion

int main()
{
	gs_applicationIsRunning = true;
	
	COORD bufferSizeCR;
	HANDLE outputWindowHandle;

	SetupConsole(bufferSizeCR, outputWindowHandle);

	SharedMemory sharedMemory(bufferSizeCR);

	enum class ManagerType { CollisionRender, Input, /*Network,*/ Scene, NumberOfTypes };

	// Generate managers
	Manager** managers = new Manager * [static_cast<int>(ManagerType::NumberOfTypes)]
	{
		// NOTE/WARNING: windowHandle is not being used in InputManager
		// because this instance was retrieved before the window was active.
		// Leaving this here for posterity
		new CollisionRenderManager(outputWindowHandle, sharedMemory),
		new InputManager(sharedMemory),
		//new NetworkManager(),
		new SceneManager(sharedMemory)
	};

	bool detachThread[static_cast<int>(ManagerType::NumberOfTypes)]{ false, true, false };
	std::thread managerThreads[static_cast<int>(ManagerType::NumberOfTypes)];

	// Start manager threads and detach (if applicable)
	for (int threadIndex = Consts::NO_VALUE; threadIndex < (int)ManagerType::NumberOfTypes; threadIndex++)
	{
		managerThreads[threadIndex] = std::thread(ManagerThreadEntry, threadIndex, managers);

		// Detach if applicable
		if (detachThread[threadIndex])
		{
			managerThreads[threadIndex].detach();
		}
	}

	// Join manager threads (if applicable)
	for (int threadIndex = Consts::NO_VALUE; threadIndex < (int)ManagerType::NumberOfTypes; threadIndex++)
	{
		if (detachThread[threadIndex] == false)
		{
			managerThreads[threadIndex].join();
		}
	}

	// Delete manager pointers
	for (int managerIndex = Consts::NO_VALUE; managerIndex < (int)ManagerType::NumberOfTypes; managerIndex++)
	{
		delete managers[managerIndex];
	}

	// Delete manager container
	delete[] managers;

	return 0;
}
void ManagerThreadEntry(int _threadIndex, Manager** const _managers)
{
	while (gs_applicationIsRunning)
	{
		_managers[_threadIndex]->Update();
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