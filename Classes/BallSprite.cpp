#include "BallSprite.h"
#include "PhysicsShapeCache.h"

USING_NS_CC;

int BallSprite::g_id = 0;

std::string BallSprite::spriteName = "small_ball.png";

bool BallSprite::init()
{


	id = g_id++;
	this->initWithSpriteFrameName(spriteName);
	//sprite = Sprite::createWithSpriteFrameName(spriteName);
	//this->setContentSize(sprite->getContentSize());

	auto shapeCache = PhysicsShapeCache::getInstance();

	auto body = PhysicsBody::createCircle(this->getContentSize().width * 0.5 * 0.5, DEFAULT_MAERIAL);
	this->setPhysicsBody(body);

	body->setCollisionBitmask(collisionMaskDefault);
	body->setCategoryBitmask(BALL_COLLISION_MASK);
	body->setContactTestBitmask(ALL_COLLISION ^ BALL_COLLISION_MASK ^ LEVEL_COLLISION_BITMASK ^ CRISTAL_COLLISION_MASK);
	body->setDynamic(false);
	body->setGravityEnable(false);


	this->setName(BALL_NAME);
	this->setScale(0.5);
	status = Status::Stopped;

	this->scheduleUpdate();

	return true;
}

void BallSprite::push(Vec2 vec)
{
	vec.normalize();
	auto body = this->getPhysicsBody();
	body->setDynamic(true);
	//body->applyImpulse(vec);
	body->setVelocity(vec * 1000 * Director::getInstance()->getContentScaleFactor() * speed);
	status = Status::Flying;

}

void BallSprite::grab(const Point &pos)
{
	auto body = this->getPhysicsBody();

	body->setDynamic(false);
	body->resetForces();
	this->setPosition(pos);
	status = Status::Stopped;
}

void BallSprite::disableCollision()
{
	auto body = this->getPhysicsBody();
	body->setCollisionBitmask(collisionMaskNotBrick);
}

void BallSprite::enableCollision()
{
	auto body = this->getPhysicsBody();
	body->setCollisionBitmask(collisionMaskDefault);
}

void BallSprite::fireEffectEnable()
{
}

void BallSprite::fireEffectDisable()
{
}

bool BallSprite::fireEffectIsEnabled()
{
	return false;
}

void BallSprite::update(float dt)
{
	
	//effect->setPosition(this->getPosition());
	if (status == Status::Flying)
	{
		auto body = this->getPhysicsBody();
		auto vec = body->getVelocity();
		vec.normalize();

		//TODO - наш шар можно пульнуть под любым углом, а обьекты у нас разных форм
		// но что делать, если шар сткнулся так удачно, что завис в горизонтальном положении?
		// короче тупой костыль. Во время игры это не заметно абсолютно.
		if (vec.y <= 0.05f && vec.y  >= -0.05f)
		{
			CCLOG("BALL  HORIZONTAL BAG");

			if (vec.y >= 0.f)
				vec.y = 0.1f;
			else
				vec.y = -0.1f;
		}
		
		body->setVelocity(vec * 1000 * Director::getInstance()->getContentScaleFactor() * speed);
	}
	
}

BallSprite * BallSprite::createWithScene(cocos2d::Scene * scene)
{
	if (scene == nullptr)
		return nullptr;
	auto ball = BallSprite::create();
	if (ball == nullptr)
		return nullptr;

	scene->addChild(ball);
	return ball;
}

void BallSprite::initParticles()
{

	auto emitter = ParticleFlower::create();
	emitter->setPositionType(ParticleSystem::PositionType::FREE);
	//emitter->setEmitterMode(ParticleSystem::Mode::RADIUS);
	effect = emitter;

	this->getParent()->addChild(effect);	
}
