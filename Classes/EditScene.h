#pragma once

#if defined(_DEBUG)

#include <cocos2d.h>

#include "ContainerNode.h"

class GridNode;

class EditScene : public cocos2d::Scene
{
public:
	bool init();

	CREATE_FUNC(EditScene);
private:
	bool initMouseEvent();
	bool initClickedEvent();
	bool initKeyEvent();
	bool initPanelObject();
	bool initStatusObject();
	bool initLevel();
	bool updateStatusBlock();

	GridNode *grid;
	cocos2d::Node *statusNode = nullptr;
	cocos2d::Label* labelInfo = nullptr;
	cocos2d::Point mousePointFromGrid = {};
	cocos2d::Sprite* spriteMouse = nullptr;
	cocos2d::Node* fakeContainer = nullptr;

	struct MemCont
	{
		ContainerNode::SpriteID spriteID; 
		ContainerNode::COLOR colorID;
		ContainerNode::RotationContainer rotation;
		int hp;
		int exp;

		int coll;
		int row;

		bool init;
		
	};

	bool blockBuild = false;
	cocos2d::Node *colorWidget = nullptr;
	MemCont memoryContainerCfg = {};
	cocos2d::Label *labelGridInfo = nullptr;
};


#endif