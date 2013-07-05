//
//  RankingScene.h
//  AirHockey
//
//  Created by macbook_016 on 2013/07/01.
//
//

#ifndef __AirHockey__RankingScene__
#define __AirHockey__RankingScene__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
class RankingScene : public CCLayer
{
private:
public:
    virtual bool init();
    static CCScene* scene();
    void onHttpRequestCompleted(CCNode *sender, void *data);
    CREATE_FUNC(RankingScene);
    
    void menuPlay(CCObject* pSender);
    void menuMenu(CCObject* pSender);
};
#endif /* defined(__AirHockey__RankingScene__) */
