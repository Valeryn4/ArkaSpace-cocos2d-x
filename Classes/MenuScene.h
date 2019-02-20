#pragma once

#include "cocos2d.h"

class MenuScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MenuScene);

private:
	void Menu();
	void Play(cocos2d::Ref *);
	void Score(cocos2d::Ref *);
	void Exit(cocos2d::Ref *);
	void Info(cocos2d::Ref *);
};