#include "PlayerStatus.h"

USING_NS_CC;



PlayerStatus::PlayerStatus()
{
}

PlayerStatus::PlayerStatus(float dmg_, float hp_, int ball_) :
	damage(dmg_),
	hp(hp_),
	ball(ball_)
{
}

PlayerStatus::PlayerStatus(const PlayerStatus & copy) :
	damage(copy.damage),
	hp(copy.hp),
	ball(copy.ball)
{
}

PlayerStatus::PlayerStatus(PlayerStatus && move) :
	damage(move.damage),
	hp(move.hp),
	ball(move.ball)
{
}

PlayerStatus::PlayerStatus(const cocos2d::ValueMap & map)
{
	CCASSERT(map.size() == 3, "toPlayerStatusFromValueVector size failed!");
	CCASSERT(
		map.at("damage").getType() == Value::Type::FLOAT
		&& map.at("hp").getType() == Value::Type::FLOAT
		&& map.at("ball").getType() == Value::Type::INTEGER,
		"ValueVector failed type");


	damage = map.at("damage").asFloat();
	hp = map.at("hp").asFloat();
	ball = map.at("ball").asInt();
}

void PlayerStatus::addBall(int ball_)
{
	ball += ball_;
}

void PlayerStatus::delBall(int ball_)
{
	ball -= ball_;
}

void PlayerStatus::addHP(float hp_)
{
	hp += hp_;
	if (hp > 100)
		hp = 100;
}

void PlayerStatus::delHP(float hp_)
{
	hp -= hp_;
}

void PlayerStatus::setHP(float hp_)
{
	hp = hp_;
	if (hp > 100)
		hp = 100.f;
}

void PlayerStatus::addDamage(int dmg_)
{
	damage += dmg_;
}

void PlayerStatus::delDamage(int dmg_)
{
	damage -= dmg_;
	if (damage < 1)
		damage = 1;
}

void PlayerStatus::setDamage(int dmg_)
{
	damage = dmg_;
}

PlayerStatus PlayerStatus::toPlayerStatusFromValueVector(const ValueMap & map)
{
	CCASSERT(map.size() == 3, "toPlayerStatusFromValueVector size failed!");
	CCASSERT(
		map.at("damage").getType() == Value::Type::FLOAT 
		&& map.at("hp").getType() == Value::Type::FLOAT
		&& map.at("ball").getType() == Value::Type::INTEGER,
		"ValueVector failed type");

	return PlayerStatus(map.at("damage").asFloat(), map.at("hp").asFloat(), map.at("ball").asInt());
}

PlayerStatus & PlayerStatus::operator=(const PlayerStatus & copy)
{
	this->damage = copy.damage;
	this->hp = copy.hp;
	this->ball = copy.ball;
	return *this;
}

cocos2d::ValueMap PlayerStatus::toValueVectorFromPlayerStatus(const PlayerStatus &status)
{
	return cocos2d::ValueMap({ 
		{ "damage", Value(status.damage)}, 
		{ "hp", Value(status.hp)}, 
		{ "ball", Value(status.ball)} 
		});
}
