// ---------------------------------------
// Sprite definitions for UISpriteSheet
// Generated with TexturePacker 4.11.0
//
// https://www.codeandweb.com/texturepacker
// 
// $TexturePacker:SmartUpdate:05dd6be776fe9fd46166035eb99d5d8b:546d51f840190a0298214c6f4d19b385:73cc9f018a7bb85df3a704a9affa18a7$
// ---------------------------------------

#ifndef __UISPRITESHEET_H__
#define __UISPRITESHEET_H__

#include "cocos2d.h"

namespace TexturePacker
{

class UISpriteSheet
{
public:
    /**
     * Add sprite frames contained in theUISpriteSheet sheet to
     * the SpriteFrameCache.
     */
    static void addSpriteFramesToCache();

    /**
     * Remove sprite frames contained in theUISpriteSheet sheet from
     * the SpriteFrameCache.
     */
    static void removeSpriteFramesFromCache();

    // ---------------------
    // sprite name constants
    // ---------------------
    static const std::string blue_sliderDown;
    static const std::string blue_sliderLeft;
    static const std::string blue_sliderRight;
    static const std::string blue_sliderUp;
    static const std::string green_sliderDown;
    static const std::string green_sliderLeft;
    static const std::string green_sliderRight;
    static const std::string green_sliderUp;
    static const std::string grey_sliderDown;
    static const std::string grey_sliderEnd;
    static const std::string grey_sliderHorizontal;
    static const std::string grey_sliderLeft;
    static const std::string grey_sliderRight;
    static const std::string grey_sliderUp;
    static const std::string grey_sliderVertical;

    // --------------
    // Sprite objects
    // --------------
    static cocos2d::Sprite *createBlue_sliderDownSprite();
    static cocos2d::Sprite *createBlue_sliderLeftSprite();
    static cocos2d::Sprite *createBlue_sliderRightSprite();
    static cocos2d::Sprite *createBlue_sliderUpSprite();
    static cocos2d::Sprite *createGreen_sliderDownSprite();
    static cocos2d::Sprite *createGreen_sliderLeftSprite();
    static cocos2d::Sprite *createGreen_sliderRightSprite();
    static cocos2d::Sprite *createGreen_sliderUpSprite();
    static cocos2d::Sprite *createGrey_sliderDownSprite();
    static cocos2d::Sprite *createGrey_sliderEndSprite();
    static cocos2d::Sprite *createGrey_sliderHorizontalSprite();
    static cocos2d::Sprite *createGrey_sliderLeftSprite();
    static cocos2d::Sprite *createGrey_sliderRightSprite();
    static cocos2d::Sprite *createGrey_sliderUpSprite();
    static cocos2d::Sprite *createGrey_sliderVerticalSprite();

}; // class

}; // namespace

#endif // __UISPRITESHEET_H__
