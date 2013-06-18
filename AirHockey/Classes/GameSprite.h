//
//  GameSprite.h
//  AirHockey
//
//  Created by Trung Kien Do on 13/06/03.
//
//

#ifndef __AirHockey__GameSprite__
#define __AirHockey__GameSprite__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class GameSprite : public CCSprite {
public:
    CC_SYNTHESIZE(CCPoint, _nextPosition, NextPosition);
    CC_SYNTHESIZE(CCPoint, _vector, Vector);
    CC_SYNTHESIZE(CCTouch *, _touch, Touch);

    GameSprite(void);
    ~GameSprite(void);
  
    static GameSprite* gameSpriteWidthFile(const char * pszFileName);
    virtual void setPosition(const CCPoint& pos);
    float radius();
};

#endif /* defined(__AirHockey__GameSprite__) */
