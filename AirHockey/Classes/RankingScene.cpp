//
//  RankingScene.cpp
//  AirHockey
//
//  Created by macbook_016 on 2013/07/01.
//
//

#include "RankingScene.h"
#include "cocos2d.h"
#include "rapidjson.h"
#include "document.h"
#include "Menu.h"
#include "GamePlay.h"
using namespace cocos2d;
USING_NS_CC;
USING_NS_CC_EXT;

CCScene* RankingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    RankingScene *layer1 = RankingScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer1);
    
    // return the scene
    return scene;
}
bool RankingScene::init()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://localhost:3000/users.json");
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankingScene::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    
    
    //create startMenuItem
    CCMenuItemImage *startMenuItem = CCMenuItemImage::create(
                                                             "btn_Continue.png",
                                                             "btn_Continue.png",
                                                             this,
                                                             menu_selector(RankingScene::menuPlay));
    startMenuItem->setScale(4);
    startMenuItem->setPosition(ccp(size.width/4, size.height/8));
    
    //create rankMenuItem
    CCMenuItemImage *rankMenuItem = CCMenuItemImage::create(
                                                            "btn_menu.png",
                                                            "btn_menu.png",
                                                            this,
                                                            menu_selector(RankingScene::menuMenu));
    rankMenuItem->setScale(4);
    rankMenuItem->setPosition(ccp(size.width * 3/4, size.height/8));
    CCMenu* pMenu = CCMenu::create(startMenuItem, rankMenuItem, NULL);
    pMenu->setPosition(ccp(0,0));
    this->addChild(pMenu);
    
    return true;
}
void RankingScene::onHttpRequestCompleted(CCNode *sender, void *data)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCHttpResponse *response = (CCHttpResponse*)data;
    
    if (!response)
    {
        return;
    }
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
    //   m_labelStatusCode->setString(statusString);
    CCLog("response code: %d", statusCode);
    
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        return;
    }
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        char * data2 = (char*)(malloc(buffer->size() * sizeof(char)));
//       char  data2[buffer->size()+10];
        int d = -1;
        printf("Http Test, dump data: ");
        for (unsigned int i = 0; i < buffer->size(); i++)
        {
            d++ ;
            data2[d] = (*buffer)[i];
        }
        data2[d + 1] = '\0';
        //-----------------------
        int dem = 0;
        rapidjson::Document document;
        if(data2 != NULL && !document.Parse<0>(data2).HasParseError())
        {
            
            for (rapidjson::SizeType  i = 0; i < document.Size(); i++)
            {
                CCLabelTTF *nameLabel = CCLabelTTF::create(document[i]["name"].GetString(),
                                                           "Time New Roman", 50);
                nameLabel->setAnchorPoint(ccp(0, 0)) ;
                nameLabel->setPosition(ccp(size.width / 6 ,size.height - (dem + 1) * 60));
                this->addChild(nameLabel);
                char strP [20] = {0};
                sprintf(strP,"%i", document[i]["point"].GetInt());
                CCLabelTTF *pointLabel = CCLabelTTF::create(strP,
                                                            "Time New Roman", 50);
                pointLabel->setAnchorPoint(ccp(0, 0));
                pointLabel->setPosition(ccp(size.width/2 ,size.height - (dem + 1) * 60));
                this->addChild(pointLabel);
                dem++;
            }
        }
        else
        {
            CCLog(document.GetParseError());
        }
        free(data2);
}
void RankingScene::menuMenu(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(Menu::scene());
}
void RankingScene::menuPlay(CCObject* pSender)
{
    CCScene *GamePlayScene = GamePlay::scene();
    CCScene *pScene = CCTransitionFadeTR::create(2, GamePlayScene);
    CCDirector::sharedDirector()->replaceScene(pScene);
}