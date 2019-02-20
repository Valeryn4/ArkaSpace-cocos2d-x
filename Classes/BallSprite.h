#pragma once

#include <cocos2d.h>
#include <list>

#include "Defines.h"

class GameScene;

/*
	TODO - переделать на спрайт
*/
class BallSprite : public cocos2d::Sprite
{
public:
	enum class Status
	{
		Flying,
		Stopped
	};

	bool init() override;
	void push(cocos2d::Vec2 vec);
	void grab(const cocos2d::Point &pos);
	void disableCollision();
	void enableCollision();

	void fireEffectEnable();
	void fireEffectDisable();
	bool fireEffectIsEnabled();

	void update(float dt) override;


	inline Status getStatus() const { return status; }
	int getId() const { return id; }

	CREATE_FUNC(BallSprite);

	static BallSprite* createWithScene(cocos2d::Scene* scene);
private:

	float speed = 1.5;
	int id;
	static int g_id;
	Status status = Status::Stopped;

	cocos2d::ParticleSystem* effect = nullptr;
	static std::string spriteName;

	static constexpr int collisionMaskDefault = 
		ALL_COLLISION ^ BALL_COLLISION_MASK ^ BONUS_COLLISION_MASK ^ CRISTAL_COLLISION_MASK;
	static constexpr int collisionMaskNotBrick = 
		ALL_COLLISION ^ BALL_COLLISION_MASK ^ BONUS_COLLISION_MASK ^ BRICK_COLLISION_MASK ^ CRISTAL_COLLISION_MASK;

	void initParticles();

	bool m_fireEffectEnable;

};

