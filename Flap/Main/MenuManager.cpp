#pragma region Includes
#include "MenuManager.h"

#include "BufferCell.h"
#include "Consts.h"
#include "Enums.h"
#include "ExitMenu.h"
#include "LocalMultiplayerMenu.h"
#include "MainMenu.h"
#include "MultiplayerMenu.h"
#include "NetworkMenu.h"
#include "OptionsMenu.h"
#include "PauseMenu.h"
#include "ResultsMenu.h"
#include "SharedCollisionRender.h"
#include "SharedInput.h"
#include "SinglePlayerMenu.h"
#include "Structure.h"
#include "MenuBase.h"
#include "WelcomeMenu.h"
#pragma endregion

#pragma region Initialization
MenuManager::MenuManager(SharedCollisionRender& _sharedCollisionRender) : m_currentMenuIndex(Consts::NO_VALUE), mr_sharedCollisionRender(_sharedCollisionRender)
{
	m_readIndex = Consts::NO_VALUE;

	mpp_menus = new MenuBase*[static_cast<int>(Enums::MenuName::NumberOfMenus)];

	// For each menu
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < static_cast<int>(Enums::MenuName::NumberOfMenus); m_reusableIterator++)
	{
		switch ((Enums::MenuName)m_reusableIterator)
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
void MenuManager::InputDown(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Click:
		break;
	case Enums::InputPressState::Dead:
		break;
	case Enums::InputPressState::Held:
		break;
	case Enums::InputPressState::PressedThisFrame:
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
void MenuManager::InputLeft(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Click:
		break;
	case Enums::InputPressState::Dead:
		break;
	case Enums::InputPressState::Held:
		break;
	case Enums::InputPressState::PressedThisFrame:
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
void MenuManager::InputRight(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Click:
		break;
	case Enums::InputPressState::Dead:
		break;
	case Enums::InputPressState::Held:
		break;
	case Enums::InputPressState::PressedThisFrame:
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
void MenuManager::InputUp(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Click:
		break;
	case Enums::InputPressState::Dead:
		break;
	case Enums::InputPressState::Held:
		break;
	case Enums::InputPressState::PressedThisFrame:
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
#pragma endregion

#pragma region Private Functionality
void MenuManager::DisplayMenu(Enums::MenuName _menuName)
{
	// Toggle off current menu

	// Toggle on next menu

	m_currentMenuIndex = static_cast<int>(_menuName);
}
void MenuManager::WriteMenuIntoBuffer()
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mpp_menus[m_currentMenuIndex]->m_numberOfTextLines; m_reusableIterator++)
	{
		WriteTextLineIntoBuffer(*mpp_menus[m_currentMenuIndex]->mp_textLines[m_reusableIterator]);
	}
}
void MenuManager::WriteTextLineIntoBuffer(const Structure::TextLine& _textLine)
{
	mp_walker = _textLine.m_text;
	m_textLetterColumnPosition = _textLine.m_position.m_x;

	while (*mp_walker != '\0')
	{
		mp_bufferCell = &mr_sharedCollisionRender.mp_frameBuffer[(_textLine.m_position.m_y * mr_sharedCollisionRender.SCREEN_BUFFER_CR.X) + m_textLetterColumnPosition++];
		mp_bufferCell->m_character = *mp_walker;
		mp_bufferCell->m_colorBFGround = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

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