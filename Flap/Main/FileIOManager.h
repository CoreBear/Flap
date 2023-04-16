#ifndef FILE_IO_MANAGER_H
#define FILE_IO_MANAGER_H

class SharedGame;

class FileIOManager
{
public:
	// Initialization
	inline FileIOManager(SharedGame& _sharedGame) : 
		mpp_highScoreLines(nullptr),
		mp_walker(nullptr),
		mr_sharedGame(_sharedGame) 
	{
		return; 
	}

	// Functionality
	bool CheckForNewHighScore();
	void LoadGame();
	void SaveGame();
	void SaveHighScores(bool _save, const char* _initials);

private:
	// Static Variables
	static constexpr int MAX_STRING_LENGTH = 50;			// NOTE: Arbitrary value

	// Member Variables
	char** mpp_highScoreLines;
	const char* mp_walker;
	const char* const HIGH_SCORES_FILE_NAME = "HighScores.txt";
	int m_reusableIterator;
	SharedGame& mr_sharedGame;

	// Functionality
	int GetScore(int _scoreIndex);
	void UpdateHighScores(const char* _initials);
};

#endif FILE_IO_MANAGER_H