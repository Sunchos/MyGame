#include "WordTask.h"

/////////////////////////////////////////////////////////////////////
// Implementation of the WordTask class.
//

WordTask::WordTask() 
	: answer_(nullptr), key_(' '), task_(nullptr) , falseAns_(0), rightAns_(0)
{

}

WordTask::WordTask(char* t, char* ans, char key, int falAns, int righAns)
{
	this->init(t, ans, key, falAns, righAns);
}

WordTask::~WordTask()
{

}

int WordTask::init(char* t, char* ans, char key, int falAns, int righAns)
{
	this->key_ = key;
	if (ans != nullptr && t != nullptr)
	{
		this->answer_ = ans;
		this->task_ = t;
	}

	falseAns_ = falAns;
	rightAns_ = righAns;

	return 0;
}

bool WordTask::isKeyCorrect(const EventKeyboard::KeyCode& keyCode)
{
	int offset = static_cast<char>(EventKeyboard::KeyCode::KEY_A) - 'a';
	if (static_cast<EventKeyboard::KeyCode>(this->key_ + offset) == keyCode)
		return true;
	return false;
}

int WordTask::setAnswer(char* ans)
{
	this->answer_ = ans;
	return 0;
}

void WordTask::SetFalseAns(const int falseAns)
{
	this->falseAns_ = falseAns;
}

int WordTask::setKey(const char key)
{
	this->key_ = key;
	return 0;
}

void WordTask::SetRightAns(const int rightAns)
{
	this->rightAns_ = rightAns;
}

int WordTask::setTask(char* task)
{
	this->task_ = task;
	return 0;
}