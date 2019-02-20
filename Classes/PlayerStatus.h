#pragma once

#include <cocos2d.h>

class PlayerStatus
{
public:
	PlayerStatus();
	PlayerStatus(float dmg_, float hp_, int ball_);
	PlayerStatus(const PlayerStatus &copy);
	PlayerStatus(PlayerStatus &&move);
	PlayerStatus(const cocos2d::ValueMap &map);
	~PlayerStatus() {}

	void addBall(int ball_);
	void delBall(int ball_ = 1);
	inline int getBall() { return ball; }

	void addHP(float hp_);
	void delHP(float hp_ = -1.f);
	void setHP(float hp_ = 100.f);
	inline float getHP() { return hp; }

	void addDamage(int dmg_ = 1);
	void delDamage(int dmg_ = 1);
	void setDamage(int dmg_ = 1);
	inline int getDamage() { return damage; }

	static cocos2d::ValueMap toValueVectorFromPlayerStatus(const PlayerStatus &status);
	static PlayerStatus toPlayerStatusFromValueVector(const cocos2d::ValueMap& vec);

	PlayerStatus& operator=(const PlayerStatus &copy);
private:
	float damage = 1;
	float hp = 100.f;
	int ball = 0;
};