//
//  Menu.h
//  AirHockey
//
//  Created by macbook_016 on 2013/06/20.
//
//

#ifndef __AirHockey__Menu__
#define __AirHockey__Menu__

#include <iostream>
#include "cocos2d.h"
class Menu : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuBGM(CCObject* pSender);
    void menuRanking(CCObject* pSender);
    void menuStartgame(CCObject* pSender);
    CREATE_FUNC(Menu);
};

#endif /* defined(__AirHockey__Menu__) */
