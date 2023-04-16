#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Consts.h"
#include "DList.h"
#include "Enums.h"
#include "InputReceiver.h"

#include <stack>

class BufferCell;
class MenuBase;
class SharedGame;
class SharedInput;
class SharedRender;
class TextLine;
namespace Structure { struct Vector2; }

class MenuManager final : public InputReceiver
{
public:
	// Initialzation
	MenuManager(SharedGame& _sharedGame, SharedRender& _sharedRender);
	MenuManager(const MenuManager&) = delete;
	MenuManager& operator=(const MenuManager&) = delete;

	// Updates
	void FixedUpdate();
	void Update() { return; }

	// Functionality
	inline void PauseGame() { ReadyNextMenu(Enums::MenuName::Pause); }
	inline void ToMain() { ReadyNextMenu(Enums::MenuName::Main); }
	void ToResultsMulti();
	void ToResultsSingle(bool _newHighScore);

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
	DList<BufferCell*>::Iterator m_menuCellsIterator;
	MenuBase** mpp_menus;
	SharedGame& mr_sharedGame;
	SharedRender& mr_sharedRender;
	std::stack<int> m_returnMenuStack;
	unsigned short m_lineColor;

	// Functionality
	void ReadyNextMenu(int _menuNameIndex, bool _isReturning = false);
	void ResetAllMenus();
	void WriteMenuIntoFrameBuffer();
	void WriteTextLineIntoBuffer(bool _highlightLine, const TextLine& _textLine);
};
#endif MENU_MANAGER_H