#pragma region Includes
#include "SharedGame.h"

#include "BufferCell.h"
#include "Consts.h"
#include "MenuBase.h"
#include "Queue.h"
#include "SceneObject.h"
#include "Snake.h"
#include "Tools.h"
#pragma endregion

#pragma region Initialization
SharedGame::SharedGame(const Structure::Vector2<short>& _frameBufferWidthHeight) :
	mpp_frameBuffer(new BufferCell*[_frameBufferWidthHeight.m_y]),
	mpp_highScoreLines(nullptr),
	m_isInGameSession(false),
	m_gameActivityIndex(Enums::GameActivity::Menu),
	MAX_NUMBER_OF_NODES_TO_ADD(9),					// NOTE: Arbitrary value
	m_numberOfSnakesInGame(Consts::NO_VALUE),
	m_numberOfFramesBeforeGameStart(60),			// NOTE: Arbitrary value
	m_numberOfFramesBetweenSpawn(60),				// NOTE: Arbitrary value
	mp_availableSpawnPositions(nullptr),
	mp_arrayOfColumnIndices(nullptr),
	FRAME_BUFFER_HEIGHT_WIDTH(_frameBufferWidthHeight)
{
	MenuBase::AssignSharedGame(*this);
	SceneObject::AssignSharedGame(*this);
	Snake::AssignSharedGame(*this);
	Tools::AssignSharedGame(*this);

	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < _frameBufferWidthHeight.m_y; m_reusableIterator_1++)
	{
		mpp_frameBuffer[m_reusableIterator_1] = new BufferCell[_frameBufferWidthHeight.m_x];
	}

	ResetFrameBuffer();

	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME; m_reusableIterator_1++)
	{
		mp_snakeStartPositions[m_reusableIterator_1] = new Structure::Vector2<int>[static_cast<int>(m_reusableIterator_1 + Consts::OFF_BY_ONE)];
	}

	// HACK: Do this dynamically and use calculated positions
	{
		// One player
		{
			mp_snakeStartPositions[0][0].m_x = 10;
			mp_snakeStartPositions[0][0].m_y = 20;
		}

		// Two players
		{
			// Player 1
			mp_snakeStartPositions[1][0].m_x = 10;
			mp_snakeStartPositions[1][0].m_y = 20;

			// Player 2
			mp_snakeStartPositions[1][1].m_x = 20;
			mp_snakeStartPositions[1][1].m_y = 20;
		}

		// Three players
		{
			// Player 1
			mp_snakeStartPositions[2][0].m_x = 10;
			mp_snakeStartPositions[2][0].m_y = 20;

			// Player 2			   
			mp_snakeStartPositions[2][1].m_x = 20;
			mp_snakeStartPositions[2][1].m_y = 20;

			// Player 3			   
			mp_snakeStartPositions[2][2].m_x = 30;
			mp_snakeStartPositions[2][2].m_y = 20;
		}

		// Four players
		{
			// Player 1
			mp_snakeStartPositions[3][0].m_x = 10;
			mp_snakeStartPositions[3][0].m_y = 20;

			// Player 2
			mp_snakeStartPositions[3][1].m_x = 20;
			mp_snakeStartPositions[3][1].m_y = 20;

			// Player 3
			mp_snakeStartPositions[3][2].m_x = 30;
			mp_snakeStartPositions[3][2].m_y = 20;

			// Player 4
			mp_snakeStartPositions[3][3].m_x = 40;
			mp_snakeStartPositions[3][3].m_y = 20;
		}
	}

	UpdateMyGameAreaBounds(FRAME_BUFFER_HEIGHT_WIDTH.m_x, FRAME_BUFFER_HEIGHT_WIDTH.m_y);
}
#pragma endregion

#pragma region Public Functionality
void SharedGame::AddAvailableSpawnIndex(int _x, int _y)
{
	mp_availableSpawnPositions[_x].PushBack(_y);
}
void SharedGame::EndGameSession() 
{
	m_borderIsRequired = false;
	m_isInGameSession = false;
	m_isSinglePlayerGame = false;
}
const Structure::Vector2<int>& SharedGame::GetRandomSpawnPositionRef()
{
	// Select a row with available columns
	do
	{
		m_randomPosition.m_y = m_random() % FRAME_BUFFER_HEIGHT_WIDTH.m_y;
	} while (mp_availableSpawnPositions[m_randomPosition.m_y].IsEmpty());

	// Store, remove, and return first randomized column index
	m_randomPosition.m_x = mp_availableSpawnPositions[m_randomPosition.m_y].Peek();
	mp_availableSpawnPositions[m_randomPosition.m_y].PopFront();
	return m_randomPosition;
}
const Structure::Vector2<int>& SharedGame::GetSnakeStartPositionRef(int _numberOfPlayersIndex, int _playerIndex)
{ 
	RemoveAvailableSpawnIndex(mp_snakeStartPositions[_numberOfPlayersIndex][_playerIndex].m_x, mp_snakeStartPositions[_numberOfPlayersIndex][_playerIndex].m_y);

	return mp_snakeStartPositions[_numberOfPlayersIndex][_playerIndex]; 
}
void SharedGame::RemoveAvailableSpawnIndex(int _x, int _y)
{
	mp_availableSpawnPositions[_x].Remove(_y);
}
void SharedGame::ResetAvailableSpawnIndices()
{
	delete[] mp_arrayOfColumnIndices;
	delete[] mp_availableSpawnPositions;

	mp_arrayOfColumnIndices = new int[FRAME_BUFFER_HEIGHT_WIDTH.m_x];
	mp_availableSpawnPositions = new Queue<int>[FRAME_BUFFER_HEIGHT_WIDTH.m_x];

	// For each row
	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < FRAME_BUFFER_HEIGHT_WIDTH.m_y; m_reusableIterator_1++)
	{
		// Reset column indices
		for (m_reusableIterator_2 = Consts::NO_VALUE; m_reusableIterator_2 < FRAME_BUFFER_HEIGHT_WIDTH.m_x; m_reusableIterator_2++)
		{
			mp_arrayOfColumnIndices[m_reusableIterator_2] = m_reusableIterator_2;
		}

		// Clear current column indices
		mp_availableSpawnPositions[m_reusableIterator_1].Clear();

		// Randomize column indices
		for (m_reusableIterator_2 = Consts::NO_VALUE; m_reusableIterator_2 < FRAME_BUFFER_HEIGHT_WIDTH.m_x; m_reusableIterator_2++)
		{
			m_randomColumn = m_random() % FRAME_BUFFER_HEIGHT_WIDTH.m_x;

			mp_arrayOfColumnIndices[m_reusableIterator_2] += mp_arrayOfColumnIndices[m_randomColumn];
			mp_arrayOfColumnIndices[m_randomColumn] = mp_arrayOfColumnIndices[m_reusableIterator_2] - mp_arrayOfColumnIndices[m_randomColumn];
			mp_arrayOfColumnIndices[m_reusableIterator_2] -= mp_arrayOfColumnIndices[m_randomColumn];
		}

		// Add randomized indices into row list
		for (m_reusableIterator_2 = Consts::NO_VALUE; m_reusableIterator_2 < FRAME_BUFFER_HEIGHT_WIDTH.m_x; m_reusableIterator_2++)
		{
			mp_availableSpawnPositions[m_reusableIterator_1].PushBack(mp_arrayOfColumnIndices[m_reusableIterator_2]);
		}
	}
}
void SharedGame::ResetFrameBuffer()
{
	// Using 3 and 4, becuase ResetAvailableSpawnIndices are using 1 and 2
	for (m_reusableIterator_3 = Consts::NO_VALUE; m_reusableIterator_3 < FRAME_BUFFER_HEIGHT_WIDTH.m_y; m_reusableIterator_3++)
	{
		for (m_reusableIterator_4 = Consts::NO_VALUE; m_reusableIterator_4 < FRAME_BUFFER_HEIGHT_WIDTH.m_x; m_reusableIterator_4++)
		{
			mpp_frameBuffer[m_reusableIterator_3][m_reusableIterator_4].ResetCell();
		}
	}
}
void SharedGame::ResetFrameBufferSynced()
{
	m_frameBufferMutex.lock();

	ResetFrameBuffer();

	m_frameBufferMutex.unlock();
}
void SharedGame::StartGameSession(bool _isInGameSession, bool _isSinglePlayerGame)
{
	m_isInGameSession = _isInGameSession;
	m_isSinglePlayerGame = _isSinglePlayerGame;

	TryToggleBorder(true);
}
void SharedGame::TryToggleBorder(bool _isVisible)
{
	if (_isVisible)
	{
		if (m_gameAreaBounds.m_x != FRAME_BUFFER_HEIGHT_WIDTH.m_x || m_gameAreaBounds.m_y != FRAME_BUFFER_HEIGHT_WIDTH.m_y)
		{
			m_borderIsRequired = true;
			return;
		}
	}

	m_borderIsRequired = false;
}
void SharedGame::UpdateClientSharedGameAreaOffsets(const Structure::Vector2<short>& _clientOffsets)
{
	if (_clientOffsets.m_x < m_clientSharedGameAreaOffsets.m_x)
	{
		m_clientSharedGameAreaOffsets.m_x = _clientOffsets.m_x;
	}
	if (_clientOffsets.m_y < m_clientSharedGameAreaOffsets.m_y)
	{
		m_clientSharedGameAreaOffsets.m_y = _clientOffsets.m_y;
	}
}
void SharedGame::UpdateMyGameAreaBounds(short _xBound, short _yBound)
{
	m_gameAreaBounds.m_x = _xBound;
	m_gameAreaBounds.m_y = _yBound;
}
#pragma endregion

#pragma region Destruction
SharedGame::~SharedGame()
{
	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < FRAME_BUFFER_HEIGHT_WIDTH.m_y; m_reusableIterator_1++)
	{
		delete[] mpp_frameBuffer[m_reusableIterator_1];
	}

	delete[] mpp_frameBuffer;

	if (mpp_highScoreLines != nullptr)
	{
		for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < MAX_NUMBER_OF_HIGH_SCORES; m_reusableIterator_1++)
		{
			if (mpp_highScoreLines[m_reusableIterator_1] != nullptr)
			{
				delete[] mpp_highScoreLines[m_reusableIterator_1];
				mpp_highScoreLines[m_reusableIterator_1] = nullptr;
			}
		}
		delete[] mpp_highScoreLines;
		mpp_highScoreLines = nullptr;
	}

	delete[] mp_arrayOfColumnIndices;
	delete[] mp_availableSpawnPositions;

	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME; m_reusableIterator_1++)
	{
		delete mp_snakeStartPositions[m_reusableIterator_1];
	}
}
#pragma endregion