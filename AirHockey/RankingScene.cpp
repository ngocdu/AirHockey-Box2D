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
    CCLabelTTF *ranking = CCLabelTTF::create("RANKING", "Time new roman", 60);
    ranking->setPosition(ccp(size.width / 2, 9 * size.height / 10));
    this->addChild(ranking);

    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://192.168.1.77:3000/users.json");
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankingScene::onHttpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
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
        CCLabelTTF *notConnectLabel =
        CCLabelTTF::create("PLEASE CHECK YOUR INTERNET CONNECTION", "Time new roman",
                           30);
        notConnectLabel->setPosition(ccp(size.width / 2, size.height / 2));
        this->addChild(notConnectLabel);
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    char * data2 = (char*)(malloc(buffer->size() *  sizeof(char)));
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
            nameLabel->setPosition(ccp(size.width / 6,
                                       size.height * (12 - dem) / 15));
            this->addChild(nameLabel);
            char strP [20] = {0};
            sprintf(strP,"%i", document[i]["point"].GetInt());
            CCLabelTTF *pointLabel = CCLabelTTF::create(strP,
                                                            "Time New Roman", 50);
            CCLOG("%f",pointLabel->getContentSize().width);
            pointLabel->setAnchorPoint(ccp(1, 0));
            pointLabel->setPosition(ccp(4 * size.width / 5,
                                        size.height * (12 - dem) / 15));
            this->addChild(pointLabel);
            dem++;
            }
        }
        else
        {
            CCLog(document.GetParseError());
        }
        //free([]data2);
        d = -1;
        delete []data2;
}



