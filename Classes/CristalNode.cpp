#include "CristalNode.h"
#include "PhysicsShapeCache.h"
#include "Defines.h"

USING_NS_CC;

using CristT = CristalNode::CristalType;
CristalNode::SpriteMap CristalNode::mapSprite =
{
	{CristT::BLUE,	"item001.png"},
	{CristT::YELLOW,"item002.png"},
	{CristT::GREEN,	"item003.png"},
	{CristT::RED,	"item004.png"},

};

CristalNode::ShapeMap CristalNode::mapShape =
{
	{CristT::BLUE,	"item001"},
	{CristT::YELLOW,"item002"},
	{CristT::GREEN,	"item003"},
	{CristT::RED,	"item004"},
};


bool CristalNode::init()
{
	return true;
}

bool CristalNode::initPhysics()
{
	auto shapeCache = PhysicsShapeCache::getInstance();
	auto res = shapeCache->setBodyOnSprite(mapShape[type], sprite, CRISTAL_MATTERIAL);
	if (!res)
		return false;

	auto body = sprite->getPhysicsBody();
	body->setCollisionBitmask(PLATFORM_COLLISION_MASK | LEVEL_COLLISION_BITMASK);
	body->setCategoryBitmask(CRISTAL_COLLISION_MASK);
	body->setContactTestBitmask(PLATFORM_COLLISION_MASK | LEVEL_BOTTOM_COLLISION_BITMASK);
	body->setDynamic(true);

	return true;
}


CristalNode * CristalNode::create(CristalType type, int expirence)
{
	auto node = create();
	if (node == nullptr)
		return nullptr;

	auto sprite = Sprite::createWithSpriteFrameName(mapSprite[type]);
	if (sprite == nullptr)
		return nullptr;

	node->setContentSize(sprite->getContentSize());
	sprite->setPosition(node->getContentSize() * 0.5f);
	sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	node->addChild(sprite);
	node->sprite = sprite;


	switch (type)
	{
	case CristalNode::CristalType::RED:
		expirence *= 10;
		break;
	case CristalNode::CristalType::YELLOW:
		expirence *= 4;
		break;
	case CristalNode::CristalType::GREEN:
		expirence *= 2;
		break;
	case CristalNode::CristalType::BLUE:
		break;
	default:
		break;
	}

	node->expirence = expirence;
	node->setScale(0.4f);

	return node;
}

const CristalNode::SpriteMap & CristalNode::getSpriteMap()
{
	return mapSprite;
}

const CristalNode::ShapeMap & CristalNode::getShapeMap()
{
	return mapShape;
}

void CristalNode::destroy()
{
	destroyAnimation();
}

void CristalNode::destroyAnimation()
{

}

void CristalNode::applyImpulse(const Vec2 v)
{
	auto body = sprite->getPhysicsBody();
	body->applyImpulse(v);
	body->applyTorque(v.x * 30.f);
}
