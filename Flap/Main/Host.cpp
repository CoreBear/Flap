#pragma region Includes
#include "Host.h"
#pragma endregion

#pragma region Destruction
Host::~Host()
{
	shutdown(m_commSocket, SD_BOTH);
	closesocket(m_commSocket);
}
#pragma endregion