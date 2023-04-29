#pragma region Includes
#include "SharedGame.h"

#include "BufferCell.h"
#include "MenuBase.h"
#include "Queue.h"
#include "SceneObject.h"
#include "Snake.h"
#include "Tools.h"
#pragma endregion

#pragma region Initialization
SharedGame::SharedGame(const Structure::Vector2<short>& _frameBufferWidthHeight) :
	m_bufferSize(_frameBufferWidthHeight.m_x * _frameBufferWidthHeight.m_y),
	mp_frameBuffer(new BufferCell[m_bufferSize]),
	mpp_highScoreLines(nullptr),
	m_isInGameSession(false),
	m_gameActivityIndex(Enums::GameActivity::Menu),
	MAX_NUMBER_OF_NODES_TO_ADD(9),				// NOTE: Arbitrary value
	m_numberOfSnakesInGame(Consts::NO_VALUE),
	m_numberOfFramesBeforeGameStart(60),		// NOTE: Arbitrary value
	m_numberOfFramesBetweenSpawn(60),			// NOTE: Arbitrary value
	mp_availableSpawnPositions(nullptr),
	mp_arrayOfColumnIndices(nullptr),
	FRAME_BUFFER_HEIGHT_WIDTH(_frameBufferWidthHeight)
{
	MenuBase::AssignSharedGame(*this);
	SceneObject::AssignSharedGame(*this);
	Snake::AssignSharedGame(*this);
	Tools::AssignSharedGame(*this);

	ResetFrameBuffer();

	for (int positionIndex = Consts::NO_VALUE; positionIndex < Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME; positionIndex++)
	{
		mp_snakeStartPositions[positionIndex] = new Structure::Vector2<int>[static_cast<int>(positionIndex + Consts::OFF_BY_ONE)];
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

	m_gameAreaWidthHeight.m_x = FRAME_BUFFER_HEIGHT_WIDTH.m_x;
	m_gameAreaWidthHeight.m_y = FRAME_BUFFER_HEIGHT_WIDTH.m_y;

	m_gameAreaTopLeftPosition.m_x = 0;
	m_gameAreaTopLeftPosition.m_y = 0;
}
#pragma endregion

#pragma region Public Functionality
void SharedGame::AddAvailableSpawnIndex(int _x, int _y)
{
	mp_availableSpawnPositions[_x].PushBack(_y);
}
void SharedGame::GameSession(bool _isInGameSession, bool _isSinglePlayerGame)
{
	m_isInGameSession = _isInGameSession;
	m_isSinglePlayerGame = _isSinglePlayerGame;
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
	for (m_reusableIterator_3 = Consts::NO_VALUE; m_reusableIterator_3 < m_bufferSize; m_reusableIterator_3++)
	{
		mp_frameBuffer[m_reusableIterator_3].ResetCell();
	}
}
void SharedGame::ResetFrameBufferSynced()
{
	m_frameBufferMutex.lock();

	ResetFrameBuffer();

	m_frameBufferMutex.unlock();
}
#pragma endregion

#pragma region Destruction
SharedGame::~SharedGame()
{
	delete[] mp_frameBuffer;

	if (mpp_highScoreLines != nullptr)
	{
		for (int lineIndex = Consts::NO_VALUE; lineIndex < MAX_NUMBER_OF_HIGH_SCORES; lineIndex++)
		{
			if (mpp_highScoreLines[lineIndex] != nullptr)
			{
				delete[] mpp_highScoreLines[lineIndex];
				mpp_highScoreLines[lineIndex] = nullptr;
			}
		}
		delete[] mpp_highScoreLines;
		mpp_highScoreLines = nullptr;
	}

	delete[] mp_arrayOfColumnIndices;
	delete[] mp_availableSpawnPositions;

	for (int positionIndex = Consts::NO_VALUE; positionIndex < Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME; positionIndex++)
	{
		delete mp_snakeStartPositions[positionIndex];
	}
}
#pragma endregion