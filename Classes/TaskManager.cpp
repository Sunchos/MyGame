#include "TaskManager.h"
#include <fstream>
#include <string>

///////////////////////////////////////////////////////
// Implementation of the TaskManager class.
//

TaskManager::TaskManager()
{
	//listOfTasks_ = nullptr;
	numTasks_ = 3;
	random_ = 0;
}

TaskManager::~TaskManager()
{
	//delete[] listOfTasks_;
	//listOfTasks_ = nullptr;
}

void TaskManager::ClearIndexes()
{
	indexes_.clear();
}

void TaskManager::FindData(const std::string& line, const size_t index)
{
	std::string task;
	std::string ans;
	std::string key;

	size_t found = line.find(' ');
	size_t old = 0;

	if (found != std::string::npos)
		for (size_t i = 0; i != found; ++i)
			task.push_back(line[i]);
	old = found + 1;
	found = line.find(' ', old);

	if (found != std::string::npos)
		for (size_t i = old; i != found; ++i)
			ans.push_back(line[i]);

	old = found + 1;

	if (found != std::string::npos)
		for (size_t i = old; i != line.size(); ++i)
			key.push_back(line[i]);

	char *taskCh = new char[task.length() + 1];
	strcpy(taskCh, task.c_str());
	char *ansCh = new char[ans.length() + 1];
	strcpy(ansCh, ans.c_str());

	listOfTasks_.push_back(WordTask(taskCh, ansCh, key[0u]));
}

int TaskManager::getNumTasks()
{
	return numTasks_;
}

WordTask TaskManager::getRandTask()
{
	while(true)
	{
		random_ = random(0, numTasks_ - 1);
		if (std::find(indexes_.begin(), indexes_.end(), random_) == indexes_.end())
		{
			indexes_.push_back(random_);
			break;
		}
	}
	
	return getTask(random_);
}

WordTask TaskManager::getTask(const int count)
{
	return listOfTasks_.at(count);
}

void TaskManager::init(const int numTask)
{
	if (!LoadFile("words.txt"))
		LoadDefaultWords();

	SetNumTasks(listOfTasks_.size());
	srand(time(0));
}

void TaskManager::LoadDefaultWords()
{
	char *tasks[] = { "c_llege", "fi_h", "p_one" };
	char *ans[] = { "college", "fish", "phone" };
	char key[] = { 'o', 's', 'h' };

	for (int i = 0; i != numTasks_; ++i)
	{
		listOfTasks_.push_back(WordTask(tasks[i], ans[i], key[0u]));
	}
}

bool TaskManager::LoadFile(const std::string filePath)
{
	std::ifstream ifile(filePath);
	std::string line;
	size_t i = 0;
	if (!ifile.is_open())
		return false;

	while (std::getline(ifile, line))
	{
		FindData(line, i);
		++i;
	}
	ifile.close();
	
	return true;
}

void TaskManager::SetNumTasks(const int num)
{
	numTasks_ = num;
}