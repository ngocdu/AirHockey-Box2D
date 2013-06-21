//
//  Menu.cpp
//  AirHockey
//
//  Created by macbook_016 on 2013/06/20.
//
//

#include "Menu.h"
#include "HelloWorldScene.h"
using namespace cocos2d;
CCScene* Menu::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    Menu *layer = Menu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
bool Menu::init()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    //create startMenuItem
    CCMenuItemImage *startMenuItem = CCMenuItemImage::create(
                                                          "start.png",
                                                          "start.png",
                                                          this,
                                                          menu_selector(Menu::menuStartgame));
    startMenuItem->setPosition(ccp((size.width / 2) - 20, size.height - 348));

    //create rankMenuItem
    CCMenuItemImage *rankMenuItem = CCMenuItemImage::create(
                                                        "rank.png",
                                                        "rank.png",
                                                        this,
                                                        menu_selector(Menu::menuRanking));
    rankMenuItem->setPosition(ccp((size.width / 2) - 20, size.height - 487));

    //create BMGMenuItem
    CCMenuItemImage *bmgMenuItem = CCMenuItemImage::create(
                                                        "bgm.png",
                                                        "bgm.png",
                                                        this,
                                                        menu_selector(Menu::menuBGM));
    bmgMenuItem->setPosition(ccp((size.width / 2) - 20, size.height - 635));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(startMenuItem, rankMenuItem, bmgMenuItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    CCSprite* pSprite = CCSprite::create("menu.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(size.width / 2, size.height / 2));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    return true;
}
void Menu::menuBGM(CCObject* pSender)
{
}
void Menu::menuRanking(CCObject* pSender)
{
}
void Menu::menuStartgame(CCObject* pSender)
{
    CCScene *helloScene = HelloWorld::scene();
    CCScene *pScene = CCTransitionFadeTR::create(2, helloScene);
    CCDirector::sharedDirector()->replaceScene(pScene);
}


