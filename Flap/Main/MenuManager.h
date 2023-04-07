#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Consts.h"
#include "Enums.h"
#include "InputReceiver.h"

#include <mutex>
#include <stack>

class BufferCell;
class MenuBase;
class SharedCollisionRender;
class SharedGame;
class SharedInput;
namespace Structure { struct TextLine; }
namespace Structure { struct Vector2; }

class MenuManager final : public InputReceiver
{
public:
	// Initialzation
	MenuManager(SharedCollisionRender& _sharedCollisionRender, SharedGame& _sharedGame);
	MenuManager(const MenuManager&) = delete;
	MenuManager& operator=(const MenuManager&) = delete;

	// Updates
	void FixedUpdate();
	void Update() { return; }

	// Functionality
	void ExitToMain() { DisplayMenu(Enums::MenuName::Main); }
	void ExitToResults() { DisplayMenu(Enums::MenuName::Results); }
	inline void PauseGame() { DisplayMenu(Enums::MenuName::Pause); }

	// Destructor
	~MenuManager();

protected:
	// Functionality
	void InputAccept(Enums::InputPressState _inputPressState) override;
	void InputDown(Enums::InputPressState _inputPressState);
	void InputLeft(Enums::InputPressState _inputPressState);
	void InputRight(Enums::InputPressState _inputPressState);
	void InputUp(Enums::InputPressState _inputPressState);

private:
	// Member Variables
	const bool m_menuCanBeReturnedTo[static_cast<int>(Enums::MenuName::NumberOfMenus)] { false, true, true, true, true, false, true, false, true, false };
	BufferCell* mp_bufferCell;
	const char* mp_walker;
	int m_currentMenuIndex;
	int m_potentialNextMenuIndex;
	int m_reusableIterator;
	int m_textLetterColumnPosition;
	MenuBase** mpp_menus;
	SharedCollisionRender& mr_sharedCollisionRender;
	SharedGame& mr_sharedGame;
	short lineColor;
	std::stack<int> m_returnMenuStack;
	std::unique_lock<std::mutex> m_menuUniqueLock;

	// Functionality
	void DisplayMenu(int _menuNameIndex, bool _isReturning = false);
	void PostChangeSync();
	void PreChangeSync();
};
#endif MENU_MANAGER_H