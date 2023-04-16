#pragma region Includes
#include "FileIOManager.h"

#include "Consts.h"
#include "SharedGame.h"

#include <fstream>
#define _CRT_SECURE_NO_WARNINGS 1
#pragma endregion

#pragma region Public Functionality
bool FileIOManager::CheckForNewHighScore()
{
	// Load high scores
	std::ifstream inStream(HIGH_SCORES_FILE_NAME);

	if (inStream.is_open())
	{
		mpp_highScoreLines = new char* [mr_sharedGame.MAX_NUMBER_OF_HIGH_SCORES];

		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedGame.MAX_NUMBER_OF_HIGH_SCORES; m_reusableIterator++)
		{
			mpp_highScoreLines[m_reusableIterator] = new char[MAX_STRING_LENGTH];
			inStream.getline(mpp_highScoreLines[m_reusableIterator], MAX_STRING_LENGTH);
		}

		inStream.close();
	}

	const int LAST_SCORE_INDEX = mr_sharedGame.MAX_NUMBER_OF_HIGH_SCORES - Consts::OFF_BY_ONE;
	
	// If new score beats the last score, it belongs on the list
	return (mr_sharedGame.m_largestSnakeLengthUponDeath > GetScore(LAST_SCORE_INDEX)) ? true : false;
}
void FileIOManager::LoadGame()
{

}
void FileIOManager::SaveGame()
{

}
void FileIOManager::SaveHighScores(bool _save, const char* _initials)
{
	if (_save)
	{
		UpdateHighScores(_initials);

		std::ofstream outStream(HIGH_SCORES_FILE_NAME, std::ios::trunc);

		if (outStream.is_open())
		{
			for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedGame.MAX_NUMBER_OF_HIGH_SCORES - Consts::OFF_BY_ONE; m_reusableIterator++)
			{
				outStream.write(mpp_highScoreLines[m_reusableIterator], strlen(mpp_highScoreLines[m_reusableIterator]));
				outStream.write("\n", Consts::OFF_BY_ONE);
			}

			outStream.write(mpp_highScoreLines[m_reusableIterator], strlen(mpp_highScoreLines[m_reusableIterator]));

			outStream.close();
		}
	}

	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedGame.MAX_NUMBER_OF_HIGH_SCORES; m_reusableIterator++)
	{
		delete[] mpp_highScoreLines[m_reusableIterator];
	}

	delete[] mpp_highScoreLines;
}
#pragma endregion

#pragma region Private Functionality
int FileIOManager::GetScore(int _scoreIndex)
{
	mp_walker = mpp_highScoreLines[_scoreIndex];

	constexpr char DELIM = '.';
	while (*mp_walker != DELIM)
	{
		++mp_walker;
	}

	++mp_walker;

	return Tools::StringToInt(mp_walker);
}
void FileIOManager::UpdateHighScores(const char* _initials)
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedGame.MAX_NUMBER_OF_HIGH_SCORES; m_reusableIterator++)
	{
		if (mr_sharedGame.m_largestSnakeLengthUponDeath > GetScore(m_reusableIterator))
		{
			delete[] mpp_highScoreLines[mr_sharedGame.MAX_NUMBER_OF_HIGH_SCORES - Consts::OFF_BY_ONE];

			for (int i = mr_sharedGame.MAX_NUMBER_OF_HIGH_SCORES - Consts::OFF_BY_ONE; i > m_reusableIterator; i--)
			{
				mpp_highScoreLines[i] = mpp_highScoreLines[i - Consts::OFF_BY_ONE];
			}

			char* newHighScoreString = new char[MAX_STRING_LENGTH];
			strcpy(newHighScoreString, _initials);
			strcat(newHighScoreString, ".");

			const char* intString = Tools::IntToString(mr_sharedGame.m_largestSnakeLengthUponDeath);
			strcat(newHighScoreString, intString);
			delete[] intString;

			mpp_highScoreLines[m_reusableIterator] = newHighScoreString;

			break;
		}
	}
}
#pragma endregion