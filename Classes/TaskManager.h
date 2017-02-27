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
	//! Clear indexes.
	void ClearIndexes();
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
	bool LoadFile(const std::string filePath);
	//! Set num tasks.
	void SetNumTasks(const int);
	//! Save fo file.
	void SaveFile(const std::string& filePath);
	
	//
	// Private members.
	//
private:
	//! Indexes.
	std::vector<int> indexes_;
	//! List tasks.
	std::vector<WordTask> listOfTasks_;
	//! Number tasks.
	int numTasks_;
	//! Random num.
	int random_;
};