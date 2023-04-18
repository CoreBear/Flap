#ifndef FILE_IO_MANAGER_H
#define FILE_IO_MANAGER_H

class SharedGame;

class FileIOManager
{
public:
	// Initialization
	inline FileIOManager(SharedGame& _sharedGame) : 
		mp_walker(nullptr),
		mr_sharedGame(_sharedGame) 
	{
		return;
	}

	// Functionality
	bool CheckForNewHighScore();
	void ClearHighScores();
	void LoadGame();
	void LoadHighScores();
	void SaveGame();
	void SaveHighScores(bool _save, const char* _initials);

private:
	// Member Variables
	const char* mp_walker;
	const char* const HIGH_SCORES_FILE_NAME = "HighScores.txt";
	int m_reusableIterator;
	SharedGame& mr_sharedGame;

	// Functionality
	int GetScore(int _scoreIndex);
	void UpdateHighScores(const char* _initials);
};

#endif FILE_IO_MANAGER_H