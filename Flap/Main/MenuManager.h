#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Consts.h"
#include "DList.h"
#include "Enums.h"
#include "InputReceiver.h"
#include "MenuBase.h"

#include <stack>

class BufferCell;
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
	inline void ToHighScore() { ReadyNextMenu(Enums::MenuName::HighScore); }
	inline void ToMain() { ReadyNextMenu(Enums::MenuName::Main); }
	inline void ToNewHighScore() { ReadyNextMenu(Enums::MenuName::NewHighScore); }
	inline void ToResultsMulti() { ReadyNextMenu(Enums::MenuName::ResultsMulti); }

	// Destructor
	~MenuManager();

protected:
	// Functionality
	void InputAccept(Enums::InputPressState _inputPressState) override;
	inline void InputCharacter(int _inputIndexOrKeyCode) override { mpp_menus[m_currentMenuIndex]->InputCharacter(_inputIndexOrKeyCode); }
	void InputDown(Enums::InputPressState _inputPressState) override;
	void InputLeft(Enums::InputPressState _inputPressState) override;
	void InputRight(Enums::InputPressState _inputPressState) override;
	void InputUp(Enums::InputPressState _inputPressState) override;

private:
	// Member Variables
	const bool m_menuCanBeReturnedTo[static_cast<int>(Enums::MenuName::NumberOfMenus)]{ false, false, true, true, true, true, false, false, true, false, true, false };
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
	void ClearReturnMenuStack();
	void ReadyNextMenu(int _menuNameIndex, bool _isReturning = false);
	void WriteMenuIntoFrameBuffer();
	void WriteTextLineIntoBuffer(bool _highlightLine, const TextLine& _textLine);
};
#endif MENU_MANAGER_H