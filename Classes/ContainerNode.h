#pragma once

#include <cocos2d.h>
#include <unordered_map>
#include "ObjectNode.h"

class ContainerNode : public ObjectNode
{
public:
	enum class SizeContainer
	{
		SIZE_1_1,
		SIZE_2_1,
		SIZE_1_2,
		SIZE_2_2
	};

	enum class SpriteID
	{
		BOX_1 = 0,
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
		RED_SMALL = BOX_7,
		COLOR_MASK_SMALL,
		COLOR_MASK_SMALL2,
		COLOR_MASK_MEDIUM,
		COLOR_MASK_BIG

	};

	enum class COLOR
	{
		RED = 0,
		GREEN,
		GRAY,
		BLACK,
		YELLOW,
		BLUE,
		WHITE,

		COUNT
	};

	enum class RotationContainer
	{
		VERTICAL = 0,
		HORIZONTAL = 1
	};

	using MapSprite = std::unordered_map<SpriteID, std::string>;
	using MapShape = std::unordered_map<SpriteID, std::string>;
	using MapColor = std::unordered_map<COLOR, cocos2d::Color3B>;


	bool init() override;

	static ContainerNode* create(
		int experience_,
		int hp_,
		SpriteID spriteID_ = SpriteID::BOX_1,
		RotationContainer rotation_ = RotationContainer::HORIZONTAL,
		COLOR color_ = COLOR::BLUE
	);

	static const MapSprite& getPullSpriteName();
	static const MapShape& getPullShapeName();
	static const MapColor& getMapColor();

	void damage(int dmg);
	void damageAnimation(const cocos2d::Vec2 &vec);
	
	static void destroy(ContainerNode *obj);
	
	inline int getHP() { return hp; }
	inline int getExp() { return experience; }
	
	inline RotationContainer getRotationContainer() { return rotation; }
	inline SpriteID getSpriteID() { return spriteID; }
	inline COLOR getColorID() { return color; }

private:
	CREATE_FUNC(ContainerNode);

	static MapSprite pullSpriteName;
	static MapShape pullShapeName;
	static MapColor mapColor;

	int hp = 1;
	int experience = 1;

	SizeContainer sizeContainder;
	RotationContainer rotation;
	SpriteID spriteID;
	COLOR color;
	cocos2d::Sprite *sprite;



};