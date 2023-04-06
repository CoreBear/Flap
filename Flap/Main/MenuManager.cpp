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
#include "SharedInput.h"
#include "SinglePlayerMenu.h"
#include "Structure.h"
#include "WelcomeMenu.h"
#pragma endregion

#pragma region Initialization
MenuManager::MenuManager(SharedCollisionRender& _sharedCollisionRender) :
	mr_sharedCollisionRender(_sharedCollisionRender)
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
}
#pragma endregion

#pragma region Updates
void MenuManager::FixedUpdate()
{
	InputReceiver::HandleInput();

	WriteMenuIntoBuffer();
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
			break;

		case Enums::MenuReturn::ExitToMain:
			break;

			// Display nothing and move into the game state
		case Enums::MenuReturn::PlayGame:
			break;

			// Display nothing and move back into the game state
		case Enums::MenuReturn::Resume:
			break;

			// Return to previous menu
		case Enums::MenuReturn::Return:
		{
			DisplayMenu(m_returnMenuStack.top(), true);
			m_returnMenuStack.pop();
		}
			break;

			// Networking search
		case Enums::MenuReturn::Search:
			break;

			// Display the next menu
		default:
			DisplayMenu(m_potentialNextMenuIndex);
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
void MenuManager::DisplayMenu(int _menuNameIndex, bool _isReturning)
{
	// Toggle off current menu
	if (_isReturning)
	{

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
}
void MenuManager::WriteMenuIntoBuffer()
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mpp_menus[m_currentMenuIndex]->m_numberOfTextLines; m_reusableIterator++)
	{
		WriteTextLineIntoBuffer(mpp_menus[m_currentMenuIndex]->GetCurrentButtonNumber() == m_reusableIterator, *mpp_menus[m_currentMenuIndex]->mp_textLines[m_reusableIterator]);
	}
}
void MenuManager::WriteTextLineIntoBuffer(bool _highlightLine, const Structure::TextLine& _textLine)
{
	// NOTE: I know I can use strlen, but I wanted to do it this way. I'll use strlen in Tools

	mp_walker = _textLine.m_text;
	m_textLetterColumnPosition = _textLine.m_position.m_x;

	// If option should be highlighted, else not
	lineColor = (_highlightLine) ? Consts::WHITE_BACKGROUND : Consts::WHITE_FOREGROUND;

	while (*mp_walker != '\0')
	{
		mp_bufferCell = &mr_sharedCollisionRender.mp_frameBuffer[(_textLine.m_position.m_y * mr_sharedCollisionRender.SCREEN_BUFFER_CR.X) + m_textLetterColumnPosition++];
		mp_bufferCell->m_character = *mp_walker;
		mp_bufferCell->m_colorBFGround = lineColor;

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