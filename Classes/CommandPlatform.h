#pragma once

#include "PlatformNode.h"
#include "GameScene.h"
#include "EventPlatform.h"


struct CommandPlatformPush
{
	static void execute(PlatformNode *platform)
	{
		if (platform == nullptr)
			return;
		if (platform->getStatus() != PlatformNode::PlatformStatus::GrabBall)
			return;

		auto pos = platform->getPosition() + cocos2d::Vec2(0, platform->getContentSize().height);
		auto vec = cocos2d::Vec2(0, 500);
		platform->setStatus(PlatformNode::PlatformStatus::NoGrab);
		PlatformPushEvent::event({ vec, pos });

	}
};