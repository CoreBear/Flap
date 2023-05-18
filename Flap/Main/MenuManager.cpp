#pragma region Includes
#include "MenuManager.h"

#include "BufferCell.h"
#include "ClientWaitMenu.h"
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
#include "ServerSearchMenu.h"
#include "SharedGame.h"
#include "SharedInput.h"
#include "SharedNetwork.h"
#include "SinglePlayerMenu.h"
#include "Structure.h"
#include "TextLine.h"
#include "WelcomeMenu.h"

#include <mutex>
#pragma endregion

#pragma region Initialization
MenuManager::MenuManager(SharedGame& _sharedGame, SharedNetwork& _sharedNetwork) :
	mr_sharedGame(_sharedGame)
{
	m_readIndex = Consts::NO_VALUE;

	mpp_menus = new MenuBase * [static_cast<int>(Enums::MenuName::NumberOfMenus)];

	// For each menu
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < static_cast<int>(Enums::MenuName::NumberOfMenus); m_reusableIterator++)
	{
		switch (m_reusableIterator)
		{
		case Enums::MenuName::ClientSearch:
		{
			mpp_menus[m_reusableIterator] = new ClientWaitMenu(_sharedNetwork);
		}
		break;
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
			mpp_menus[m_reusableIterator] = new NetworkMenu(_sharedNetwork);
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
		case Enums::MenuName::ServerSearch:
		{
			mpp_menus[m_reusableIterator] = new ServerSearchMenu(_sharedNetwork);
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
}
#pragma endregion

#pragma region Updates
void MenuManager::FixedUpdate()
{
	InputReceiver::HandleInput();

	mpp_menus[m_currentMenuIndex]->FixedUpdate();

	WriteMenuIntoFrameBuffer();
}
#pragma endregion

#pragma region Public Functionality
void MenuManager::DisplayMenu(int _menuNameIndex, bool _isReturning)
{
	// If switching to a different menu
	if (_menuNameIndex != m_currentMenuIndex)
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
		mpp_menus[m_currentMenuIndex]->Initialize();
	}
}
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
		m_nextActivityOrMenuIndex = mpp_menus[m_currentMenuIndex]->InputAccept();

		switch (m_nextActivityOrMenuIndex)
		{
			// Do nothing, because a non-button was clicked
		case Enums::MenuReturn::NA:
			break;

		case Enums::MenuReturn::ExitToMain:
		case Enums::MenuReturn::PlayGameLocal:
		case Enums::MenuReturn::PlayGameSingle:
		case Enums::MenuReturn::ResumeGame:
			ClearReturnMenuStack();

			// NOTE: Notice the fallthrough!
		case Enums::MenuReturn::ExitApp:
		case Enums::MenuReturn::HighScoreToMain:
		case Enums::MenuReturn::Join:
		case Enums::MenuReturn::LoadGame:
		case Enums::MenuReturn::RunAsClient:
		case Enums::MenuReturn::RunAsServer:
		case Enums::MenuReturn::SaveGame:
		case Enums::MenuReturn::SaveHighScores:
		case Enums::MenuReturn::ShowHighScores:
		case Enums::MenuReturn::StartNetworking:
		case Enums::MenuReturn::StopHost:
		case Enums::MenuReturn::StopNetworking:
		{
			mr_sharedGame.m_gameActivityIndexMutex.lock();
			mr_sharedGame.m_gameActivityIndex = m_nextActivityOrMenuIndex;
			mr_sharedGame.m_gameActivityIndexMutex.unlock();
		}
		break;

		case Enums::MenuReturn::Main:
		{
			ClearReturnMenuStack();

			DisplayMenu(m_nextActivityOrMenuIndex);
		}
		break;

		// Return to previous menu
		case Enums::MenuReturn::Return:
			ReturnToPreviousMenu();
			break;

			// Display the next menu (if not mentioned above)
		default:
			DisplayMenu(m_nextActivityOrMenuIndex);
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
void MenuManager::WriteMenuIntoFrameBuffer()
{
	mr_sharedGame.m_frameBufferMutex.lock();

	mr_sharedGame.ResetFrameBuffer();

	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mpp_menus[m_currentMenuIndex]->m_currNumOfTextLines; m_reusableIterator++)
	{
		WriteTextLineIntoBuffer(mpp_menus[m_currentMenuIndex]->GetCurrentButtonNumber() == m_reusableIterator, *mpp_menus[m_currentMenuIndex]->mp_textLines[m_reusableIterator]);
	}

	for (m_menuCellsIterator = mpp_menus[m_currentMenuIndex]->m_cells.Begin(); m_menuCellsIterator != mpp_menus[m_currentMenuIndex]->m_cells.End(); ++m_menuCellsIterator)
	{
		mp_bufferCell = &mr_sharedGame.mpp_frameBuffer[(*m_menuCellsIterator)->m_position.m_y][(*m_menuCellsIterator)->m_position.m_x];
		mp_bufferCell->m_character = (*m_menuCellsIterator)->m_character;
		mp_bufferCell->m_colorBFGround = (*m_menuCellsIterator)->m_colorBFGround;
	}

	mr_sharedGame.m_frameBufferMutex.unlock();
	mr_sharedGame.m_rendererConVar.notify_one();
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
		mp_bufferCell = &mr_sharedGame.mpp_frameBuffer[_textLine.m_position.m_y][m_textLetterColumnPosition++];
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