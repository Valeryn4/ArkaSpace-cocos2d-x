
#include "PreviewScene.h"
#include "AuthorScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* PreviewScene::createScene()
{
	return PreviewScene::create();
}

// on "init" you need to initialize your instance
bool PreviewScene::init()
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

	auto cocosSprite = Sprite::create("cocos2d.png");
	if (cocosSprite == nullptr)
	{
		CCLOG("cocos2d.png is null!");

	}
	else
	{
		cocosSprite->setScale(Director::getInstance()->getContentScaleFactor());
		cocosSprite->setPosition(desingRes.width / 2, desingRes.height / 2);
		this->addChild(cocosSprite);
	}

	float delayTime = 1;
#ifdef _DEBUG
	delayTime = 0.1;
#endif


	auto delay = DelayTime::create(delayTime);
	auto nexScene = CallFunc::create([this]() {
		float visibleTime = 0.5;
#ifdef _DEBUG
		visibleTime = 0.001;
#endif
		auto scene = AuthorScene::createScene();
		auto transition = TransitionFade::create(visibleTime, scene);
		Director::getInstance()->replaceScene(transition);
		
	});

	auto sec = Sequence::create(delay, nexScene, nullptr);

	this->runAction(sec);

	return true;
}

