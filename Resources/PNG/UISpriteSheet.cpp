// ---------------------------------------
// Sprite definitions for UISpriteSheet
// Generated with TexturePacker 4.11.0
//
// https://www.codeandweb.com/texturepacker
// ---------------------------------------

#include "UISpriteSheet.hpp"

USING_NS_CC;

namespace TexturePacker
{

void UISpriteSheet::addSpriteFramesToCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("UISpriteSheet.plist");
};

void UISpriteSheet::removeSpriteFramesFromCache()
{
    SpriteFrameCache *cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("UISpriteSheet.plist");
};


// ---------------------
// sprite name constants
// ---------------------
const std::string UISpriteSheet::blue_sliderDown       = "blue_sliderDown.png";
const std::string UISpriteSheet::blue_sliderLeft       = "blue_sliderLeft.png";
const std::string UISpriteSheet::blue_sliderRight      = "blue_sliderRight.png";
const std::string UISpriteSheet::blue_sliderUp         = "blue_sliderUp.png";
const std::string UISpriteSheet::green_sliderDown      = "green_sliderDown.png";
const std::string UISpriteSheet::green_sliderLeft      = "green_sliderLeft.png";
const std::string UISpriteSheet::green_sliderRight     = "green_sliderRight.png";
const std::string UISpriteSheet::green_sliderUp        = "green_sliderUp.png";
const std::string UISpriteSheet::grey_sliderDown       = "grey_sliderDown.png";
const std::string UISpriteSheet::grey_sliderEnd        = "grey_sliderEnd.png";
const std::string UISpriteSheet::grey_sliderHorizontal = "grey_sliderHorizontal.png";
const std::string UISpriteSheet::grey_sliderLeft       = "grey_sliderLeft.png";
const std::string UISpriteSheet::grey_sliderRight      = "grey_sliderRight.png";
const std::string UISpriteSheet::grey_sliderUp         = "grey_sliderUp.png";
const std::string UISpriteSheet::grey_sliderVertical   = "grey_sliderVertical.png";

// ---------------------------------------------------------
// convenience functions returing pointers to Sprite objects
// ---------------------------------------------------------
Sprite* UISpriteSheet::createBlue_sliderDownSprite()
{
    return Sprite::createWithSpriteFrameName(blue_sliderDown);
}

Sprite* UISpriteSheet::createBlue_sliderLeftSprite()
{
    return Sprite::createWithSpriteFrameName(blue_sliderLeft);
}

Sprite* UISpriteSheet::createBlue_sliderRightSprite()
{
    return Sprite::createWithSpriteFrameName(blue_sliderRight);
}

Sprite* UISpriteSheet::createBlue_sliderUpSprite()
{
    return Sprite::createWithSpriteFrameName(blue_sliderUp);
}

Sprite* UISpriteSheet::createGreen_sliderDownSprite()
{
    return Sprite::createWithSpriteFrameName(green_sliderDown);
}

Sprite* UISpriteSheet::createGreen_sliderLeftSprite()
{
    return Sprite::createWithSpriteFrameName(green_sliderLeft);
}

Sprite* UISpriteSheet::createGreen_sliderRightSprite()
{
    return Sprite::createWithSpriteFrameName(green_sliderRight);
}

Sprite* UISpriteSheet::createGreen_sliderUpSprite()
{
    return Sprite::createWithSpriteFrameName(green_sliderUp);
}

Sprite* UISpriteSheet::createGrey_sliderDownSprite()
{
    return Sprite::createWithSpriteFrameName(grey_sliderDown);
}

Sprite* UISpriteSheet::createGrey_sliderEndSprite()
{
    return Sprite::createWithSpriteFrameName(grey_sliderEnd);
}

Sprite* UISpriteSheet::createGrey_sliderHorizontalSprite()
{
    return Sprite::createWithSpriteFrameName(grey_sliderHorizontal);
}

Sprite* UISpriteSheet::createGrey_sliderLeftSprite()
{
    return Sprite::createWithSpriteFrameName(grey_sliderLeft);
}

Sprite* UISpriteSheet::createGrey_sliderRightSprite()
{
    return Sprite::createWithSpriteFrameName(grey_sliderRight);
}

Sprite* UISpriteSheet::createGrey_sliderUpSprite()
{
    return Sprite::createWithSpriteFrameName(grey_sliderUp);
}

Sprite* UISpriteSheet::createGrey_sliderVerticalSprite()
{
    return Sprite::createWithSpriteFrameName(grey_sliderVertical);
}


}; // namespace

