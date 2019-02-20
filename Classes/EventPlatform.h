#pragma once

#include <functional>
#include <tuple>
#include "cocos2d.h"
#include "PlatformNode.h"
#include "GameScene.h"
#include "BallSprite.h"
#include "EventBall.h"

enum class PlatformEventType
{
	PUSH,
	NONE
};

template<PlatformEventType type>
struct PlatformEventTpl
{
	static std::string& getID()
	{
		static std::string name("ev_platf_id_" + std::to_string(static_cast<int>(type)));
		return name;
	}

	static void event(void *data)
	{
		cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(getID(), data);
	}

	static cocos2d::EventListenerCustom* createLister(const std::function<void(cocos2d::EventCustom*)> &fn)
	{
		auto listener = cocos2d::EventListenerCustom::create(getID(), fn);
		return listener;
	}

};

struct PlatformPushEvent : public PlatformEventTpl<PlatformEventType::PUSH>
{
	using DataType = std::tuple<cocos2d::Vec2, cocos2d::Point>;
	static void event(DataType data)
	{

		PlatformEventTpl<PlatformEventType::PUSH>::event(&data);
	}

	static cocos2d::EventListenerCustom* createLister(cocos2d::Node *parent)
	{
		auto listener = cocos2d::EventListenerCustom::create(getID(), [parent](cocos2d::EventCustom *event)
		{
			auto data = reinterpret_cast<PlatformPushEvent::DataType *>(event->getUserData());
			if (data == nullptr)
				return;

			auto point = std::get<1>(*data);
			auto vec = std::get<0>(*data);

			auto ball = BallSprite::create();
			if (ball == nullptr)
				return;
			ball->setPosition(point);
			ball->push(vec);
			parent->addChild(ball);

			BallCreateEvent::event(ball);
		});
		return listener;
	}
};