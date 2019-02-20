#include "ScreenFormat.h"
#include "Defines.h"

USING_NS_CC;

const Size ScreenFormat::LDPI = Size(200, 320);
const Size ScreenFormat::MDPI = Size(320, 480);
const Size ScreenFormat::HDPI = Size(480, 800);
const Size ScreenFormat::XHDPI = Size(720, 1280);
const Size ScreenFormat::XXHDPI = Size(960, 1600);
const Size ScreenFormat::XXXHDPI = Size(1440, 2560);



ScreenFormat::ScreenFormat()
{
	init();
}

bool ScreenFormat::init()
{
	auto director = Director::getInstance();
	if (director == nullptr)
		return false;

	visibleSize = director->getVisibleSize();
	origin = director->getVisibleOrigin();
	scale = director->getContentScaleFactor();
	desingRes = director->getOpenGLView()->getDesignResolutionSize();

	screenRect = { {0,0}, desingRes };


	float intervalSize = screenRect.size.width / COLL_GRID;
	float gameZoneSizeHeight = intervalSize * ROW_GRID; //std::ceil(screenRect.size.height * GAMEZONE_SCALE_FROM_VSIZE / intervalSize);
	Point posGameZone = { screenRect.getMidX(), screenRect.getMidY() };
	Size gameZoneSize = { screenRect.size.width, gameZoneSizeHeight };
	gameZoneRect = { posGameZone - (gameZoneSize * 0.5f), gameZoneSize };

	auto freeHeight = screenRect.size.height - gameZoneRect.size.height;

	headerRect = { 
		Point(screenRect.origin.x, gameZoneRect.getMaxY()),
		Size(screenRect.size.width, freeHeight * 0.5f)
	};

	bottomRect = {
		Point(screenRect.origin.x, screenRect.origin.y),
		Size(screenRect.size.width, freeHeight * 0.5f)
	};



	return true;
}

const cocos2d::Rect & ScreenFormat::getScreenRect()
{
	return inst()->screenRect;
}

const cocos2d::Point & ScreenFormat::getOrigin()
{
	return inst()->origin;
}

const cocos2d::Rect & ScreenFormat::getHeaderRect()
{
	return inst()->headerRect;
}

const cocos2d::Rect & ScreenFormat::getGameZoneRect()
{
	return inst()->gameZoneRect;
}

const cocos2d::Rect & ScreenFormat::getBottomRect()
{
	return inst()->bottomRect;
}

const cocos2d::Size & ScreenFormat::desingResolution()
{
	return inst()->desingRes;
}

const ScreenFormat::Format ScreenFormat::getFormat()
{
	return inst()->format;
}

void ScreenFormat::setFormat(Format format)
{
	inst()->format = format;
}

const float ScreenFormat::getScale()
{
	return inst()->scale;
}

bool ScreenFormat::reInit()
{
	return inst()->init();
}

cocos2d::Node* ScreenFormat::createRoot()
{
	auto rootNode = Node::create();
	rootNode->setContentSize(inst()->screenRect.size);
	rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	rootNode->setPosition(Vec2(inst()->visibleSize * 0.5f) + inst()->origin);

	float scale_x = inst()->visibleSize.width / inst()->screenRect.size.width;
	float scale_y = inst()->visibleSize.height / inst()->screenRect.size.height;
	rootNode->setScale(scale_x, scale_y);

	return rootNode;
}
