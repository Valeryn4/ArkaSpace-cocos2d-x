#include "GameScene.h"
#include "Defines.h"
#include "PhysicsShapeCache.h"

#include "EventBall.h"

#include "EventPlatform.h"
#include "CommandPlatform.h"

#include "EventPlayer.h"

#include "MenuScene.h"

#include "LevelData.h"

#include "ScreenFormat.h"

USING_NS_CC;

auto dispatcher_ = Director::getInstance()->getEventDispatcher();

GameScene * GameScene::createScene(int level_)
{
	auto scene = GameScene::create(level_);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -1000));

	return scene;
}

GameScene * GameScene::create(int level_)
{
	

	GameScene *pRet = new(std::nothrow) GameScene();
	if (pRet && pRet->init(level_))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}

}

bool GameScene::init(int level_)
{
	if (!this->initWithPhysics())
		return false;

	this->setName(GAME_SCENE_NAME);

	m_root = ScreenFormat::createRoot();
	this->addChild(m_root);

	auto gameRect = ScreenFormat::getGameZoneRect();
	m_gameNodeBox = Node::create();
	m_gameNodeBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	m_gameNodeBox->setPosition(gameRect.origin);
	m_gameNodeBox->setContentSize(gameRect.size);
	m_root->addChild(m_gameNodeBox);

	auto headerRect = ScreenFormat::getHeaderRect();
	m_headerNodeBox = Node::create();
	m_headerNodeBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	m_headerNodeBox->setPosition(headerRect.origin);
	m_headerNodeBox->setContentSize(headerRect.size);
	m_root->addChild(m_headerNodeBox);

	auto bottomRect = ScreenFormat::getBottomRect();
	m_bottomNodeBox = Node::create();
	m_bottomNodeBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	m_bottomNodeBox->setPosition(bottomRect.origin);
	m_bottomNodeBox->setContentSize(bottomRect.size);
	m_root->addChild(m_bottomNodeBox);
	
#ifdef _DEBUG
	this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#else
	this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
#endif

	if (!this->initLevel())
		return false;

	if (!this->initHeader())
		return false;

	if (!this->initKeyBoardEvent())
		return false;

	if (!this->initPlatform())
		return false;

	if (!this->initBall())
		return false;
	
	if (!this->initMouseEvent())
		return false;

	if (!this->initCollisionEvent())
		return false;

	if (!this->initPlayer())
		return false;

	if (!this->initBonusMap())
		return false;

#if defined(_DEBUG)
	showDebug();
#endif

	this->scheduleUpdate();
	return true;
}

void GameScene::Loose()
{
	auto scene = MenuScene::createScene();
	auto transition = TransitionFade::create(0.1f, scene);
	Director::getInstance()->replaceScene(transition);
}

bool GameScene::initPlatform()
{
	platformNode = PlatformNode::create();
	if (platformNode == nullptr)
		return false;

	m_gameNodeBox->addChild(platformNode);

	platformNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	platformNode->setPosition(m_gameNodeBox->getContentSize().width * 0.5f, platformNode->getContentSize().height);

	auto listenerPush = PlatformPushEvent::createLister(m_gameNodeBox);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerPush, this);

	return true;
}

bool GameScene::initLevel()
{
	if (!initGrid())
		return false;

	if (!genLevel())
		return false;


	if (!initCollisionLevel())
		return false;

	if (!initBackground())
		return false;

	/*BOTTOM INIT*/
	auto bottom = Node::create();
	bottom->setContentSize(Size(m_gameNodeBox->getContentSize().width, 2));
	
	auto bodyBottomZone = PhysicsBody::createBox(bottom->getContentSize(), DEFAULT_MAERIAL);
	bodyBottomZone->setCollisionBitmask(LEVEL_BOTTOM_COLLISION_BITMASK | BALL_COLLISION_MASK);
	bodyBottomZone->setCategoryBitmask(LEVEL_BOTTOM_COLLISION_BITMASK);
	bodyBottomZone->setContactTestBitmask(BALL_COLLISION_MASK | BONUS_COLLISION_MASK | CRISTAL_COLLISION_MASK);
	bodyBottomZone->setDynamic(false);

	bottom->addComponent(bodyBottomZone);
	bottom->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	bottom->setPosition(0,0);

	m_gameNodeBox->addChild(bottom);

	/*Add build_1*/

	/**/
	auto gameSize = m_gameNodeBox->getContentSize();
	for (float x = 0; x < gameSize.width; )
	{
		auto buildSprite = Sprite::createWithSpriteFrameName("build_01.png");
		buildSprite->setScale(0.5);
		auto size = buildSprite->getContentSize();
		buildSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		buildSprite->setPosition(x, gameSize.height);
		x += size.width * buildSprite->getScale() - 0.5f;
		m_gameNodeBox->addChild(buildSprite);
	}
	

	for (float x = 0; x < gameSize.width; )
	{
		auto buildSprite = Sprite::createWithSpriteFrameName("build_01.png");
		buildSprite->setScale(0.5);
		auto size = buildSprite->getContentSize();
		buildSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		buildSprite->setPosition(x, 0.f);
		x += size.width * buildSprite->getScale() - 0.5f;
		m_gameNodeBox->addChild(buildSprite);
	}

	{
		auto arrowSpriteLeft = Sprite::create("arrow_01.png");
		auto tex = arrowSpriteLeft->getTexture();
		auto bottomSize = m_bottomNodeBox->getContentSize();
		float opacity = 100.f;

		Texture2D::TexParams params;
		params.minFilter = GL_LINEAR;
		params.magFilter = GL_LINEAR; //GL_NEAREST;
		params.wrapS = GL_REPEAT;
		params.wrapT = GL_REPEAT;
		tex->setTexParameters(params);

		auto height = arrowSpriteLeft->getContentSize().height;

		auto texRect = arrowSpriteLeft->getTextureRect();
		arrowSpriteLeft->setTextureRect(Rect(
			{ 0,0 },
			Size(bottomSize.width, height)
		));

		arrowSpriteLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		arrowSpriteLeft->setPosition(bottomSize.width * 0.5f, bottomSize.height * 0.5f);
		arrowSpriteLeft->setScale(0.5);
		arrowSpriteLeft->setOpacity(opacity);
		m_bottomNodeBox->addChild(arrowSpriteLeft, -1);

		auto arrowSpriteRight = Sprite::createWithTexture(arrowSpriteLeft->getTexture());
		arrowSpriteRight->setTextureRect(Rect(
			{ 0,0 },
			Size(bottomSize.width, height)
		));

		arrowSpriteRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		arrowSpriteRight->setPosition(bottomSize.width * 0.5f, bottomSize.height * 0.5f);
		arrowSpriteRight->setScale(0.5);
		arrowSpriteRight->setOpacity(opacity);
		arrowSpriteRight->setFlippedX(true);

		m_bottomNodeBox->addChild(arrowSpriteRight, -1);

	}



	return true;
}

bool GameScene::initGrid()
{
	
	gridNode = GridNode::create();
	gridNode->setContentSize(m_gameNodeBox->getContentSize());
	gridNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	gridNode->setPosition(0.f, 0.f);
	gridNode->setSizeGrid(COLL_GRID, ROW_GRID);
	m_gameNodeBox->addChild(gridNode);

#if defined(_DEBUG)
	gridNode->showDebug();
#endif
	return true;
}

bool GameScene::initCollisionLevel()
{
	/*COLLISION LEVEL INIT*/

	auto bodyGameZone = PhysicsBody::createEdgeBox(m_gameNodeBox->getContentSize(), DEFAULT_MAERIAL);
	if (bodyGameZone == nullptr)
		return false;

	bodyGameZone->setCollisionBitmask(LEVEL_COLLISION_BITMASK | BALL_COLLISION_MASK | CRISTAL_COLLISION_MASK);
	bodyGameZone->setCategoryBitmask(LEVEL_COLLISION_BITMASK);
	bodyGameZone->setDynamic(false);

	m_gameNodeBox->addComponent(bodyGameZone);

	return true;
}

bool GameScene::initBackground()
{
	/*BG*/
	auto bgSprite = Sprite::create("blue.png");
	auto tex = bgSprite->getTexture();
	

	Texture2D::TexParams params;
	params.minFilter = GL_LINEAR;
	params.magFilter = GL_LINEAR; //GL_NEAREST;
	params.wrapS = GL_REPEAT;
	params.wrapT = GL_REPEAT;
	tex->setTexParameters(params);

	bgSprite->setTextureRect(cocos2d::Rect({ 0,0 }, m_gameNodeBox->getContentSize()));
	bgSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	bgSprite->setPosition(0, 0);
	m_gameNodeBox->addChild(bgSprite, -1);

	return true;
}

bool GameScene::initBall()
{
	auto listenerCreate = BallCreateEvent::createLister(this);
	if (listenerCreate == nullptr)
		return false;
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerCreate, this);

	auto listenerRemove = BallRemoveEvent::createLister(this);
	if (listenerRemove == nullptr)
		return false;
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerRemove, this);

	auto listenerIsEmpty = BallIsEmptyEvent::createLister(this);
	if (listenerIsEmpty == nullptr)
		return false;
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerIsEmpty, this);

	return true;
}

bool GameScene::initKeyBoardEvent()
{
	auto listerKey = EventListenerKeyboard::create();
	if (listerKey == nullptr)
		return false;
	listerKey->onKeyPressed = CC_CALLBACK_2(GameScene::onPressKeyboardEvent, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listerKey, this);

	return true;
}

bool GameScene::initMouseEvent()
{
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	
	auto mouseLister = EventListenerMouse::create();
	if (mouseLister == nullptr)
		return false;
	mouseLister->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMoveEvent, this);
	mouseLister->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDownEvent, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseLister, this);

#else

	auto touchListener = EventListenerTouchOneByOne::create();
	if (touchListener == nullptr)
		return false;
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoveEvent, this);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchEvent, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	
#endif

	return true;
}

bool GameScene::initCollisionEvent()
{
	auto contactListener = EventListenerPhysicsContact::create();
	if (contactListener == nullptr)
		return false;

	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBeginEvent, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
	return true;
}

bool GameScene::initHeader()
{

	headerNode = HeaderNode::create(m_headerNodeBox->getContentSize());
	headerNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	if (headerNode == nullptr)
		return false;
	

	headerNode->setPosition(0.f, 0.f);
	
	m_headerNodeBox->addChild(headerNode);
	return true;
}

bool GameScene::initPlayer()
{

	auto listenerLoose = PlayerLooseEvent::createLister(this);
	if (listenerLoose == nullptr)
		return false;
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerLoose, this);

	return true;
}

bool GameScene::initBonusMap()
{
	
	auto insertBonuse = [this](BonusType type) -> BonusNode*
	{
		auto node = BonusNode::create(type);
		m_mapBonus.emplace(type, node);
		m_root->addChild(node);
		return node;	
	};

	/*
		fire bonus
	*/
	auto fire = insertBonuse(BonusType::Fire);
	fire->onActivated = [fire, this]()
	{
		player.addDamage(player.getDamage() * 2);
		m_ballCollision = false;
		BallFireModeEvent::event(true);

		auto delay = DelayTime::create(3);
		auto deactivate = CallFunc::create([fire]()
		{
			fire->deactivate();
		});

		auto seq = Sequence::create(delay, deactivate, nullptr);

		m_root->runAction(seq);
	};
	fire->onDeactivated = [this]()
	{
		player.delDamage(player.getDamage() / 2);
		m_ballCollision = true;
		BallFireModeEvent::event(false);
	};


	/*
		split bonus
	*/
	auto split = insertBonuse(BonusType::Split);
	split->onActivated = [split, this]()
	{
	
		auto node = m_gameNodeBox->getChildByName(BALL_NAME);
		if (node == nullptr)
		{
			split->deactivate();
			return;
		}
		auto ball = dynamic_cast<BallSprite*>(node);

		auto pos =  ball->getPosition();


		auto point_ = cocos2d::Vec2(0, 500);

#if defined (_DEBUG)

		auto drawNode = DrawNode::create();
		drawNode->drawPoint(pos, 15, Color4F(1.f, 0.3f, 0.7f, 1.0f));
		debugDraw.push_back(drawNode);
		m_gameNodeBox->addChild(drawNode);
#endif
		
		
		auto rotate = [](const Vec2 &point, float angle) {
			Vec2 rotated_point;
			rotated_point.x = point.x * cos(angle) - point.y * sin(angle);
			rotated_point.y = point.x * sin(angle) + point.y * cos(angle);
			return rotated_point;
		};

	
		CCLOG(" POS X %f  Y %f", pos.x, pos.y);
		for (int i = 0; i < 3; i++)
		{
			auto vec = rotate(point_, rand() % 360 - 180);
			PlatformPushEvent::event({ vec, pos });
		}

		split->deactivate();
		
	};
	split->onDeactivated = []()
	{};




	return true;
}

bool GameScene::genLevel()
{

	auto res =  LevelData::loadMapFromFile("0.level", gridNode);
	
	container_count = gridNode->getObjectListFromGrid().size();

	return res;
	/*FILL LEVEL*/
	/*
	int cols = gridNode->getColls();
	int row_level = 8;
	int exp = 10;
	int hp = 1;
	for (int x = 1; x < cols - 2; ++x)
	{
		for (int y = 1; y < row_level - 1; ++y)
		{
			auto h = rand() % 2;
			auto type = rand() % 7;
			auto box = ContainerNode::create(exp, hp, ContainerNode::SpriteID(type), ContainerNode::RotationContainer(h));
			bool res = false;
			if (res = gridNode->addObjectFromGrid(x, y, box); !res)
			{
				if (h)
					h = 0;
				else
					h = 1;
				auto box = ContainerNode::create(exp, hp, ContainerNode::SpriteID(type), ContainerNode::RotationContainer(h));

				if (res = gridNode->addObjectFromGrid(x, y, box); res)
					CCLOG("Add object from grid failed!");
			}
			
			if (res)
				this->container_count++;
		}
	}

	for (int x = 1; x < cols - 1; ++x)
	{
		for (int y = 1; y < row_level; ++y)
		{
			if (gridNode->isPositionFree(x, y))
			{
				auto num = rand() % 3;
				auto id =
					num == 0 ? ContainerNode::SpriteID::BLUE_SMALL :
					num == 1 ? ContainerNode::SpriteID::BLUE_MK2_SMALL :
					ContainerNode::SpriteID::RED_SMALL;

				auto box = ContainerNode::create(exp, hp, id);
				bool res = false;
				if (res = gridNode->addObjectFromGrid(x, y, box); !res)
				{
					CCLOG("Add object from grid failed!");
				}
				
				if (res)
					this->container_count++;
			}
		}
	}

	CCLOG("Level generate %ui block", this->container_count);

	if (!LevelData::saveMapToFile(gridNode, "0.level"))
	{
		CCLOG("Failed save level");
	}

	return true;

	*/
	
}

bool GameScene::pushBall()
{
	if (platformNode == nullptr)
		return false;
	if (platformNode->getStatus() != PlatformNode::PlatformStatus::GrabBall)
		return false;

	auto pos = platformNode->getPosition() + cocos2d::Vec2(0, platformNode->getContentSize().height);
	auto vec = cocos2d::Vec2(0, 500);
	platformNode->setStatus(PlatformNode::PlatformStatus::NoGrab);
	PlatformPushEvent::event({ vec, pos });
	return true;
}

void GameScene::update(float delta)
{

	Scene::update(delta);
}

void GameScene::onPressKeyboardEvent(EventKeyboard::KeyCode code, Event *)
{

#if defined(_DEBUG)
	if (code == EventKeyboard::KeyCode::KEY_D)
	{
		if (is_showDebug)
		{
			//levelNode->hideDebug();
			this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
			this->hideDebug();
			headerNode->hideDebug();
			gridNode->hideDebug();
			is_showDebug = false;
		}
		else
		{
			//levelNode->showDebug();
			this->showDebug();
			this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
			headerNode->showDebug();
			gridNode->showDebug();
			is_showDebug = true;
		}
	}
#endif
}

void GameScene::moveEvent(float pos_x)
{
	
	if (platformNode == nullptr)
		return;

	auto bottom_size = m_bottomNodeBox->getContentSize();
	auto platform_width = platformNode->getContentSize().width * 0.5f;


	if (pos_x >= bottom_size.width - platform_width)
		pos_x = bottom_size.width - platform_width;

	if (pos_x <= platform_width)
		pos_x = platform_width;

	platformNode->setPositionX(pos_x);
}

void GameScene::onMouseMoveEvent(cocos2d::EventMouse * event)
{
	auto pos_global = Vec2(event->getCursorX(), event->getCursorY());
	auto pos_x = m_bottomNodeBox->convertToNodeSpace(pos_global).x;
	moveEvent(pos_x);

	
}

void GameScene::onTouchMoveEvent(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto pos_x = m_bottomNodeBox->convertTouchToNodeSpace(touch).x;
	moveEvent(pos_x);
}

bool GameScene::onTouchEvent(cocos2d::Touch * touch, cocos2d::Event * event)
{
	return pushBall();
}

void GameScene::onMouseDownEvent(EventMouse * event)
{
	pushBall();
}

bool GameScene::onContactBeginEvent(cocos2d::PhysicsContact & contact)
{
	//using collision_t = int;
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	//collision_t maskA = contact.getShapeA()->getBody()->getCollisionBitmask();
	//collision_t maskB = contact.getShapeB()->getBody()->getCollisionBitmask();
	int maskCatA = contact.getShapeA()->getBody()->getCategoryBitmask();
	int maskCatB = contact.getShapeB()->getBody()->getCategoryBitmask();

	if (!nodeA || !nodeB)
		return false;


	auto swapPointer = [&nodeA, &nodeB, maskCatA, maskCatB](int A, int B) -> void
	{
		if (maskCatB == A && maskCatA == B)
		{
			std::swap(nodeA, nodeB);
		}
	};


	switch (maskCatA | maskCatB)
	{
	case BALL_COLLISION_MASK | LEVEL_BOTTOM_COLLISION_BITMASK:
		swapPointer(BALL_COLLISION_MASK, LEVEL_BOTTOM_COLLISION_BITMASK);
		collisionBallBottom(nodeA, nodeB);
		break;
	case BALL_COLLISION_MASK | BRICK_COLLISION_MASK:
		swapPointer(BALL_COLLISION_MASK, BRICK_COLLISION_MASK);
		collisionBallBrick(nodeA, nodeB);

		if (!m_ballCollision)
			return false;
		break;
	case BALL_COLLISION_MASK | PLATFORM_COLLISION_MASK:

		break;
	case BONUS_COLLISION_MASK | PLATFORM_COLLISION_MASK:
		swapPointer(BONUS_COLLISION_MASK, PLATFORM_COLLISION_MASK);
		collisionBonusPlatform(nodeA, nodeB);
		break;
	case BONUS_COLLISION_MASK | LEVEL_BOTTOM_COLLISION_BITMASK:
		swapPointer(BONUS_COLLISION_MASK, LEVEL_BOTTOM_COLLISION_BITMASK);
		collisionBonusBottom(nodeA, nodeB);
		break;
	case CRISTAL_COLLISION_MASK | PLATFORM_COLLISION_MASK:
		swapPointer(CRISTAL_COLLISION_MASK, PLATFORM_COLLISION_MASK);
		collisionCristalPlatform(nodeA, nodeB);
		break;
	case CRISTAL_COLLISION_MASK | LEVEL_BOTTOM_COLLISION_BITMASK :
		swapPointer(CRISTAL_COLLISION_MASK, LEVEL_BOTTOM_COLLISION_BITMASK);
		collisionCristalBottom(nodeA, nodeB);
		break;
	default:
		return false;
	}

	return true;



	/*
	//BALL
	if (maskCatA == BALL_COLLISION_MASK || maskCatB == BALL_COLLISION_MASK) 
	{
		if (maskCatA == BALL_COLLISION_MASK && maskCatB == BRICK_COLLISION_MASK)
		{

			CCLOG("Collision ball brick A");
			collisionBallBrick(nodeA, nodeB);
			return true;
		}
		else if (maskCatB == BALL_COLLISION_MASK && maskCatA == BRICK_COLLISION_MASK)
		{

			CCLOG("Collision ball brick B");
			collisionBallBrick(nodeB, nodeA);
			return true;
		}

		if (maskCatA == BALL_COLLISION_MASK && maskCatB == LEVEL_BOTTOM_COLLISION_BITMASK)
		{

			CCLOG("Collision ball brick A");
			collisionBallBottom(nodeA, nodeB);
			return true;
		}
		else if (maskCatB == BALL_COLLISION_MASK && maskCatA == LEVEL_BOTTOM_COLLISION_BITMASK)
		{

			CCLOG("Collision ball brick B");
			collisionBallBottom(nodeB, nodeA);
			return true;
		}
	}
	*/

	/*
	//BONUS
	if (maskCatA == BONUS_COLLISION_MASK || maskCatB == BONUS_COLLISION_MASK)
	{
		if (maskCatA == BONUS_COLLISION_MASK && maskCatB == PLATFORM_COLLISION_MASK)
		{
			CCLOG("Collision bonus platform A");
			collisionBonusPlatform(nodeA, nodeB);
			return true;
		}
		else if (maskCatA == PLATFORM_COLLISION_MASK && maskCatB == BONUS_COLLISION_MASK)
		{
			CCLOG("Collision bonus platform B");
			collisionBonusPlatform(nodeB, nodeA);
			return true;
		}

		if (maskCatA == BONUS_COLLISION_MASK && maskCatB == LEVEL_BOTTOM_COLLISION_BITMASK)
		{
			CCLOG("Collision bonus bottom A");
			collisionBonusBottom(nodeA, nodeB);
			return true;
		}
		else if (maskCatA == LEVEL_BOTTOM_COLLISION_BITMASK && maskCatB == BONUS_COLLISION_MASK)
		{
			CCLOG("Collision bonus bottom A");
			collisionBonusBottom(nodeB, nodeA);
			return true;

		}
	}

	*/
	
	/*
	//CRISTAL
	if (maskCatA == CRISTAL_COLLISION_MASK || maskCatB == CRISTAL_COLLISION_MASK)
	{

		if (maskCatA == CRISTAL_COLLISION_MASK && maskCatB == PLATFORM_COLLISION_MASK)
		{
			CCLOG("Collision cristal platform A");
			collisionCristalPlatform(nodeA, nodeB);
			return true;
		}
		else if (maskCatA == PLATFORM_COLLISION_MASK && maskCatB == CRISTAL_COLLISION_MASK)
		{
			CCLOG("Collision cristal platform B");
			collisionCristalPlatform(nodeB, nodeA);
			return true;
		}

		if (maskCatA == CRISTAL_COLLISION_MASK && maskCatB == LEVEL_BOTTOM_COLLISION_BITMASK)
		{
			CCLOG("Collision bonus bottom A");
			collisionCristalBottom(nodeA, nodeB);
			return true;
		}
		else if (maskCatA == LEVEL_BOTTOM_COLLISION_BITMASK && maskCatB == CRISTAL_COLLISION_MASK)
		{
			CCLOG("Collision bonus bottom A");
			collisionCristalBottom(nodeB, nodeA);
			return true;

		}
	}
	*/

	//CCLOG("A:%i\nB:%i\nA cat:%i\nB cat:%i", maskA, maskB, maskCatA, maskCatB);

	//bodies can collide
	return true;
}

void GameScene::collisionBallBrick(cocos2d::Node * ball_, cocos2d::Node * brick_)
{
	auto ball = reinterpret_cast<BallSprite*>(ball_);
	auto container = reinterpret_cast<ContainerNode*>(brick_->getParent());

	auto body = ball->getPhysicsBody();
	Vec2 vecDMG = body->getVelocity() * 0.05f;
	container->damage(player.getDamage());
	if (container->getHP() <= 0)
	{
		destroyContainer(container);
	}
	else
	{
		auto test1 = body->getVelocityAtLocalPoint(ball->getPosition());

		auto test2 = body->getAngularVelocity();
		Vec2 vecDMG = body->getVelocity() * 0.05f;
		container->damageAnimation(vecDMG);
	}


}

void GameScene::collisionBallBottom(cocos2d::Node * ball_, cocos2d::Node * bottom)
{
	auto ball = reinterpret_cast<BallSprite*>(ball_->getParent());
	if (ball == nullptr)
		return;

	BallRemoveEvent::event(ball);
	ball->removeFromParentAndCleanup(true);

}

void GameScene::collisionBonusBottom(cocos2d::Node * bonus_, cocos2d::Node * bottom)
{
	bonus_->removeFromParentAndCleanup(true);
}

void GameScene::collisionBonusPlatform(cocos2d::Node * bonus_, cocos2d::Node * platform)
{
	
}

void GameScene::collisionCristalPlatform(cocos2d::Node * cristal_, cocos2d::Node * platform)
{
	auto cristal = reinterpret_cast<CristalNode*>(cristal_->getParent());
	headerNode->addScore(cristal->getExpirence());
	headerNode->update();
	cristal->destroy();
	cristal->removeFromParentAndCleanup(true);
}

void GameScene::collisionCristalBottom(cocos2d::Node * cristal_, cocos2d::Node * bottom)
{
	auto cristal = reinterpret_cast<CristalNode*>(cristal_->getParent());
	cristal->destroy();
	cristal->removeFromParentAndCleanup(true);
}

void GameScene::destroyContainer(ContainerNode *obj)
{
	auto pos = obj->getPosition();
	auto parent = obj->getParent();
	auto exp = obj->getExp();
	int blockCount = obj->getSizeFromGrid().col_size * obj->getSizeFromGrid().row_size;

	auto genCristal = [&]()
	{
		auto d100 = rand() % 100 + 1;
		auto type =
			d100 <= 70 ? CristalNode::CristalType::BLUE :
			d100 <= 85 ? CristalNode::CristalType::GREEN :
			d100 <= 95 ? CristalNode::CristalType::YELLOW :
			CristalNode::CristalType::RED;

		auto cristal = CristalNode::create(type, exp);

		auto size = cristal->getContentSize();// *cristal->getScale();

		cristal->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		cristal->setPosition(pos);
		//cristal->initPhysics();
		parent->addChild(cristal);

		auto rotation = rand() % 360;
		cristal->setRotation(rotation);


		auto callbackInitPhysics = CallFunc::create([cristal]()
		{

			cristal->initPhysics();

		});

		auto callbackApplyImpulse = CallFunc::create([cristal]()
		{

			auto impulseOffset = ( rand() % 10 - 5 ) * (100 * 100);
			auto impulceForce = (rand() % 10) * (100 * 100);
			cristal->applyImpulse(Vec2(impulseOffset, impulceForce));
			

		});
		auto sec = Sequence::create(callbackInitPhysics, callbackApplyImpulse, nullptr);

		cristal->runAction(sec);

	};

	auto count = (rand() % (4 * blockCount - blockCount)) + blockCount;

	for (int i = 0; i < count; i++)
	{
		genCristal();
	}

	
#if defined(_DEBUG)

	auto drawYellow = DrawNode::create();
	drawYellow->drawDot(pos, 5, Color4F::YELLOW);
	parent->addChild(drawYellow, 100);
	this->debugDraw.push_back(drawYellow);

	if (!is_showDebug)
	{
		drawYellow->setVisible(false);
	}
#endif

	auto scaleAction = ScaleTo::create(0.2f, 0.1f);
	auto removeAction = CallFunc::create([obj, this]() {
		ContainerNode::destroy(obj);
		this->container_count--;
		if (this->container_count == 0)
		{
			this->nextLevel();
		}
	});
	auto sec = Sequence::create(scaleAction, removeAction, nullptr);

	obj->runAction(sec);
}

void GameScene::nextLevel()
{
	auto scene = GameScene::createScene(this->level + 1);
	scene->player = this->player;
	auto transition = TransitionFade::create(0.1f, scene);
	Director::getInstance()->replaceScene(transition);
}





#if defined(_DEBUG)
void GameScene::showDebug()
{
	is_showDebug = true;
	Director::getInstance()->setDisplayStats(true);
	for (auto &&v : debugDraw)
	{
		v->setVisible(true);
	}
}

void GameScene::hideDebug()
{
	is_showDebug = false;
	Director::getInstance()->setDisplayStats(false);
	for (auto &&v : debugDraw)
	{
		v->setVisible(false);
	}
}


#endif
