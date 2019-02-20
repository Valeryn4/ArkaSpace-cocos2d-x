#pragma once

#include <cocos2d.h>
#include <unordered_map>


/*
	TODO переделать под спрайты
*/
class CristalNode : public cocos2d::Node
{
public:
	enum class CristalType
	{
		RED,
		YELLOW,
		GREEN,
		BLUE
	};

	using SpriteMap = std::unordered_map<CristalType, std::string>;
	using ShapeMap = std::unordered_map<CristalType, std::string>;

	bool init() override;
	bool initPhysics();

	static CristalNode* create(CristalType type, int expirence);

	static const SpriteMap& getSpriteMap();
	static const ShapeMap& getShapeMap();

	inline CristalType getCristalType() { return type; }
	inline int getExpirence() { return expirence; }

	void destroy();
	void destroyAnimation();

	void applyImpulse(const cocos2d::Vec2 v);

private:
	CREATE_FUNC(CristalNode);

	static SpriteMap mapSprite;
	static ShapeMap mapShape;

	int expirence;
	CristalType type;

	cocos2d::Sprite *sprite;


};