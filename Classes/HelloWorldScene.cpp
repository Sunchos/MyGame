#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

void HelloWorld::addScore(const int s)
{
	setScore(score_ + s);
}

void HelloWorld::CloseApplication()
{
	this->SaveScore();
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::CreateBackbround()
{
	// Screen real dimensions
	auto vSize = Director::getInstance()->getVisibleSize();
	auto vWidth = vSize.width;
	auto vHeight = vSize.height;

	// Original image
	auto backOrig = Sprite::create("background.jpg");
	auto oWidth = backOrig->getContentSize().width;
	auto oHeight = backOrig->getContentSize().height;
	backOrig->setFlippedY(true);
	backOrig->setScale(vWidth / oWidth, vHeight / oHeight); // backOrig scaled to screen size
	backOrig->setPosition(vWidth / 2, vHeight / 2);

	// Create new texture with background in the exact size of the screen
	auto renderTexture = RenderTexture::create(vWidth, vHeight, Texture2D::PixelFormat::RGBA8888);
	renderTexture->begin();
	backOrig->visit();
	renderTexture->end();

	// Create new Sprite without scale, which perfoms much better
	auto backOk = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
	backOk->setPosition(vWidth / 2, vHeight / 2);
	addChild(backOk);
}

void HelloWorld::createEndScene()
{
	endScene_ = Scene::create();

	Label* lbEnd = Label::createWithTTF("End", LABEL_FONTNAME, 3 * LABEL_FONTSIZE);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 pos = Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2 + 60);
	lbEnd->setPosition(pos);
	endScene_->addChild(lbEnd);

	auto replayItem = MenuItemImage::create("button_black_repeat.png",
		"button_grey_repeat.png",
		CC_CALLBACK_1(HelloWorld::menuReplayCallback, this));
	replayItem->setPosition(Vec2(pos.x, pos.y - lbEnd->getContentSize().height - 10));

	auto menu = Menu::create(replayItem, NULL);
	menu->setPosition(Vec2::ZERO);
	endScene_->addChild(menu, 1);
}

void HelloWorld::CreateLives()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	lives_ = LIVE_COUNT;
	int blockSize = origin.x + visibleSize.width - LABEL_FONTSIZE * LIVE_COUNT;
	for (size_t i = 0; i != LIVE_COUNT; ++i)
	{
		Vec2 sprite_pos = Vec2(blockSize, origin.y + visibleSize.height - LABEL_FONTSIZE);
		liveSprites_[i] = CreateSprite("fullHeart.png", sprite_pos, 1);
		blockSize += LABEL_FONTSIZE;
	}
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

Sprite* HelloWorld::CreateSprite(const char *text, const Vec2& pos, const int orderZ)
{
	Sprite* sprite = Sprite::create(text);
	sprite->setPosition(pos);
	this->addChild(sprite, orderZ);
	return sprite;
}

Label* HelloWorld::CreateTextLabel(const char *text, const char* ff, const int fsize, const Vec2& pos)
{
	Label* label = Label::createWithTTF(text, ff, fsize);
	label->setPosition(pos);
	label->enableShadow();
	label->setColor(Color3B(219, 234, 255));
	this->addChild(label, 1, "text_label");
	return label;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

	//auto settingsItem = MenuItemImage::create(
	//	"SettingsCloseNormal.png",
	//	"SettingsCloseSelected.png",
	//	CC_CALLBACK_1(HelloWorld::SettingsMenuCallBack, this)
	//);

	//settingsItem->setPosition(Vec2(origin.x + settingsItem->getContentSize().width / 2,
	//	origin.y + settingsItem->getContentSize().height / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	CreateBackbround();

	Vec2 task_pos = Vec2(origin.x + visibleSize.width / 2, 
		origin.y + visibleSize.height / 4);

	taskLabel_ = this->CreateTextLabel("hi", LABEL_FONTNAME, LABEL_FONTSIZE, task_pos);
	tm_.init();
	this->updateTask(0);
	//task_ = tm_.getRandTask();
	//task_.init("hi", "bye", 'o');

	taskLabel_->setString(task_.getTask());

	//Vec2 taskMoveTo = origin + Vec2(visibleSize.width - LABEL_FONTSIZE + taskLabel_->getWidth(), LABEL_FONTSIZE);
	//MoveObject(reinterpret_cast<Sprite*>(taskLabel_), taskMoveTo, Vec2(1.5, 1.5));

	Vec2 score_pos = Vec2(origin.x + LABEL_FONTSIZE * 2 + 11, origin.y + visibleSize.height - LABEL_FONTSIZE);
	scoreLabel_ = this->CreateTextLabel("0", LABEL_FONTNAME, LABEL_FONTSIZE, score_pos);
	setScore(0);

	Vec2 maxScorePos = Vec2(origin.x + LABEL_FONTSIZE * 2 + 11, origin.y + visibleSize.height - LABEL_FONTSIZE * 2);
	maxScoreLabel_ = this->CreateTextLabel("0", LABEL_FONTNAME, LABEL_FONTSIZE, maxScorePos);
	//SetMaxScore(0);
	LoadScore();

	//Vec2 lives_pos = Vec2(origin.x + visibleSize.width - 60, origin.y + visibleSize.height - 20);
	//this->CreateTextLabel("Lives: 2/5", LABEL_FONTNAME, LABEL_FONTSIZE, lives_pos);

	CreateLives();

    return true;
}

void HelloWorld::initPosMove()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 taskPos = Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - taskLabel_->getHeight() + 11);

	taskLabel_->setPosition(taskPos);
	taskLabel_->setScale(1);
	Vec2 taskMoveTo = origin + Vec2(visibleSize.width / 2,
		-visibleSize.height / 4);
	MoveObject(reinterpret_cast<Sprite*>(taskLabel_), taskMoveTo, Vec2(3, 3));
}

void HelloWorld::LoadScore()
{
	auto maxScore = UserDefault::getInstance()->getStringForKey("MaxScore");
	maxScoreLabel_->setString(maxScore);
	//SetMaxScore();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	this->CloseApplication();
}

void HelloWorld::menuReplayCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->popScene();
	setScore(0);
	updateTask(0);
	CreateLives();
}

void HelloWorld::MoveObject(cocos2d::Sprite* obj, cocos2d::Vec2& moveTo, cocos2d::Vec2& scaleBy)
{
	auto seq = Sequence::create(MoveTo::create(8, moveTo), CallFuncN::create(CC_CALLBACK_0(HelloWorld::TaskIsOut, this)), NULL);
	//auto acMoveTo = MoveTo::create(8, moveTo);
	auto acScaleBy = ScaleBy::create(2, scaleBy.x, scaleBy.y);

	obj->runAction(acScaleBy);
	obj->runAction(seq);

	//auto acMoveTo = MoveTo::create(2, Vec2(240, 180));
	//auto moveBy1 = MoveBy::create(2, Vec2(-120, 0));
	//auto moveTofin = MoveTo::create(10, moveTo);
	//auto delay = DelayTime::create(0);
	//
	//auto acScaleBy = ScaleBy::create(3, scaleBy.x, scaleBy.y);
	//obj->runAction(acScaleBy);

	//auto rotateTo = RotateTo::create(2.0f, 40.0f);
	//obj->runAction(rotateTo);

	//auto rotateBy = RotateBy::create(2.0f, 45.0f);
	//obj->runAction(RepeatForever::create(rotateBy));

	//MoveEaseBounceIn(obj);
	//obj->runAction(Sequence::create(acMoveTo, delay, moveBy1, delay->clone(), moveTofin, nullptr));
	//obj->runAction(RepeatForever::create(Sequence::create(acMoveTo, delay, moveBy1, delay->clone(), moveTofin, nullptr)));
}

void HelloWorld::PlaySoundOnce(const std::string& path)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect(path.c_str(), false, 1.0f, 1.0f, 1.0f);
}

void HelloWorld::onKeyPressed(const cocos2d::EventKeyboard::KeyCode& keyCode, const cocos2d::Event* event)
{
	if (task_.isKeyCorrect(keyCode))
	{
		this->PlaySoundOnce("got-word.mp3");
		this->ShowAnswer();
		addScore(SCORE_CHANGE);
		scheduleOnce(schedule_selector(HelloWorld::updateTask), 1);
	} 
	else if (EventKeyboard::KeyCode::KEY_ESCAPE == keyCode)
			this->CloseApplication();
	else
	{
		ShowAnswer();
		TaskIsOut();
		//scheduleOnce(schedule_selector(HelloWorld::showEnd), 1);
	}

	//switch (keyCode)
	//{
	//case EventKeyboard::KeyCode::KEY_O:
	//{
	//	taskLabel_->setOpacity(0);
	//	break;
	//}
	//case EventKeyboard::KeyCode::KEY_A:
	//{
	//	auto visibleSize = Director::getInstance()->getVisibleSize();
	//	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//	
	//	taskLabel_->setPosition(Vec2(origin.x + visibleSize.width / 2,
	//		origin.y + visibleSize.height / 2 + 70));
	//	taskLabel_->setOpacity(255);
	//	taskLabel_->setString("Hi");
	//	break;
	//}
	//case EventKeyboard::KeyCode::KEY_B:
	//{
	//	taskLabel_->setString("bye");
	//	break;
	//}
	//case EventKeyboard::KeyCode::KEY_ESCAPE:
	//{
	//	this->CloseApplication();
	//	break;
	//}
	//}
}

void HelloWorld::SaveScore()
{
	if(maxScore_ <= score_)
		UserDefault::getInstance()->setStringForKey("MaxScore", scoreLabel_->getString());
}

void HelloWorld::setLives(const int lives)
{
	if(lives > -1)
		liveSprites_[lives] = CreateSprite("emptyHeart.png", liveSprites_[lives]->getPosition(), 1);
}

void HelloWorld::SetMaxScore(const int s)
{
	maxScore_ = s;
	maxScoreLabel_->setString(std::to_string(maxScore_));
}

void HelloWorld::setScore(const int s)
{
	score_ = s;
	scoreLabel_->setString(std::to_string(score_));
}

void HelloWorld::SettingsMenuCallBack()
{
	//! TODO: set speed fall of words.
}

void HelloWorld::ShowAnswer()
{
	taskLabel_->setString(task_.getAnswer());
	taskLabel_->stopAllActions();
}

void HelloWorld::showEnd(const float dt)
{
	this->PlaySoundOnce("end.mp3");
	createEndScene();
	Director::getInstance()->pushScene(endScene_);
}

void HelloWorld::TaskIsOut()
{
	if (--lives_ != 0)
	{
		setLives(lives_);
		this->PlaySoundOnce("you-failed.mp3");
		scheduleOnce(schedule_selector(HelloWorld::updateTask), 1);
	}
	else
	{
		setLives(lives_);
		scheduleOnce(schedule_selector(HelloWorld::showEnd), 1);
	}
}

void HelloWorld::updateTask(const float dt)
{
	task_ = tm_.getRandTask();
	taskLabel_->setString(task_.getTask());
	this->initPosMove();
}