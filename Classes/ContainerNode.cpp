#include "ContainerNode.h"
#include "PhysicsShapeCache.h"
#include "Defines.h"
#include "CristalNode.h"

USING_NS_CC;

/*
		BOX_1,
		BLUE_SMALL = BOX_1,
		BOX_2,
		RED_MEDIUM = BOX_2,
		BOX_3,
		BLUE_MEDIUM = BOX_3,
		BOX_4,
		RED_BIG = BOX_4,
		BOX_5,
		BLUE_BIG = BOX_5,
		BOX_6,
		BLUE_MK2_SMALL = BOX_6,
		BOX_7,
		RED_SMALL = BOX_7
*/
using SprID = ContainerNode::SpriteID;
ContainerNode::MapSprite ContainerNode::pullSpriteName = 
{
	{SprID::BLUE_SMALL,		"spaceBuilding_001.png"},
	{SprID::RED_MEDIUM,		"spaceBuilding_002.png"},
	{SprID::BLUE_MEDIUM,	"spaceBuilding_003.png"},
	{SprID::RED_BIG,		"spaceBuilding_004.png"},
	{SprID::BLUE_BIG,		"spaceBuilding_005.png"},
	{SprID::BLUE_MK2_SMALL,	"spaceBuilding_006.png"},
	{SprID::RED_SMALL,		"spaceBuilding_007.png"},
	{SprID::COLOR_MASK_SMALL,  "spaceBuilding_collor_001.png"},
	{SprID::COLOR_MASK_SMALL2, "spaceBuilding_collor_002.png"},
	{SprID::COLOR_MASK_MEDIUM, "spaceBuilding_collor_003.png"},
	{SprID::COLOR_MASK_BIG,    "spaceBuilding_collor_004.png"}
};

ContainerNode::MapShape ContainerNode::pullShapeName =
{
	{SprID::BLUE_SMALL,		"spaceBuilding_001"},
	{SprID::RED_MEDIUM,		"spaceBuilding_002"},
	{SprID::BLUE_MEDIUM,	"spaceBuilding_003"},
	{SprID::RED_BIG,		"spaceBuilding_004"},
	{SprID::BLUE_BIG,		"spaceBuilding_005"},
	{SprID::BLUE_MK2_SMALL,	"spaceBuilding_006"},
	{SprID::RED_SMALL,		"spaceBuilding_007"}
};

using ColorID = ContainerNode::COLOR;
ContainerNode::MapColor ContainerNode::mapColor = 
{
	{ColorID::BLACK,	{50 ,50 ,50 } },
	{ColorID::BLUE,		{100,100,255} },
	{ColorID::GRAY,		{125,125,125} },
	{ColorID::GREEN,	{100,255,100} },
	{ColorID::RED,		{255,100,100} },
	{ColorID::WHITE,	{255,255,255} },
	{ColorID::YELLOW,	{255,255,100} }
};

bool ContainerNode::init()
{
	this->setName(CONTAINER_NAME);
	this->type_id = CONTAINER_NAME;
	return ObjectNode::init();
}

ContainerNode * ContainerNode::create(int experience_, int hp_, SpriteID spriteID_, RotationContainer rotation_, COLOR color_)
{
	auto node = create();
	if (node == nullptr)
		return nullptr;

	auto sprite = Sprite::createWithSpriteFrameName(pullSpriteName[spriteID_]);
	if (sprite == nullptr)
		return nullptr;

	auto shapeCache = PhysicsShapeCache::getInstance();
	auto res = shapeCache->setBodyOnSprite(pullShapeName[spriteID_], sprite, DEFAULT_MAERIAL);
	if (!res)
		return nullptr;

	node->spriteID = spriteID_;

	node->rotation = rotation_;
	node->experience = experience_;
	node->hp = hp_;
	node->sprite = sprite;
	node->color = color_;

	auto body = sprite->getPhysicsBody();
	body->setCollisionBitmask(BALL_COLLISION_MASK);
	body->setCategoryBitmask(BRICK_COLLISION_MASK);
	body->setContactTestBitmask(BALL_COLLISION_MASK);
	body->setDynamic(false);

	auto contentSize = sprite->getContentSize();

	node->addChild(sprite);

	switch (spriteID_)
	{
	case ContainerNode::SpriteID::BLUE_MK2_SMALL:
	{
		auto spriteInner = Sprite::createWithSpriteFrameName(pullSpriteName[SprID::COLOR_MASK_SMALL2]);
		spriteInner->setColor(mapColor.at(color_));
		spriteInner->setPosition(sprite->getContentSize() / 2.f);

		sprite->addChild(spriteInner);
		node->setSizeFromGrid(1, 1);
		node->sizeContainder = SizeContainer::SIZE_1_1;
		break;
	}
	case ContainerNode::SpriteID::BLUE_SMALL:
	case ContainerNode::SpriteID::RED_SMALL:
	{
		auto spriteInner = Sprite::createWithSpriteFrameName(pullSpriteName[SprID::COLOR_MASK_SMALL]);
		spriteInner->setColor(mapColor.at(color_));
		spriteInner->setPosition(sprite->getContentSize() / 2.f);
		sprite->addChild(spriteInner);
		node->setSizeFromGrid(1, 1);
		node->sizeContainder = SizeContainer::SIZE_1_1;
		break;
	}
	case ContainerNode::SpriteID::RED_MEDIUM:
	case ContainerNode::SpriteID::BLUE_MEDIUM:
	{
		auto spriteInner = Sprite::createWithSpriteFrameName(pullSpriteName[SprID::COLOR_MASK_MEDIUM]);
		spriteInner->setColor(mapColor.at(color_));
		spriteInner->setPosition(sprite->getContentSize() / 2.f);
		sprite->addChild(spriteInner);

		if (rotation_ == RotationContainer::HORIZONTAL)
		{
			node->setSizeFromGrid(2, 1);
			node->sizeContainder = SizeContainer::SIZE_2_1;
			
		}
		else
		{
			node->setSizeFromGrid(1, 2);
			node->sizeContainder = SizeContainer::SIZE_1_2;
		}

		//node->hp *= 2;
		break;
	}
	case ContainerNode::SpriteID::RED_BIG:
	case ContainerNode::SpriteID::BLUE_BIG:
	{
		auto spriteInner = Sprite::createWithSpriteFrameName(pullSpriteName[SprID::COLOR_MASK_BIG]);
		spriteInner->setColor(mapColor.at(color_));
		spriteInner->setPosition(sprite->getContentSize() / 2.f);

		sprite->addChild(spriteInner);
		node->setSizeFromGrid(2, 2);
		node->sizeContainder = SizeContainer::SIZE_2_2;
		//node->hp *= 4;
		break;
	}
	default:
		return nullptr;
	}

	if (rotation_ == RotationContainer::HORIZONTAL)
	{
		sprite->setRotation(90.f);
		node->setContentSize(Size(contentSize.height, contentSize.width));
	}
	else
	{
		node->setContentSize(sprite->getContentSize());
	}

	//node->setScale(0.5);

	return node;
}

const ContainerNode::MapSprite & ContainerNode::getPullSpriteName()
{
	return pullSpriteName;
}

const ContainerNode::MapShape & ContainerNode::getPullShapeName()
{
	return pullShapeName;
}

const ContainerNode::MapColor & ContainerNode::getMapColor()
{
	return mapColor;
}

void ContainerNode::damage(int dmg)
{
	hp -= dmg;
}


void ContainerNode::damageAnimation(const Vec2 &vec)
{
	
	auto sec = Sequence::create(MoveBy::create(0.1, vec), MoveTo::create(0.1, fixed_position), nullptr);
	this->runAction(sec);
}

void ContainerNode::destroy(ContainerNode *obj)
{
	ObjectNode::removeFromGridAndCleanup(obj);
}





