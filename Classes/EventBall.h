#pragma once

#include <functional>

#include "cocos2d.h"
#include "BallSprite.h"
#include "GameScene.h"


enum class BallEventType
{
	CREATE,
	REMOVE,
	IS_EMPTY,
	PUSH,
	FIRE_MODE,
	NONE
};


template<BallEventType type>
struct BallEventTpl
{
	static std::string& getID()
	{
		static std::string name("ev_ball_id_" + std::to_string(static_cast<int>(type)));
		return name;
	}

	static void event(BallSprite *ball)
	{
		cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getID(), ball);
	}

	static cocos2d::EventListenerCustom* createLister(const std::function<void(cocos2d::EventCustom*)> &fn)
	{
		auto listener = cocos2d::EventListenerCustom::create(getID(), fn);
		return listener;
	}

};

struct BallCreateEvent : public BallEventTpl<BallEventType::CREATE>
{
	static cocos2d::EventListenerCustom* createLister(GameScene *scene)
	{
		auto listener = cocos2d::EventListenerCustom::create(getID(), [scene](cocos2d::EventCustom* event_)
		{
			auto ball = reinterpret_cast<BallSprite*>(event_->getUserData());
			if (ball == nullptr)
				CCLOG("WARRNING! Ball is nullptr from BallCreateEvent");
			CCLOG("ADD BALL");
			scene->getPlayer()->addBall(1);

			
		});

		return listener;
	}
};

struct BallIsEmptyEvent : public BallEventTpl<BallEventType::IS_EMPTY>
{
	static cocos2d::EventListenerCustom* createLister(GameScene *scene)
	{
		auto listener = cocos2d::EventListenerCustom::create(getID(), [scene](cocos2d::EventCustom* event_)
		{
			auto ball = reinterpret_cast<BallSprite*>(event_->getUserData());
			if (ball == nullptr)
				CCLOG("WARRNING! Ball is nullptr from BallIsEmptyEvent");
			
			CCLOG("ALL BALLS FAILED");
			scene->Loose();
		});
		return listener;
	}
};

struct BallRemoveEvent : public BallEventTpl<BallEventType::REMOVE>
{
	static cocos2d::EventListenerCustom* createLister(GameScene *scene)
	{
		auto listener = cocos2d::EventListenerCustom::create(getID(), [scene](cocos2d::EventCustom* event_)
		{
			auto ball = reinterpret_cast<BallSprite*>(event_->getUserData());
			if (ball == nullptr)
				CCLOG("WARRNING! Ball is nullptr from BallRemoveEvent");
			
			scene->getPlayer()->delBall();

			CCLOG("BALL REMOVED");
			if (scene->getPlayer()->getBall() <= 0)
				BallIsEmptyEvent::event(ball);
		});
		return listener;
	}
};


struct BallFireModeEvent : public BallEventTpl<BallEventType::FIRE_MODE>
{
	static void event(bool mode)
	{
		cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getID(), &mode);
	}

	static cocos2d::EventListenerCustom* createLister(BallSprite *ball)
	{
		auto listener = cocos2d::EventListenerCustom::create(getID(), [ball](cocos2d::EventCustom* event_)
		{
			bool *mode = reinterpret_cast<bool*>(event_->getUserData());
			if (mode == nullptr)
				CCLOG("WARRNING! Ball is nullptr from BallFireModeEvent");

			if (*mode)
			{
				ball->fireEffectEnable();
			}
			else
			{
				ball->fireEffectDisable();
			}
		});
		return listener;
	}
};





