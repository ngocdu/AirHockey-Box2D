//
//  Menu.cpp
//  AirHockey
//
//  Created by macbook_016 on 2013/06/20.
//
//
#include "HttpClient.h"
#include "Menu.h"
<<<<<<< HEAD
#include "PlayerName.h"
#include "GameManager.h"
=======
#include "GamePlay.h"
#include "document.h"
>>>>>>> ai-player
#include "RankingScene.h"
using namespace cocos2d;
using namespace cocos2d::extension;
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
    startMenuItem = CCMenuItemImage::create(
                                            "start.png",
                                            "start.png",
                                            this,
                                            menu_selector(Menu::menuStartgame));
    startMenuItem->setPosition(ccp(size.width / 2.10989011, size.height / 1.5147929));

    //create rankMenuItem
    rankMenuItem = CCMenuItemImage::create(
                                           "rank.png",
                                           "rank.png",
                                           this,
                                           menu_selector(Menu::menuRanking));
    rankMenuItem->setPosition(ccp(size.width / 2.10989011, size.height / 1.90689013));

    //create BMGMenuItem
    bmgMenuItem = CCMenuItemImage::create(
                                          "bgm.png",
                                          "bgm.png",
                                          this,
                                          menu_selector(Menu::menuBGM));
    bmgMenuItem->setPosition(ccp(size.width / 2.10989011, size.height / 2.63239075));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(startMenuItem, rankMenuItem, bmgMenuItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
    CCSprite* pSprite = CCSprite::create("menu.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(size.width / 2, size.height / 2));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    GameManager::sharedGameManager()->setBgm(true);
    return true;
}
void Menu::menuBGM(CCObject* pSender)
{
    GameManager *game = GameManager::sharedGameManager();
    if (game->getBgm()) {
        bmgMenuItem->setColor(ccc3(32, 32, 64));
        game->setBgm(false);
    } else {
        bmgMenuItem->setColor(ccc3(255, 255, 255));
        game->setBgm(true);
    }

}
void Menu::menuRanking(CCObject* pSender)
{
<<<<<<< HEAD
    CCScene *rankScene = CCScene::create();
    RankingScene *rankLayer = RankingScene::create();
    rankScene->addChild(rankLayer);
    CCScene *pScene = CCTransitionFadeTR::create(2, rankScene);
    CCDirector::sharedDirector()->replaceScene(pScene);
=======
    CCDirector::sharedDirector()->replaceScene(RankingScene::scene());
>>>>>>> ai-player
}
void Menu::menuStartgame(CCObject* pSender)
{
    CCScene *playerNameScene = PlayerName::scene();
    CCScene *pScene = CCTransitionFadeTR::create(2, playerNameScene);
    CCDirector::sharedDirector()->replaceScene(pScene);
}
