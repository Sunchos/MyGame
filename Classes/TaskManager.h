#pragma once
#include "WordTask.h"

//////////////////////////////////////////////////
// Interface of the TaskManager class.
//

class TaskManager
{
	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	TaskManager();
	//! Destructor.
	~TaskManager();

	//
	// Public methods.
	//
public:
	//! Returns num tasks.
	int getNumTasks();
	//! Returns task.
	WordTask getTask(const int);
	//! Returns random task.
	WordTask getRandTask();
	//! Init.
	void init(const int = 3);

	//
	// Private methods.
	//
private:
	//! Find Data from string.
	void FindData(const std::string& line, const size_t index);
	//! Load Default words.
	void LoadDefaultWords();
	//! Load from file.
	bool LoadFile(const std::string filePaht);
	//! Set num tasks.
	void SetNumTasks(const int);
	//! Save.
	//void Save();
	
	//
	// Private members.
	//
private:
	//! List tasks.
	std::vector<WordTask> listOfTasks_;
	//! Number tasks.
	int numTasks_;
	//! Random num.
	int random_;
};