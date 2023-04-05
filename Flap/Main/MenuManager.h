#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Consts.h"
#include "InputReceiver.h"

class BufferCell;
class MenuBase;
class SharedCollisionRender;
class SharedInput;
namespace Structure { struct TextLine; }
namespace Structure { struct Vector2; }

class MenuManager final : public InputReceiver
{
public:
	// Initialzation
	MenuManager(SharedCollisionRender& _sharedCollisionRender);
	MenuManager(const MenuManager&) = delete;
	MenuManager& operator=(const MenuManager&) = delete;

	// Updates
	void FixedUpdate();
	void Update() { return; }

	// Destructor
	~MenuManager();

protected:
	// Functionality
	void InputDown(Enums::InputPressState _inputPressState) override;
	void InputLeft(Enums::InputPressState _inputPressState) override;
	void InputRight(Enums::InputPressState _inputPressState) override;
	void InputUp(Enums::InputPressState _inputPressState) override;

private:
	// Member Variables
	BufferCell* mp_bufferCell;
	const char* mp_walker;
	int m_currentMenuIndex;
	int m_reusableIterator;
	int m_textLetterColumnPosition;
	MenuBase** mpp_menus;
	SharedCollisionRender& mr_sharedCollisionRender;

	// Functionality
	void DisplayMenu(Enums::MenuName _menuName);
	void WriteMenuIntoBuffer();
	void WriteTextLineIntoBuffer(const Structure::TextLine& _textLine);
};
#endif MENU_MANAGER_H