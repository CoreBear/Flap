#ifndef NETWORK_MENU_H
#define NETWORK_MENU_H

#include "Consts.h"
#include "DynamicTextMenu.h"
#include "SharedNetwork.h"
#include "Structure.h"
#include "TextLine.h"

class NetworkMenu final : public DynamicTextMenu
{
public:
	// Initialization
	void Initialize() override
	{
		ClearCells();

		mp_newString = new char[sp_sharedGame->MAX_HS_STRING_LENGTH];
		strcpy(mp_newString, "IP Address: ");
		strcat(mp_newString, mr_sharedNetwork.mp_myIPAddress);

		mp_walker = mp_newString;

		m_columnPositionOffset = Tools::CenterText_ReturnStartColumn(mp_newString);

		while (*mp_walker != '\0')
		{
			constexpr int ROW = 2;

			mp_newBufferCell = new BufferCell;
			mp_newBufferCell->m_character = *mp_walker;
			mp_newBufferCell->m_colorBFGround = Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)];
			mp_newBufferCell->m_position.m_x = m_columnPositionOffset++;
			mp_newBufferCell->m_position.m_y = ROW;
			m_cells.PushBack(mp_newBufferCell);

			++mp_walker;
		}

		delete[] mp_newString;
	}
	NetworkMenu(SharedNetwork& _sharedNetwork) :
		DynamicTextMenu(4),	// This value must match the number of text lines below
		mr_sharedNetwork(_sharedNetwork)
	{
		mp_textLines = new TextLine * [m_totNumOfTextLines]
		{
			new TextLine("Network", Consts::OFF_BY_ONE),  // Menu Title
			//new TextLine("IP Address: ", 2),
			new TextLine("Host Game", 5),
			new TextLine("Look for Game", 10),
			new TextLine("Return", 15)
		};
	}

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::RunAsServer;
		case 2:
			return Enums::MenuReturn::RunAsClient;
		case 3:
			return Enums::MenuReturn::StopNetworking;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	}

private:
	// Member Variables
	SharedNetwork& mr_sharedNetwork;
};

#endif NETWORK_MENU_H