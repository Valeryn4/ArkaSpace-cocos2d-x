#include "PlatformNode.h"
#include "PhysicsShapeCache.h"
#include "Defines.h"

USING_NS_CC;

std::string PlatformNode::spriteName = "platform.png";
std::string PlatformNode::shapeName = "platform";

bool PlatformNode::init()
{

	auto sprite = Sprite::createWithSpriteFrameName(spriteName);
	this->setContentSize(sprite->getContentSize());
	auto res = PhysicsShapeCache::getInstance()->setBodyOnSprite(shapeName, sprite, DEFAULT_MAERIAL);
	if (!res)
		return false;

	auto body = sprite->getPhysicsBody();
	body->setDynamic(false);
	body->setCategoryBitmask(PLATFORM_COLLISION_MASK);
	body->setCollisionBitmask(PLATFORM_COLLISION_MASK | BALL_COLLISION_MASK | BONUS_COLLISION_MASK);
	body->setContactTestBitmask(BONUS_COLLISION_MASK | BALL_COLLISION_MASK | CRISTAL_COLLISION_MASK);

	this->addChild(sprite);

	this->setName(PLATFORM_NAME);

	status = PlatformStatus::GrabBall;
	sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sprite->setPosition(this->getContentSize() * 0.5);

	
	
	//this->setScale(0.5);
	return true;
}


