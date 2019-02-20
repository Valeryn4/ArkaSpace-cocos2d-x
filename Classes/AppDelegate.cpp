/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "PreviewScene.h"
#include "PhysicsShapeCache.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;




/*

LDPI: Portrait: 200 X 320px
MDPI: Portrait: 320 X 480px
HDPI: Portrait: 480 X 800px
XHDPI: Portrait: 720 X 1280px
XXHDPI: Portrait: 960 X 1600px
XXXHDPI: Portrait: 1440 x 2560px

*/

namespace ResDPI {
	static cocos2d::Size LDPI = cocos2d::Size(200, 320);
	static cocos2d::Size MDPI = cocos2d::Size(320, 480);
	static cocos2d::Size HDPI = cocos2d::Size(480, 800);
	static cocos2d::Size XHDPI = cocos2d::Size(720, 1280);
	static cocos2d::Size XXHDPI = cocos2d::Size(960, 1600);
	static cocos2d::Size XXXHDPI = cocos2d::Size(1440, 2560);
}
static cocos2d::Size designResolutionSize = ResDPI::XHDPI;
static cocos2d::Size winDevResolitionSize = ResDPI::HDPI;

/* 
48 × 48 (mdpi)
72 × 72 (hdpi)
96 × 96 (xhdpi)
144 × 144 (xxhdpi)
192 × 192 (xxxhdpi)

512 × 512 (Google Play store)

x0.5, x0.75,  x1, x1.5, x2

*/

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}



bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("ArkaSpace", cocos2d::Rect(0, 0, winDevResolitionSize.width, winDevResolitionSize.height));
#else
        glview = GLViewImpl::create("ArkaSpace");
#endif
        director->setOpenGLView(glview);
    }


#ifdef _DEBUG
	director->setDisplayStats(true);
#else
	director->setDisplayStats(false);
#endif
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);


    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
	using namespace ResDPI;
    if (frameSize.height >= XXXHDPI.height)
    {        
        director->setContentScaleFactor(MIN(XXXHDPI.height/designResolutionSize.height, XXXHDPI.width/designResolutionSize.width));
		FileUtils::getInstance()->addSearchPath("Large");
	}
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height >= XHDPI.height)
    {        
        director->setContentScaleFactor(MIN(XHDPI.height/designResolutionSize.height, XHDPI.width/designResolutionSize.width));
		FileUtils::getInstance()->addSearchPath("Medium");
	}
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(MDPI.height/designResolutionSize.height, MDPI.width/designResolutionSize.width));
		//FileUtils::getInstance()->addSearchPath("Small");
		FileUtils::getInstance()->addSearchPath("Medium");
	}

	PhysicsShapeCache::getInstance()->addShapesWithFile("shapesheets.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("spritesheets.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UISpacePackSpriteSheet.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UISpriteSheet.plist");

	FileUtils::getInstance()->addSearchPath("fonts");
    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = PreviewScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
