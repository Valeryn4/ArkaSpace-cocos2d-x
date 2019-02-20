#include "LevelListScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* LevelListScene::createScene()
{
	return LevelListScene::create();
}

// on "init" you need to initialize your instance
bool LevelListScene::init()
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


	return true;
}