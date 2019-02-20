#pragma once

#include <functional>
#include <cocos2d.h>
#include "GameScene.h"

enum class PlayerEventType
{
	LOOSE,
	NONE
};

template<PlayerEventType type>
struct EventPlayerTpl
{
	static std::string& getID()
	{
		static std::string name("ev_player_id_" + std::to_string(static_cast<int>(type)));
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

struct PlayerLooseEvent : public EventPlayerTpl<PlayerEventType::LOOSE>
{

	static void event()
	{
		EventPlayerTpl<PlayerEventType::LOOSE>::event(nullptr);
	}

	static cocos2d::EventListenerCustom* createLister(GameScene *scene)
	{
		auto listener = cocos2d::EventListenerCustom::create(getID(), [scene](cocos2d::EventCustom *event)
		{
			scene->Loose();
		});
		return listener;
	}
};