
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#if defined(_DEBUG)
#include "EditScene.h"
#endif

USING_NS_CC;

Scene* MenuScene::createScene()
{
	return MenuScene::create();
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Menu();

	return true;
}

void MenuScene::Menu()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto desingRes = Director::getInstance()->getOpenGLView()->getDesignResolutionSize(); 
	auto scale = Director::getInstance()->getContentScaleFactor();

	MenuItemFont::setFontName("UB-Calculator");
	MenuItemFont::setFontSize(128 * scale);

	auto playItem = MenuItemFont::create("PLAY", CC_CALLBACK_1(MenuScene::Play, this));
	
	auto text = Label::createWithTTF("Play", "fonts/Calculator.ttf", 65 * scale);
	//playItem->addChild(text, 100);

	auto scoreItem = MenuItemFont::create("SCORE", CC_CALLBACK_1(MenuScene::Score, this));

	auto infoItem = MenuItemFont::create("INFO", CC_CALLBACK_1(MenuScene::Info, this));

	auto exitItem = MenuItemFont::create("EXIT", CC_CALLBACK_1(MenuScene::Exit, this));

#if defined(_DEBUG)

	auto editItem = MenuItemFont::create("edit", [](cocos2d::Ref *) 
	{
		CCLOG("Edit selected");
		auto scene = EditScene::create();
		auto transition = TransitionFade::create(0.1, scene);
		Director::getInstance()->replaceScene(transition);
	});
	auto menu = Menu::create(playItem, editItem, scoreItem, infoItem, exitItem, nullptr);
#else

	auto menu = Menu::create(playItem, scoreItem, infoItem, exitItem, nullptr);
#endif
	menu->setPosition(desingRes / 2);
	menu->alignItemsVerticallyWithPadding(64 * scale);

	this->addChild(menu, 1);
}

void MenuScene::Play(cocos2d::Ref *)
{
	CCLOG("Play select");

	auto scene = GameScene::createScene(1);
	auto transition = TransitionFade::create(0.1, scene);
	Director::getInstance()->replaceScene(transition);
}

void MenuScene::Score(cocos2d::Ref *)
{

	CCLOG("Score select");
}

void MenuScene::Exit(cocos2d::Ref *)
{
	CCLOG("Exit select");

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MenuScene::Info(cocos2d::Ref *)
{
	CCLOG("Info select");
}
