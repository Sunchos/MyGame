#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "WordTask.h"
#include "TaskManager.h"

#define LABEL_FONTNAME "fonts/Marker Felt.ttf"
#define LABEL_FONTSIZE 24
#define SCORE_CHANGE 10
#define LIVE_COUNT 5

class HelloWorld : public cocos2d::Layer
{
public:
	//! Create end scene.
	void createEndScene();
	//! Create scene.
    static cocos2d::Scene* createScene();
	//! Create Sprite.
	cocos2d::Sprite* CreateSprite(const char *text, const cocos2d::Vec2& pos, const int orderZ);
	//! Text Label.
	cocos2d::Label* CreateTextLabel(const char *text, const char* ff, const int fsize, const cocos2d::Vec2& pos);
	//! Init method.
    virtual bool init();
    //! a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	//! Callback for replay button.
	void menuReplayCallback(cocos2d::Ref* pSender);
	//! Move objects on screen.
	void MoveObject(cocos2d::Sprite* obj, cocos2d::Vec2& moveTo, cocos2d::Vec2& scaleBy);
	//! Event Key pressed.
	void onKeyPressed(const cocos2d::EventKeyboard::KeyCode& keyCode, const cocos2d::Event* event);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	//
	// Private methods.
	//
private:
	//! Add Score.
	void addScore(const int s);
	//! Close application.
	void CloseApplication();
	//! Create background.
	void CreateBackbround();
	//! Create lives.
	void CreateLives();
	//! Init object move.
	void initPosMove();
	//! Load score.
	void LoadScore();
	//! Play sound once.
	void PlaySoundOnce(const std::string& path);
	//! Save score.
	void SaveScore();
	//! Set lives.
	void setLives(const int lives);
	//! Set max score.
	void SetMaxScore(const int s);
	//! Set socre.
	void setScore(const int s);
	//! Settings menu.
	void SettingsMenuCallBack();
	//! Show answer.
	void ShowAnswer();
	//! Show end scene.
	void showEnd(const float dt);
	//! Task label is out of screen.
	void TaskIsOut();
	//! Udpate task.
	void updateTask(const float dt);

	//
	// Private members.
	//
private:
	//! Scene.
	cocos2d::Scene *endScene_;
	//! Lives counter.
	int lives_;
	//! Lives in Sprites.
	cocos2d::Sprite* liveSprites_[LIVE_COUNT];
	//! Max score.
	int maxScore_;
	//! Max score label.
	cocos2d::Label *maxScoreLabel_;
	//! Score.
	int score_;
	//! Score label.
	cocos2d::Label *scoreLabel_;
	//! Task label.
	cocos2d::Label* taskLabel_;
	//! Task manager.
	TaskManager tm_;
	//! Word task.
	WordTask task_;
};

#endif // __HELLOWORLD_SCENE_H__
