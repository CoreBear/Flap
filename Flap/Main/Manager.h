#ifndef MANAGER_H
#define MANAGER_H

class Manager
{
public:
	// Updates
	virtual void Update() { return; }
		    
	// Functionality
	static bool GameIsRunning() { return s_gameIsRunning; }

	// Destruction
	virtual ~Manager() { return; }

protected:
	// Static Variables
	static bool s_gameIsRunning;
};

#endif MANAGER_H