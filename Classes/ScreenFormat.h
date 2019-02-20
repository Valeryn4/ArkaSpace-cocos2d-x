#pragma once

#include <cocos2d.h>

class ScreenFormat
{
public:
	ScreenFormat(const ScreenFormat &) = delete;
	ScreenFormat(ScreenFormat &&move) = delete;
	ScreenFormat& operator=(const ScreenFormat &) = delete;
	ScreenFormat& operator=(ScreenFormat &&move) = delete;

	
	static const cocos2d::Size LDPI;// = cocos2d::Size(200, 320);
	static const cocos2d::Size MDPI;// = cocos2d::Size(320, 480);
	static const cocos2d::Size HDPI;// = cocos2d::Size(480, 800);
	static const cocos2d::Size XHDPI;// = cocos2d::Size(720, 1280);
	static const cocos2d::Size XXHDPI;// = cocos2d::Size(960, 1600);
	static const cocos2d::Size XXXHDPI;// = cocos2d::Size(1440, 2560);
	
	enum class Format
	{
		F_16_9 = 0,
		F_16_10,
		F_5_3,
		NORMAL = F_16_9
	};

	
	static const cocos2d::Rect& getScreenRect();
	static const cocos2d::Point& getOrigin();
	static const cocos2d::Rect& getHeaderRect();
	static const cocos2d::Rect& getGameZoneRect();
	static const cocos2d::Rect& getBottomRect();
	static const cocos2d::Size& desingResolution();
	static const Format getFormat();
	static void setFormat(Format format);
	static const float getScale();

	static bool reInit();

	static cocos2d::Node* createRoot();
private:
	ScreenFormat();
	bool init();
	
	static ScreenFormat* inst()
	{
		static ScreenFormat instance;
		return &instance;
	}


	cocos2d::Rect screenRect = {};
	cocos2d::Rect headerRect = {};
	cocos2d::Rect gameZoneRect = {};
	cocos2d::Rect bottomRect = {};
	cocos2d::Point origin = {};
	cocos2d::Size desingRes = {};
	cocos2d::Size visibleSize = {};
	ScreenFormat::Format format = Format::F_16_9;
	float scale;

};