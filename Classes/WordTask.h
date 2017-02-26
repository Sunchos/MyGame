#pragma once
#include "cocos2d.h"
using namespace cocos2d;

//////////////////////////////////////////////////////////////////////////////////
// Interface of the WordTask class.
//

class WordTask
{
	//
	// Constructor and destructor.
	//
public:
	//! Constructor.
	WordTask();
	//! Destructor.
	~WordTask();

	//
	// Public methods.
	//
public:
	//! Init.
	int init(char*, char*, char);
	//! Check key.
	bool isKeyCorrect(const EventKeyboard::KeyCode& keyCode);
	//! Retrurns answer.
	char* getAnswer() { return answer_; }
	//! Returns key.
	char getKey() { return key_; }
	//! Returns task.
	char* getTask() { return task_; }
	//! Set answer.
	int setAnswer(char*);
	//! Set key.
	int setKey(char);
	//! Set task.
	int setTask(char*);

	//
	// Private members.
	//
private:
	//! Answer.
	char *answer_;
	//! Key.
	char key_;
	//! Task.
	char *task_;
};