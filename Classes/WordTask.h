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
	//! Constructor.
	WordTask(char*, char*, char, int falAns = 0, int righAns = 0);
	//! Destructor.
	~WordTask();

	//
	// Public methods.
	//
public:
	//! Init.
	int init(char*, char*, char, int falAns = 0, int righAns = 0);
	//! Check key.
	bool isKeyCorrect(const EventKeyboard::KeyCode& keyCode);
	//! Retrurns answer.
	char* getAnswer() { return answer_; }
	//! Returns count false answer.
	int getFalseAns() const { return falseAns_; }
	//! Returns key.
	char getKey() { return key_; }
	//! Returns count right answer.
	int getRightAns() const { return rightAns_; }
	//! Returns task.
	char* getTask() { return task_; }
	//! Set answer.
	int setAnswer(char*);
	//! Set false answer.
	void SetFalseAns(const int);
	//! Set key.
	int setKey(char);
	//! Set right answer.
	void SetRightAns(const int);
	//! Set task.
	int setTask(char*);

	//
	// Private members.
	//
private:
	//! Answer.
	char *answer_;
	//! Count false answer.
	int falseAns_;
	//! Key.
	char key_;
	//! Count right answer.
	int rightAns_;
	//! Task.
	char *task_;
};