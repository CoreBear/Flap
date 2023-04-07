#pragma region Includes
#include "MenuManager.h"

#include "BufferCell.h"
#include "ExitMenu.h"
#include "LocalMultiplayerMenu.h"
#include "MainMenu.h"
#include "MenuBase.h"
#include "MultiplayerMenu.h"
#include "NetworkMenu.h"
#include "OptionsMenu.h"
#include "PauseMenu.h"
#include "ResultsMenu.h"
#include "SharedCollisionRender.h"
#include "SharedGame.h"
#include "SharedInput.h"
#include "SinglePlayerMenu.h"
#include "Structure.h"
#include "WelcomeMenu.h"
#pragma endregion

#pragma region Initialization
MenuManager::MenuManager(SharedCollisionRender& _sharedCollisionRender, SharedGame& _sharedGame) :
	mr_sharedCollisionRender(_sharedCollisionRender),
	mr_sharedGame(_sharedGame),
	m_menuUniqueLock(_sharedCollisionRender.m_menuMutex)
{
	m_readIndex = Consts::NO_VALUE;

	mpp_menus = new MenuBase*[static_cast<int>(Enums::MenuName::NumberOfMenus)];

	// For each menu
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < static_cast<int>(Enums::MenuName::NumberOfMenus); m_reusableIterator++)
	{
		switch (m_reusableIterator)
		{
		case Enums::MenuName::Exit:
		{
			mpp_menus[m_reusableIterator] = new ExitMenu();
		}
			break;
		case Enums::MenuName::LocalMultiplayer:
		{
			mpp_menus[m_reusableIterator] = new LocalMultiplayerMenu();
		}
			break;
		case Enums::MenuName::Main:
		{
			mpp_menus[m_reusableIterator] = new MainMenu();
		}
			break;
		case Enums::MenuName::Multiplayer:
		{
			mpp_menus[m_reusableIterator] = new MultiplayerMenu();
		}
			break;
		case Enums::MenuName::Network:
		{
			mpp_menus[m_reusableIterator] = new NetworkMenu();
		}
			break;
		case Enums::MenuName::Options:
		{
			mpp_menus[m_reusableIterator] = new OptionsMenu();
		}
			break;
		case Enums::MenuName::Pause:
		{
			mpp_menus[m_reusableIterator] = new PauseMenu();
		}
			break;
		case Enums::MenuName::Results:
		{
			mpp_menus[m_reusableIterator] = new ResultsMenu();
		}
			break;
		case Enums::MenuName::SinglePlayer:
		{
			mpp_menus[m_reusableIterator] = new SinglePlayerMenu();
		}
			break;
		case Enums::MenuName::Welcome:
		{
			mpp_menus[m_reusableIterator] = new WelcomeMenu();
		}
			break;
		default:
		{
			// NOTE/WARNING: Execution shouldn't make it here
			throw std::exception();
		}
			break;
		}
	}

	DisplayMenu(Enums::MenuName::Welcome);

	// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
	m_menuUniqueLock.unlock();
}
#pragma endregion

#pragma region Updates
void MenuManager::FixedUpdate()
{
	InputReceiver::HandleInput();
}
#pragma endregion

#pragma region Public Functionality
void MenuManager::DisplayMenu(int _menuNameIndex, bool _isReturning)
{
	if (_isReturning)
	{
		m_returnMenuStack.pop();
	}
	else
	{
		if (m_menuCanBeReturnedTo[m_currentMenuIndex])
		{
			m_returnMenuStack.push(m_currentMenuIndex);
		}
	}

	// Toggle on next menu
	m_currentMenuIndex = _menuNameIndex;

	mr_sharedCollisionRender.mp_menu = mpp_menus[m_currentMenuIndex];
}
#pragma endregion

#pragma region Protected Functionality
void MenuManager::InputAccept(Enums::InputPressState _inputPressState)
{
	if (_inputPressState == Enums::InputPressState::PressedThisFrame)
	{
		m_potentialNextMenuIndex = mpp_menus[m_currentMenuIndex]->InputAccept();

		switch (m_potentialNextMenuIndex)
		{
			// Completely exit the application
		case Enums::MenuReturn::ExitApp:
		{
			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::ExitApp;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
			break;

		case Enums::MenuReturn::ExitToMain:
		{
			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::ExitToMain;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
			break;

			// Display nothing and move into the game state
		case Enums::MenuReturn::PlayGame:
		{
			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::StartGame;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
			break;

			// Display nothing and move back into the game state
		case Enums::MenuReturn::Resume:
		{
			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::ResumeGame;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
			break;

			// Return to previous menu
		case Enums::MenuReturn::Return:
		{
			m_menuUniqueLock.lock();
			PreChangeSync();
			DisplayMenu(m_returnMenuStack.top(), true);
			m_menuUniqueLock.unlock();
			PostChangeSync();
		}
		break;

		// Networking search
		case Enums::MenuReturn::Search:
		{
			mr_sharedGame.m_gameStateMutex.lock();
			//mr_sharedGame.m_gameState = Enums::GameState::net;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
			break;

			// Display the next menu
		default:
		{
			m_menuUniqueLock.lock();
			PreChangeSync();
			DisplayMenu(m_potentialNextMenuIndex);
			m_menuUniqueLock.unlock();
			PostChangeSync();
		}
		break;
		}
	}
}
void MenuManager::InputDown(Enums::InputPressState _inputPressState)
{
	if (_inputPressState == Enums::InputPressState::PressedThisFrame)
	{
		m_menuUniqueLock.lock();
		PreChangeSync();
		mpp_menus[m_currentMenuIndex]->InputDown();
		m_menuUniqueLock.unlock();
		PostChangeSync();
	}
}
void MenuManager::InputLeft(Enums::InputPressState _inputPressState)
{
	if (_inputPressState == Enums::InputPressState::PressedThisFrame)
	{
		m_menuUniqueLock.lock();
		PreChangeSync();
		mpp_menus[m_currentMenuIndex]->InputLeft();
		m_menuUniqueLock.unlock();
		PostChangeSync();
	}
}
void MenuManager::InputRight(Enums::InputPressState _inputPressState)
{
	if (_inputPressState == Enums::InputPressState::PressedThisFrame)
	{
		m_menuUniqueLock.lock();
		PreChangeSync();
		mpp_menus[m_currentMenuIndex]->InputRight();
		m_menuUniqueLock.unlock();
		PostChangeSync();
	}
}
void MenuManager::InputUp(Enums::InputPressState _inputPressState)
{
	if (_inputPressState == Enums::InputPressState::PressedThisFrame)
	{
		m_menuUniqueLock.lock();
		PreChangeSync();
		mpp_menus[m_currentMenuIndex]->InputUp();
		m_menuUniqueLock.unlock();
		PostChangeSync();
	}
}
#pragma endregion

#pragma region Private Functionality
void MenuManager::PostChangeSync()
{
	m_menuUniqueLock.lock();

	// If Buffer thread is already waiting
	if (mr_sharedCollisionRender.m_threadWaitingFlag)
	{
		// Release it
		mr_sharedCollisionRender.m_menuConVar.notify_one();

		// Notify the system that nothing is waiting (when this is unlocked and the Buffer thread grabs the lock)
		mr_sharedCollisionRender.m_threadWaitingFlag = false;
	}

	// If Buffer thread is not already waiting
	else
	{
		// Notify system that this (Scene) thread is waiting
		mr_sharedCollisionRender.m_threadWaitingFlag = true;

		// When Render thread releases this (Scene) thread
		mr_sharedCollisionRender.m_menuConVar.wait(m_menuUniqueLock);
	}

	m_menuUniqueLock.unlock();
}
void MenuManager::PreChangeSync()
{
	// If Buffer thread is already waiting
	if (mr_sharedCollisionRender.m_threadWaitingFlag)
	{
		// Release it
		mr_sharedCollisionRender.m_menuConVar.notify_one();

		// Notify the system that nothing is waiting (when this is unlocked and the Buffer thread grabs the lock)
		mr_sharedCollisionRender.m_threadWaitingFlag = false;
	}

	// If Buffer thread is not already waiting
	else
	{
		// Notify system that this (Scene) thread is waiting
		mr_sharedCollisionRender.m_threadWaitingFlag = true;

		// When Buffer thread releases this (Scene) thread
		mr_sharedCollisionRender.m_menuConVar.wait(m_menuUniqueLock);

		// Let it know it can release, because this (Scene) thread needs to go first
		mr_sharedCollisionRender.m_menuConVar.notify_one();
	}
}
#pragma endregion

#pragma region Destruction
MenuManager::~MenuManager()
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < static_cast<int>(Enums::MenuName::NumberOfMenus); m_reusableIterator++)
	{
		delete mpp_menus[m_reusableIterator];
	}

	delete[] mpp_menus;
}
#pragma endregion