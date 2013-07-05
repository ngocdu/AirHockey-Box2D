//
//  AirHockeyAppDelegate.cpp
//  AirHockey
//
//  Created by Trung Kien Do on 13/06/12.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GamePlay.h"
#include "Menu.h"
#include "AppMacros.h"
USING_NS_CC;
using namespace CocosDenshion;
typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
}Resource;

static Resource smallResource  =  { cocos2d::CCSizeMake(320, 480),   "sd" };
static Resource mediumResource =  { cocos2d::CCSizeMake(768, 1024),  "sd"   };
static Resource largeResource  =  { cocos2d::CCSizeMake(1536, 2048), "hd" };
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(768, 1024);
AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
    
	CCSize frameSize = pEGLView->getFrameSize();
    //CCSize designSize = CCSizeMake(768, 1024);
    std::vector<std::string> searchPath;
    
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
    
    if (frameSize.height > mediumResource.size.height)
    {
        searchPath.push_back(largeResource.directory);
        pDirector->setContentScaleFactor(largeResource.size.height  / designResolutionSize.height);
        CCLog("resoucre:  %s ",largeResource.directory);
    }
    //if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        searchPath.push_back(mediumResource.directory);
        pDirector->setContentScaleFactor((mediumResource.size.height * 2.0f) / designResolutionSize.height);
        CCLog("resoucre:  %s ",mediumResource.directory);
    }
    else
    {
        searchPath.push_back(smallResource.directory);
        pDirector->setContentScaleFactor((smallResource.size.height *4.4f) / designResolutionSize.height);
    }
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    //-------------------------------------------------------    
    // create a scene. it's an autorelease object
    CCScene *pScene = Menu::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
