#pragma once

#include <cocos2d.h>

#include "BallSprite.h"
#include "PlatformNode.h"
#include "HeaderNode.h"
#include "ContainerNode.h"
#include "GridNode.h"
#include "CristalNode.h"
#include "BonusNode.h"

#include "Command.h"

#include "PlayerStatus.h"


class GameScene : public cocos2d::Scene
{
public:
	static GameScene* createScene(int level_);
	static GameScene* create(int level_);

	bool init(int level_);
	void update(float delta);

	PlayerStatus* getPlayer()
	{
		return &player;
	}

	void Loose();

private:

	bool initPlatform();
	
	bool initLevel();
	bool initGrid();
	bool initCollisionLevel();
	bool initBackground();

	bool initBall();
	bool initKeyBoardEvent();
	bool initMouseEvent();
	bool initCollisionEvent();
	bool initHeader();
	bool initPlayer();
	bool initBonusMap();

	bool genLevel();
	bool pushBall();

	void onPressKeyboardEvent(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *);
	void moveEvent(float pos_x);
	void onMouseMoveEvent(cocos2d::EventMouse* event);
	void onMouseDownEvent(cocos2d::EventMouse* event);

	void onTouchMoveEvent(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouchEvent(cocos2d::Touch *touch, cocos2d::Event *event);

	bool onContactBeginEvent(cocos2d::PhysicsContact& contact);

	void collisionBallBrick(cocos2d::Node *ball, cocos2d::Node *brick);
	void collisionBallBottom(cocos2d::Node *ball, cocos2d::Node *bottom);

	void collisionBonusPlatform(cocos2d::Node *bonus, cocos2d::Node *platform);
	void collisionBonusBottom(cocos2d::Node *bonus, cocos2d::Node *bottom);
	
	void collisionCristalPlatform(cocos2d::Node *cristal, cocos2d::Node *platform);
	void collisionCristalBottom(cocos2d::Node *cristal, cocos2d::Node *bottom);

	void destroyContainer(ContainerNode* node);
	void nextLevel();

	int level = 1;
	unsigned int container_count = 0;

	PlatformNode *platformNode = nullptr;
	HeaderNode *headerNode = nullptr;
	GridNode *gridNode = nullptr;

	PlayerStatus player;

	cocos2d::Node* m_root;
	cocos2d::Node* m_gameNodeBox;
	cocos2d::Node* m_headerNodeBox;
	cocos2d::Node* m_bottomNodeBox;

	std::unordered_map<BonusType, BonusNode*> m_mapBonus;

	bool m_ballCollision = true;

#if defined(_DEBUG)

	void showDebug();
	void hideDebug();

	std::vector<cocos2d::DrawNode*> debugDraw;
	bool is_showDebug = true;
#endif


};
