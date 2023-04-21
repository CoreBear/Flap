#ifndef CLIENT_SEARCH_MENU_H
#define CLIENT_SEARCH_MENU_H

#include "NetworkSearchMenu.h"
#include "TextLine.h"

class SharedNetwork;

class ClientSearchMenu final : public NetworkSearchMenu
{
public:
	// Initialization
	ClientSearchMenu(SharedNetwork& _sharedNetwork) :
		NetworkSearchMenu(2, _sharedNetwork),			// Usually, this value must match the number of text lines below, but the last option will be revealed when at least one client connects
		m_lobbyWasEmptyLastFrame(true)
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Searching For Players", Consts::OFF_BY_ONE),	// Menu Title
			//new TextLine("...", 2),
			//new TextLine("Number Of Connected Users: 0/4", 10),
			new TextLine("Return to network menu", 15),
			new TextLine("Start Game", 20)
		};
	}
	ClientSearchMenu(const ClientSearchMenu&) = delete;
	ClientSearchMenu& operator=(const ClientSearchMenu&) = delete;

	//Updates
	void FixedUpdate() override
	{
		if (NetworkSearchMenu::UpdateDotsAndConnectedUsers())
		{
			// If lobby is empty
			mr_sharedNetwork.m_numOfConnClientsOnServMutext.lock();
			if (mr_sharedNetwork.m_numOfConnClientsOnServ == '0')
			{
				mr_sharedNetwork.m_numOfConnClientsOnServMutext.unlock();

				if (m_lobbyWasEmptyLastFrame == false)
				{
					m_lobbyWasEmptyLastFrame = true;

					// HACK: Hardcoded
					UpdateNumberOfTextLines(2);
				}
			}

			// If lobby is not empty
			else
			{
				mr_sharedNetwork.m_numOfConnClientsOnServMutext.unlock();

				if (m_lobbyWasEmptyLastFrame)
				{
					m_lobbyWasEmptyLastFrame = false;

					// HACK: Hardcoded
					UpdateNumberOfTextLines(3);
				}
			}
		}
	}

protected:
	// Functionality
	int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::StopHost;
		case 2:
			return Enums::MenuReturn::StopHost;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};

private:
	// Member Variables
	bool m_lobbyWasEmptyLastFrame;
};

#endif CLIENT_SEARCH_MENU_H