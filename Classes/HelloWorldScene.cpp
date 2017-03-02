#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include <fstream>
#include <string>

USING_NS_CC;

int HelloWorld::endLevelScore_ = 400;
int HelloWorld::speed_ = 8;

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

void HelloWorld::CreateBackground()
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

void HelloWorld::createSettingsScene()
{
	settingsScene_ = Scene::create();

	Vector<MenuItem*> menuItems;
	Label* settingsLabel = Label::createWithTTF("Choose speed fall", LABEL_FONTNAME, 2 * LABEL_FONTSIZE);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 pos = Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - 2 * LABEL_FONTSIZE);
	settingsLabel->setPosition(pos);
	settingsScene_->addChild(settingsLabel);

	Vec2 middle(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2);

	auto lowSpeedItem = MenuItemImage::create(
		"NormalGreen.png",
		"PressedGreen.png",
		CC_CALLBACK_1(HelloWorld::menuSettingSpeedCallback, this, 8));
	lowSpeedItem->setPosition(middle.x - lowSpeedItem->getContentSize().width, middle.y);
	menuItems.pushBack(lowSpeedItem);

	auto middleSpeedItem = MenuItemImage::create(
		"NormalYellow.png",
		"PressedYellow.png",
		CC_CALLBACK_1(HelloWorld::menuSettingSpeedCallback, this, 4));
	middleSpeedItem->setPosition(lowSpeedItem->getPosition().x + lowSpeedItem->getContentSize().width, middle.y);
	menuItems.pushBack(middleSpeedItem);

	auto fastSpeedItem = MenuItemImage::create(
		"NormalRed.png",
		"PressedRed.png",
		CC_CALLBACK_1(HelloWorld::menuSettingSpeedCallback, this, 2));
	fastSpeedItem->setPosition(middleSpeedItem->getPosition().x + middleSpeedItem->getContentSize().width, middle.y);
	menuItems.pushBack(fastSpeedItem);

	textField_ = cocos2d::ui::TextField::create("Input value here, less is faster", LABEL_FONTNAME, LABEL_FONTSIZE);
	textField_->setPosition(Vec2(middle.x, middle.y - fastSpeedItem->getContentSize().height - LABEL_FONTSIZE));
	textField_->addEventListener(CC_CALLBACK_2(HelloWorld::textFieldEvent, this));
	settingsScene_->addChild(textField_);

	auto menu = Menu::createWithArray(menuItems);
	menu->setPosition(Vec2::ZERO);
	settingsScene_->addChild(menu, 1);
}

void HelloWorld::createWonScene()
{
	wonScene_ = Scene::create();

	Label* lbWon = Label::createWithTTF("You Won", LABEL_FONTNAME, 3 * LABEL_FONTSIZE);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 pos = Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2 + 60);
	lbWon->setPosition(pos);
	wonScene_->addChild(lbWon);

	auto replayItem = MenuItemImage::create("PlayButtonNormal.png",
		"PlayButtonNormalPressed.png",
		CC_CALLBACK_1(HelloWorld::menuReplayCallback, this));
	replayItem->setPosition(Vec2(pos.x, pos.y - lbWon->getContentSize().height - 10));

	auto menu = Menu::create(replayItem, NULL);
	menu->setPosition(Vec2::ZERO);
	wonScene_->addChild(menu, 1);
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
		liveSprites_.push_back(CreateSprite("fullHeart.png", sprite_pos, 1));
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

	Vector<MenuItem*> menuItems;
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

	auto settingsItem = MenuItemImage::create(
		"SettingsCloseNormal.png",
		"SettingsCloseSelected.png",
		CC_CALLBACK_1(HelloWorld::SettingsMenuCallBack, this)
	);

	settingsItem->setPosition(Vec2(origin.x + settingsItem->getContentSize().width / 2,
		origin.y + settingsItem->getContentSize().height / 2));

    // create menu, it's an autorelease object
	menuItems.pushBack(closeItem);
	menuItems.pushBack(settingsItem);
    auto menu = Menu::createWithArray(menuItems);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	CreateBackground();
	CreateLives();

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

	Vec2 rightAnsPos = Vec2(origin.x + LABEL_FONTSIZE * 2 + 11, origin.y + visibleSize.height - LABEL_FONTSIZE * 3);
	rightAnsLabel_ = this->CreateTextLabel(
		std::string("ra: " + std::to_string(task_.getRightAns())).c_str(), LABEL_FONTNAME, LABEL_FONTSIZE, rightAnsPos);

	Vec2 falseAnsPos = Vec2(origin.x + LABEL_FONTSIZE * 2 + 11, origin.y + visibleSize.height - LABEL_FONTSIZE * 4);
	falseAnsLabel_ = this->CreateTextLabel(
		std::string("fan: " + std::to_string(task_.getFalseAns())).c_str(), LABEL_FONTNAME, LABEL_FONTSIZE, falseAnsPos);

	//Vec2 lives_pos = Vec2(origin.x + visibleSize.width - 60, origin.y + visibleSize.height - 20);
	//this->CreateTextLabel("Lives: 2/5", LABEL_FONTNAME, LABEL_FONTSIZE, lives_pos);

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
	//auto maxScore = UserDefault::getInstance()->getStringForKey("MaxScore");
	//maxScoreLabel_->setString(maxScore);
	std::ifstream ifile(FileUtils::getInstance()->fullPathForFilename("maxscore.txt"));
	std::string line;
	std::getline(ifile, line);
	ifile.close();

	std::string scoreStr;

	size_t found = line.find(std::string("score: "));
	if (found != std::string::npos)
	{
		scoreStr.resize(line.size() - found - std::string("score: ").size());
		std::copy(line.begin() + found + std::string("score: ").size(), line.end(), scoreStr.begin());
	}

	this->SetMaxScore(std::atoi(scoreStr.c_str()));
	//SetMaxScore();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	this->CloseApplication();
}

void HelloWorld::menuReplayCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->popScene();
	ResetGame();
}

void HelloWorld::menuSettingSpeedCallback(cocos2d::Ref* pSender, const int speed)
{
	Director::getInstance()->popScene();
	speed_ = speed;
	ShowAnswer();
	ResetGame();
}

void HelloWorld::MoveObject(cocos2d::Sprite* obj, cocos2d::Vec2& moveTo, cocos2d::Vec2& scaleBy)
{
	auto seq = Sequence::create(MoveTo::create(speed_, moveTo), CallFuncN::create(CC_CALLBACK_0(HelloWorld::TaskIsOut, this)), NULL);
	auto acScaleBy = ScaleBy::create(2, scaleBy.x, scaleBy.y);

	obj->runAction(acScaleBy);
	obj->runAction(seq);
}

void HelloWorld::onKeyPressed(const cocos2d::EventKeyboard::KeyCode& keyCode, const cocos2d::Event* event)
{
	if (task_.isKeyCorrect(keyCode))
	{
		this->PlaySoundOnce("got-word.mp3");
		this->ShowAnswer();
		task_.SetRightAns(task_.getRightAns() + 1);
		tm_.SetWordTaskToList(task_);
		addScore(SCORE_CHANGE);
		scheduleOnce(schedule_selector(HelloWorld::updateTask), 1);
	}
	else if (EventKeyboard::KeyCode::KEY_ESCAPE == keyCode)
		this->CloseApplication();
	else
	{
		ShowAnswer();
		task_.SetFalseAns(task_.getFalseAns() + 1);
		tm_.SetWordTaskToList(task_);
		TaskIsOut();
		//scheduleOnce(schedule_selector(HelloWorld::showEnd), 1);
	}
}

void HelloWorld::PlaySoundOnce(const std::string& path)
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect(path.c_str(), false, 1.0f, 1.0f, 1.0f);
}

void HelloWorld::ResetGame()
{
	this->SaveScore();
	this->LoadScore();
	tm_.ClearIndexes();
	setScore(0);
	updateTask(0);
	ResetLives();
}

void HelloWorld::ResetLives()
{
	for (size_t i = 0; i != liveSprites_.size(); ++i)
	{
		this->removeChild(liveSprites_.at(i));
		liveSprites_.at(i) = nullptr;
	}
	liveSprites_.clear();

	CreateLives();
}

void HelloWorld::SaveScore()
{
	if (maxScore_ <= score_)
	{
		//UserDefault::getInstance()->setStringForKey("MaxScore", scoreLabel_->getString());
		std::ofstream ofile(FileUtils::getInstance()->fullPathForFilename("maxscore.txt"));
		ofile << "score: " << score_ << "\n";
		ofile.close();
	}
}

void HelloWorld::setLives(const int lives)
{
	try 
	{
		if (lives > -1)
		{
			liveSprites_[lives] = CreateSprite("emptyHeart.png", liveSprites_[lives]->getPosition(), 1);
		}
	}
	catch (...)
	{

	}
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

void HelloWorld::SettingsMenuCallBack(Ref* pSender)
{
	this->PlaySoundOnce("pause.mp3");
	createSettingsScene();
	Director::getInstance()->pushScene(settingsScene_);
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

void HelloWorld::showWon(const float dt)
{
	this->PlaySoundOnce("you_won.mp3");
	createWonScene();
	Director::getInstance()->pushScene(wonScene_);
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

void HelloWorld::textFieldEvent(cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type)
{
	if (type == ui::TextField::EventType::DETACH_WITH_IME)
	{
		Director::getInstance()->popScene();
		speed_ = std::atoi(textField_->getString().c_str());
		ShowAnswer();
		ResetGame();
	}
}

void HelloWorld::updateTask(const float dt)
{
	if (score_ == endLevelScore_)
	{
		lives_ = -1;
		setLives(lives_);
		scheduleOnce(schedule_selector(HelloWorld::showWon), 1);
	}
	else
	{
		task_ = tm_.getRandTask();
		taskLabel_->setString(task_.getTask());
		if (rightAnsLabel_ != nullptr && falseAnsLabel_ != nullptr)
		{
			rightAnsLabel_->setString(std::string("ra: " + std::to_string(task_.getRightAns())).c_str());
			falseAnsLabel_->setString(std::string("fan: " + std::to_string(task_.getFalseAns())).c_str());
		}
		this->initPosMove();
	}
}