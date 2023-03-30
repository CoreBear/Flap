#ifndef MANAGER_H
#define MANAGER_H

class Manager
{
public:
	// Updates
	virtual void Update() { return; }

	// Destruction
	virtual ~Manager() { return; }
};

#endif MANAGER_H