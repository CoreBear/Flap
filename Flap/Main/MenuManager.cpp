#pragma region Includes
#include "MenuManager.h"

#include "BufferCell.h"
#include "ExitMenu.h"
#include "HighScoreMenu.h"
#include "LocalMultiplayerMenu.h"
#include "MainMenu.h"
#include "MultiplayerMenu.h"
#include "NetworkMenu.h"
#include "NewHighScoreMenu.h"
#include "OptionsMenu.h"
#include "PauseMenu.h"
#include "ResultsMultiMenu.h"
#include "SharedGame.h"
#include "SharedInput.h"
#include "SharedRender.h"
#include "SinglePlayerMenu.h"
#include "Structure.h"
#include "TextLine.h"
#include "WelcomeMenu.h"

#include <mutex>
#pragma endregion

#pragma region Initialization
MenuManager::MenuManager(SharedGame& _sharedGame, SharedRender& _sharedRender) :
	mr_sharedGame(_sharedGame),
	mr_sharedRender(_sharedRender)
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
		case Enums::MenuName::HighScore:
		{
			mpp_menus[m_reusableIterator] = new HighScoreMenu();
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
		case Enums::MenuName::NewHighScore:
		{
			mpp_menus[m_reusableIterator] = new NewHighScoreMenu();
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
		case Enums::MenuName::ResultsMulti:
		{
			mpp_menus[m_reusableIterator] = new ResultsMultiMenu();
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

	ReadyNextMenu(Enums::MenuName::Welcome);
}
#pragma endregion

#pragma region Updates
void MenuManager::FixedUpdate()
{
	InputReceiver::HandleInput();

	WriteMenuIntoFrameBuffer();
}
#pragma endregion

#pragma region Public Functionality
bool MenuManager::PreviousMenuIs(int _menuIndexBeingChecked)
{
	if (m_returnMenuStack.empty())
	{
		return false;
	}
	else
	{
		return (m_returnMenuStack.top() == _menuIndexBeingChecked) ? true : false;
	}
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
			// Do nothing, because a non-button was clicked
		case Enums::MenuReturn::NA:
			break;
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
			ClearReturnMenuStack();

			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::ExitToMain;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
		break;

		case Enums::MenuReturn::HighScoreToMain:
		{
			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::HighScoreToMain;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
		break;

		case Enums::MenuReturn::LoadGame:
		{
			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::LoadGame;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
		break;

		case Enums::MenuReturn::Main:
		{
			ClearReturnMenuStack();
			ReadyNextMenu(m_potentialNextMenuIndex);
		}
		break;

		// Display nothing and move into the game state
		case Enums::MenuReturn::PlayGameLocal:
		{
			ClearReturnMenuStack();

			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::StartGameLocal;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
		break;

		// Display nothing and move into the game state
		case Enums::MenuReturn::PlayGameSingle:
		{
			ClearReturnMenuStack();

			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::StartGameSingle;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
		break;

		// Display nothing and move back into the game state
		case Enums::MenuReturn::Resume:
		{
			ClearReturnMenuStack();

			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::ResumeGame;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
		break;

		// Return to previous menu
		case Enums::MenuReturn::Return:
			ReturnToPreviousMenu();
			break;

		case Enums::MenuReturn::SaveGame:
		{
			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::SaveGame;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
		break;

		case Enums::MenuReturn::SaveHighScores:
		{
			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::SaveHighScores;
			mr_sharedGame.m_gameStateMutex.unlock();
		}
		break;

		case Enums::MenuReturn::ShowHighScores:
		{
			mr_sharedGame.m_gameStateMutex.lock();
			mr_sharedGame.m_gameState = Enums::GameState::ShowHighScores;
			mr_sharedGame.m_gameStateMutex.unlock();
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

		// Display the next menu (if not mentioned above)
		default:
			ReadyNextMenu(m_potentialNextMenuIndex);
			break;
		}
	}
}
void MenuManager::InputDown(Enums::InputPressState _inputPressState)
{
	if (_inputPressState == Enums::InputPressState::PressedThisFrame)
	{
		mpp_menus[m_currentMenuIndex]->InputDown();
	}
}
void MenuManager::InputLeft(Enums::InputPressState _inputPressState)
{
	if (_inputPressState == Enums::InputPressState::PressedThisFrame)
	{
		mpp_menus[m_currentMenuIndex]->InputLeft();
	}
}
void MenuManager::InputRight(Enums::InputPressState _inputPressState)
{
	if (_inputPressState == Enums::InputPressState::PressedThisFrame)
	{
		mpp_menus[m_currentMenuIndex]->InputRight();
	}
}
void MenuManager::InputUp(Enums::InputPressState _inputPressState)
{
	if (_inputPressState == Enums::InputPressState::PressedThisFrame)
	{
		mpp_menus[m_currentMenuIndex]->InputUp();
	}
}
#pragma endregion

#pragma region Private Functionality
void MenuManager::ClearReturnMenuStack()
{
	while (m_returnMenuStack.empty() == false)
	{
		m_returnMenuStack.pop();
	}
}
void MenuManager::ReadyNextMenu(int _menuNameIndex, bool _isReturning)
{
	// If returning to a previous menu
	if (_isReturning)
	{
		m_returnMenuStack.pop();
	}

	// If not returning to a previous menu
	else
	{
		// If menu can be returned to
		if (m_menuCanBeReturnedTo[m_currentMenuIndex])
		{
			m_returnMenuStack.push(m_currentMenuIndex);
		}

		// Reset next menu's button number
		mpp_menus[_menuNameIndex]->ResetButtonNumber();
	}

	m_currentMenuIndex = _menuNameIndex;
	mpp_menus[_menuNameIndex]->Initialize();
}
void MenuManager::WriteMenuIntoFrameBuffer()
{
	mr_sharedRender.m_frameBufferMutex.lock();

	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mpp_menus[m_currentMenuIndex]->m_numberOfTextLines; m_reusableIterator++)
	{
		WriteTextLineIntoBuffer(mpp_menus[m_currentMenuIndex]->GetCurrentButtonNumber() == m_reusableIterator, *mpp_menus[m_currentMenuIndex]->mp_textLines[m_reusableIterator]);
	}

	for (m_menuCellsIterator = mpp_menus[m_currentMenuIndex]->m_cells.Begin(); m_menuCellsIterator != mpp_menus[m_currentMenuIndex]->m_cells.End(); ++m_menuCellsIterator)
	{
		mp_bufferCell = &mr_sharedRender.mp_frameBuffer[((*m_menuCellsIterator)->m_position.m_y * mr_sharedRender.m_bufferHW.X) + (*m_menuCellsIterator)->m_position.m_x];
		mp_bufferCell->m_character = (*m_menuCellsIterator)->m_character;
		mp_bufferCell->m_colorBFGround = (*m_menuCellsIterator)->m_colorBFGround;
	}

	mr_sharedRender.m_frameBufferMutex.unlock();
	mr_sharedRender.m_frameBufferConVar.notify_one();
}
void MenuManager::WriteTextLineIntoBuffer(bool _highlightLine, const TextLine& _textLine)
{
	// NOTE: I know I can use strlen, but I wanted to do it this way. I'll use strlen in Tools

	mp_walker = _textLine.m_text;
	m_textLetterColumnPosition = _textLine.m_position.m_x;

	// If option should be highlighted, else not
	m_lineColor = (_highlightLine) ?
	Consts::BACKGROUND_COLORS[static_cast<int>(Enums::Color::White)] | Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::Black)] :
	Consts::BACKGROUND_COLORS[static_cast<int>(Enums::Color::Black)] | Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)] ;

	constexpr char const END_OF_STRING = '\0';
	while (*mp_walker != END_OF_STRING)
	{
		mp_bufferCell = &mr_sharedRender.mp_frameBuffer[(_textLine.m_position.m_y * mr_sharedRender.m_bufferHW.X) + m_textLetterColumnPosition++];
		mp_bufferCell->m_character = *mp_walker;
		mp_bufferCell->m_colorBFGround = m_lineColor;

		++mp_walker;
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