//
//  HelloWorldScene.h
//  AirHockey
//
//  Created by Trung Kien Do on 13/06/12.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"
#include "MyContactListener.h"

USING_NS_CC;

class PhysicsSprite : public cocos2d::CCSprite
{
public:
    PhysicsSprite();
    void setPhysicsBody(b2Body * body);
    virtual bool isDirty(void);
    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
private:
    b2Body* m_pBody;    // strong ref
};

class HelloWorld : public cocos2d::CCLayer {
    CCSize _screenSize;
public:
    ~HelloWorld();
    HelloWorld();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    void initPhysics();
    // adds a new sprite at a given coordinate
    void addNewSpriteAtPosition(cocos2d::CCPoint p);

    virtual void draw();
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
    void update(float dt);
    
private:
    b2Body* _groundBody;
    b2Body *_player1Body;
    b2Body *_player2Body;
    b2Body *_ballBody;
    
    b2Fixture *_player1Fixture;
    b2Fixture *_player2Fixture;
    b2Fixture *_ballFixture;
    
    b2Fixture *lowerMiddle;
    b2Fixture *higherMiddle;
    
    CCSprite *_player1;
    CCSprite *_player2;
    CCSprite *_ball;
    
    CCLabelTTF *_player1ScoreLabel;
    CCLabelTTF *_player2ScoreLabel;
    
    int _player1Score;
    int _player2Score;
    
    CCArray *_players;
    
    b2World* world;
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
    
    b2MouseJoint *_mouseJoint;
    MyContactListener *_contactListener;
};

#endif // __HELLO_WORLD_H__
