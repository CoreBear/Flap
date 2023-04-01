#ifndef MANAGER_H
#define MANAGER_H

class Manager
{
public:
	// Initialization
	Manager(const Manager&) = delete;
	Manager& operator=(const Manager&) = delete;

	// Updates
	virtual void Update() { return; }

	// Destruction
	virtual ~Manager() = default;

protected:
	// Initialization
	Manager() = default;
};

#endif MANAGER_H