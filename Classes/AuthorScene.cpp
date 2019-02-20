
#include "AuthorScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* AuthorScene::createScene()
{
	return AuthorScene::create();
}

// on "init" you need to initialize your instance
bool AuthorScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto desingRes = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	auto scale = Director::getInstance()->getContentScaleFactor();


	auto author = Label::createWithTTF("Developer: V.Pitanov\nEmail: dtho47@yandex.ru", "fonts/Amsterdam.ttf", 64 * scale);
	author->setPosition(desingRes.width / 2, desingRes.height / 2);
	this->addChild(author);

	float delayTime = 2;
#ifdef _DEBUG
	delayTime = 0.1;
#endif


	auto delay = DelayTime::create(delayTime);
	auto nexScene = CallFunc::create([this]() {

		float visibleTime = 0.5;
#ifdef _DEBUG
		visibleTime = 0.1;
#endif

		auto scene = MenuScene::createScene();
		auto transition = TransitionFade::create(visibleTime, scene);
		Director::getInstance()->replaceScene(transition);
		
	});
	auto sec = Sequence::create(delay, nexScene, nullptr);

	this->runAction(sec);

	return true;
}