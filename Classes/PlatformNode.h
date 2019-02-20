#pragma once

#include "cocos2d.h"

class PlatformNode : public cocos2d::Node 
{
public:
	enum class PlatformStatus
	{
		GrabBall,
		NoGrab
	};

	bool init();

	inline PlatformStatus getStatus() { return status; }
	inline void setStatus(PlatformStatus status_) { status = status_; }


	CREATE_FUNC(PlatformNode);
private:
	static std::string spriteName;
	static std::string shapeName;
	PlatformStatus status;
};