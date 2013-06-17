//
//  HelloWorldScene.cpp
//  AirHockey
//
//  Created by Trung Kien Do on 13/06/12.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "MyContactListener.h"


using namespace cocos2d;
using namespace CocosDenshion;

#define PTM_RATIO 32

enum {
    kTagParentNode = 1,
};

PhysicsSprite::PhysicsSprite()
: m_pBody(NULL)
{

}

void PhysicsSprite::setPhysicsBody(b2Body * body)
{
    m_pBody = body;
}

// this method will only get called if the sprite is batched.
// return YES if the physics values (angles, position ) changed
// If you return NO, then nodeToParentTransform won't be called.
bool PhysicsSprite::isDirty(void)
{
    return true;
}

// returns the transform matrix according the Chipmunk Body values
CCAffineTransform PhysicsSprite::nodeToParentTransform(void)
{
    b2Vec2 pos  = m_pBody->GetPosition();

    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;

    if ( isIgnoreAnchorPointForPosition() ) {
        x += m_obAnchorPointInPoints.x;
        y += m_obAnchorPointInPoints.y;
    }

    // Make matrix
    float radians = m_pBody->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);

    if( ! m_obAnchorPointInPoints.equals(CCPointZero) ){
        x += c*-m_obAnchorPointInPoints.x + -s*-m_obAnchorPointInPoints.y;
        y += s*-m_obAnchorPointInPoints.x + c*-m_obAnchorPointInPoints.y;
    }

    // Rot, Translate Matrix
    m_sTransform = CCAffineTransformMake( c,  s,
        -s,    c,
        x,    y );

    return m_sTransform;
}

HelloWorld::HelloWorld()
{
    setTouchEnabled( true );
    setAccelerometerEnabled( true );
    _mouseJoint = NULL;
    // init physics
    this->initPhysics();

    CCSpriteBatchNode *parent = CCSpriteBatchNode::create("blocks.png", 100);
    m_pSpriteTexture = parent->getTexture();

    addChild(parent, 0, kTagParentNode);


    _player1Score = 0;
    _player2Score = 0;
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *court = CCSprite::create("background2.png");
    court->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
    this->addChild(court);
    
    _player1 = CCSprite::create("player1_2.png");
    _player1->setTag(99);
    _player1->setPosition(ccp(_screenSize.width * 0.5,
                              _player1->getContentSize().width));
    this->addChild(_player1, 1);
    b2BodyDef player1BodyDef;
    player1BodyDef.type = b2_dynamicBody;
    player1BodyDef.position.Set(_screenSize.width * 0.5 / PTM_RATIO,
                                _player1->getContentSize().width / PTM_RATIO );
    player1BodyDef.userData = _player1;
    player1BodyDef.linearDamping = 0.0f;
    player1BodyDef.angularDamping = 0.0f;
    _player1Body = world->CreateBody(&player1BodyDef);
    b2FixtureDef player1FixtureDef;
    b2CircleShape circle1;
    float m_radius1 = _player1->getContentSize().height/2;
        CCLOG("radius2: %f", m_radius1);
    circle1.m_radius = m_radius1/PTM_RATIO;
    player1FixtureDef.shape = &circle1;
    player1FixtureDef.density = 10.0f;
    player1FixtureDef.friction = 1.0f;
    player1FixtureDef.restitution = 2.0f;
    _player1Fixture = _player1Body->CreateFixture(&player1FixtureDef);
    
    
    _player2 = CCSprite::create("player2_2.png");
    _player2->setPosition(ccp(_screenSize.width * 0.5,
                              _screenSize.height - _player1->getContentSize().width));
    _player2->setTag(99);
    this->addChild(_player2, 1);
    b2BodyDef player2BodyDef;
    player2BodyDef.type = b2_dynamicBody;
    player2BodyDef.position.Set(_screenSize.width * 0.5 / PTM_RATIO,
                                (_screenSize.height -
                                 _player2->getContentSize().width) / PTM_RATIO);
    player2BodyDef.userData = _player2;
    _player2Body = world->CreateBody(&player2BodyDef);
    b2FixtureDef player2FixtureDef;
    b2CircleShape circle2;
    float m_radius2 = _player2->getContentSize().height/2;
    CCLOG("radius2: %f", m_radius2);
    circle2.m_radius = m_radius2/PTM_RATIO;
    player2FixtureDef.shape = &circle2;
    player2FixtureDef.density = 10.0f;
    player2FixtureDef.friction = 1.0f;
    player2FixtureDef.restitution = 2.0f;
    _player2Fixture = _player2Body->CreateFixture(&player2FixtureDef);
    
    
    _ball = CCSprite::create("ball_2.png");
    _ball->setPosition(ccp(_screenSize.width * 0.5,
                           _screenSize.height * 0.5 -_ball->getContentSize().width/2));
    this->addChild(_ball, 1, 10);
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(_screenSize.width/2/PTM_RATIO,
                             _screenSize.height/2/PTM_RATIO);
    ballBodyDef.userData = _ball;
    _ballBody = world->CreateBody(&ballBodyDef);
    b2FixtureDef ballFixtureDef;
    b2CircleShape circle3;
    float m_radius3 = _ball->getContentSize().height/2;
    circle3.m_radius = m_radius3/PTM_RATIO;
    ballFixtureDef.shape = &circle3;
    ballFixtureDef.density = 1.0f;
    ballFixtureDef.friction = 20.0f;
    ballFixtureDef.restitution = 0.5f;
    ballFixtureDef.filter.groupIndex = -10;
    _ballFixture = _ballBody->CreateFixture(&ballFixtureDef);
    
    _players = CCArray::create(_player1, _player2, NULL);
    _players->retain();
    
    
    CCSpriteBatchNode *counter = CCSpriteBatchNode::create("counter.pvr.ccz");
    this->addChild(counter);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("counter.plist");
    
    _player1ScoreLabel1 = CCSprite::createWithSpriteFrameName("0.png");
    _player1ScoreLabel1->setScale(0.7);
    _player1ScoreLabel1->setPosition(ccp(_screenSize.width - 115,
                                        _screenSize.height * 0.5 - 130));
    _player1ScoreLabel1->setRotation(90);
    this->addChild(_player1ScoreLabel1);
    _player1ScoreLabel2 = CCSprite::createWithSpriteFrameName("0.png");
    _player1ScoreLabel2->setScale(0.7);
    _player1ScoreLabel2->setPosition(ccp(_screenSize.width - 115,
                                         _screenSize.height * 0.5 - 100));
    _player1ScoreLabel2->setRotation(90);
    this->addChild(_player1ScoreLabel2);
    
    _player2ScoreLabel1 = CCSprite::createWithSpriteFrameName("0.png");
    _player2ScoreLabel1->setScale(0.7);
    _player2ScoreLabel1->setPosition(ccp(_screenSize.width - 115,
                                        _screenSize.height * 0.5 + 100));
    _player2ScoreLabel1->setRotation(90);
    this->addChild(_player2ScoreLabel1);
    _player2ScoreLabel2 = CCSprite::createWithSpriteFrameName("0.png");
    _player2ScoreLabel2->setScale(0.7);
    _player2ScoreLabel2->setPosition(ccp(_screenSize.width - 115,
                                         _screenSize.height * 0.5 + 130));
    _player2ScoreLabel2->setRotation(90);
    this->addChild(_player2ScoreLabel2);

    
    scheduleUpdate();
}

HelloWorld::~HelloWorld()
{
    delete world;
    world = NULL;
    
    //delete m_debugDraw;
}

void HelloWorld::initPhysics()
{

    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    world = new b2World(gravity);

    // Do we want to let bodies sleep?
    world->SetAllowSleeping(true);

    world->SetContinuousPhysics(true);

//     m_debugDraw = new GLESDebugDraw( PTM_RATIO );
//     world->SetDebugDraw(m_debugDraw);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    //        flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    //m_debugDraw->SetFlags(flags);


    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    _groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2EdgeShape groundBox;

    // bottom_left
    groundBox.Set(b2Vec2(0, 70/PTM_RATIO),
                  b2Vec2(260/PTM_RATIO, 70/PTM_RATIO));
    _groundBody->CreateFixture(&groundBox, 0);
    
    // bottom_right
    groundBox.Set(b2Vec2((s.width-260)/PTM_RATIO, 70/PTM_RATIO),
                  b2Vec2(s.width/PTM_RATIO, 70/PTM_RATIO));
    _groundBody->CreateFixture(&groundBox, 0);
    
    //bottom_middle
    groundBox.Set(b2Vec2(260/PTM_RATIO, 70/PTM_RATIO),
                  b2Vec2((s.width-260)/PTM_RATIO, 70/PTM_RATIO));
    b2FixtureDef bottomMiddleDef;
    bottomMiddleDef.shape = &groundBox;
    bottomMiddleDef.filter.groupIndex = -10;
    _groundBody->CreateFixture(&bottomMiddleDef);

    // bottom_middle1
    groundBox.Set(b2Vec2(260/PTM_RATIO, 70/PTM_RATIO),
                  b2Vec2(260/PTM_RATIO, 0));
    _groundBody->CreateFixture(&groundBox, 0);
    
    // bottom_middle2
    groundBox.Set(b2Vec2((s.width-260)/PTM_RATIO, 70/PTM_RATIO),
                  b2Vec2((s.width-260)/PTM_RATIO, 0));
    _groundBody->CreateFixture(&groundBox, 0);
    
    // top_left
    groundBox.Set(b2Vec2(0,(s.height-70)/PTM_RATIO),
                  b2Vec2(260/PTM_RATIO,(s.height-70)/PTM_RATIO));
    _groundBody->CreateFixture(&groundBox, 0);
    
    // top_right
    groundBox.Set(b2Vec2((s.width-260)/PTM_RATIO,(s.height-70)/PTM_RATIO),
                  b2Vec2(s.width/PTM_RATIO,(s.height-70)/PTM_RATIO));
    _groundBody->CreateFixture(&groundBox, 0);
    
    //top_middle
    groundBox.Set(b2Vec2(260/PTM_RATIO, (s.height-70)/PTM_RATIO),
                  b2Vec2((s.width-260)/PTM_RATIO, (s.height-70)/PTM_RATIO));
    b2FixtureDef topMiddleDef;
    topMiddleDef.shape = &groundBox;
    topMiddleDef.filter.groupIndex = -10;
    _groundBody->CreateFixture(&topMiddleDef);
    
    // top_middle1
    groundBox.Set(b2Vec2(260/PTM_RATIO,(s.height-70)/PTM_RATIO),
                  b2Vec2(260/PTM_RATIO, s.height/PTM_RATIO));
    _groundBody->CreateFixture(&groundBox, 0);
    
    // top_middle2
    groundBox.Set(b2Vec2((s.width-260)/PTM_RATIO,(s.height-70)/PTM_RATIO),
                  b2Vec2((s.width-260)/PTM_RATIO, s.height/PTM_RATIO));
    _groundBody->CreateFixture(&groundBox, 0);
    
    // left
    groundBox.Set(b2Vec2(55/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(55/PTM_RATIO,0));
    _groundBody->CreateFixture(&groundBox, 0);

    // right
    groundBox.Set(b2Vec2((s.width-45)/PTM_RATIO,s.height/PTM_RATIO),
                  b2Vec2((s.width-45)/PTM_RATIO,0));
    _groundBody->CreateFixture(&groundBox, 0);
    
    // middle
    groundBox.Set(b2Vec2(0, s.height/2/PTM_RATIO),
                  b2Vec2(s.width/PTM_RATIO, s.height/2/PTM_RATIO));
    b2FixtureDef MiddleDef;
    MiddleDef.shape = &groundBox;
    MiddleDef.filter.groupIndex = -10;
    _groundBody->CreateFixture(&MiddleDef);
    

}

void HelloWorld::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    world->DrawDebugData();

    kmGLPopMatrix();
}


void HelloWorld::update(float dt)
{
    
    int velocityIterations = 8;
    int positionIterations = 3;
    
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    world->Step(dt, velocityIterations, positionIterations);
    
    //Iterate over the bodies in the physics world
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO,
                                              b->GetPosition().y * PTM_RATIO) );
            if (myActor->getTag() == 99) {
                b->SetLinearVelocity(b2Vec2(0, 0));
                b->SetFixedRotation(true);
            }
            
        }
    }
    
//    std::vector<MyContact>::iterator pos;
//    for (pos = _contactListener->_contacts.begin();
//         pos != _contactListener->_contacts.end(); ++pos) {
//        MyContact contact = *pos;
//    }
    
    
    
    if (_ball->getPositionY() >= s.height-55) {
        HelloWorld::playerScore(1);
        HelloWorld::gameReset();
    }
    
    if (_ball->getPositionY() <= 55) {
        HelloWorld::playerScore(2);
        HelloWorld::gameReset();
    }
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event){
    CCSetIterator i;
    
    for (i = touches->begin(); i != touches->end(); i++) {
        if (_mouseJoint != NULL) return;
        CCTouch *touch = (CCTouch*)(*i);
        CCPoint tap = touch->getLocation();

        b2Vec2 locationWorld = b2Vec2(tap.x / PTM_RATIO, tap.y / PTM_RATIO);
        
        if (_player1Fixture->TestPoint(locationWorld)) {
            b2MouseJointDef md;
            md.bodyA = _groundBody;
            md.bodyB = _player1Body;
            md.target = locationWorld;
            md.collideConnected = true;
            md.maxForce = 100000.0f * _player1Body->GetMass();
            md.dampingRatio = 0;
            md.frequencyHz = 1000;
            
            _mouseJoint = (b2MouseJoint *)world->CreateJoint(&md);
            _player1Body->SetAwake(true);
        }
        if (_player2Fixture->TestPoint(locationWorld)) {
            b2MouseJointDef md;
            md.bodyA = _groundBody;
            md.bodyB = _player2Body;
            md.target = locationWorld;
            md.collideConnected = true;
            md.maxForce = 100000.0f * _player2Body->GetMass();
            md.dampingRatio = 0;
            md.frequencyHz = 1000;
            
            _mouseJoint = (b2MouseJoint *)world->CreateJoint(&md);
            _player2Body->SetAwake(true);
        }
    }
}

void HelloWorld::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event) {
    CCSetIterator i;
    
    for (i = touches->begin(); i != touches->end(); i++) {
        if (_mouseJoint == NULL) return;
        
        CCTouch  *myTouch = (CCTouch *)(*i);
        CCPoint location = myTouch->getLocationInView();
        location = CCDirector::sharedDirector()->convertToGL(location);
        b2Vec2 locationWorld = b2Vec2(location.x / PTM_RATIO, location.y / PTM_RATIO);
        
        _mouseJoint->SetTarget(locationWorld);
    }
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event) {
    if (_mouseJoint) {
        world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }
}

void HelloWorld::playerScore(int player) {
    SimpleAudioEngine::sharedEngine()->playEffect("score.wav");
    char scoreBuff[10];
    
    if (player == 1) {
        _player1Score++;
        _sc1Tens = _player1Score % 10;
        _sc1SingleDigit = _player1Score / 10;
        sprintf(scoreBuff, "%d.png", _sc1Tens);
        CCSprite *tens = CCSprite::createWithSpriteFrameName(scoreBuff);
        _player1ScoreLabel1->setTextureRect(tens->getTextureRect());
        _player1ScoreLabel1->setTexture(tens->getTexture());
        sprintf(scoreBuff, "%d.png", _sc1SingleDigit);
        CCSprite *singleDigit = CCSprite::createWithSpriteFrameName(scoreBuff);
        _player1ScoreLabel2->setTextureRect(singleDigit->getTextureRect());
        _player1ScoreLabel2->setTexture(singleDigit->getTexture());
        
        
        _ballBody->SetLinearVelocity(b2Vec2(0, 0));
        _ballBody->SetTransform(b2Vec2(_screenSize.width/2/PTM_RATIO,
                                       (_screenSize.height/2 +
                                        _ball->getContentSize().width/2)/PTM_RATIO), 0);
    }
    if (player == 2) {
        _player2Score++;
        _sc2Tens = _player2Score % 10;
        _sc2SingleDigit = _player2Score / 10;
        sprintf(scoreBuff, "%d.png", _sc2Tens);
        CCSprite *tens = CCSprite::createWithSpriteFrameName(scoreBuff);
        _player2ScoreLabel1->setTextureRect(tens->getTextureRect());
        _player2ScoreLabel1->setTexture(tens->getTexture());
        sprintf(scoreBuff, "%d.png", _sc2SingleDigit);
        CCSprite *singleDigit = CCSprite::createWithSpriteFrameName(scoreBuff);
        _player2ScoreLabel2->setTextureRect(singleDigit->getTextureRect());
        _player2ScoreLabel2->setTexture(singleDigit->getTexture());
        
        _ballBody->SetLinearVelocity(b2Vec2(0, 0));
        _ballBody->SetTransform(b2Vec2(_screenSize.width/2/PTM_RATIO,
                                       (_screenSize.height/2 -
                                        _ball->getContentSize().width/2)/PTM_RATIO), 0);
    }
    
}

void HelloWorld::gameReset(){
    _player1Body->SetLinearVelocity(b2Vec2(0, 0));
    _player1Body->SetTransform(b2Vec2(_screenSize.width/2/PTM_RATIO,
                                      _player1->getContentSize().width/PTM_RATIO), 0);
    
    _player2Body->SetLinearVelocity(b2Vec2(0, 0));
    _player2Body->SetTransform(b2Vec2(_screenSize.width/2/PTM_RATIO,
                                      (_screenSize.height -
                                       _player2->getContentSize().width)/PTM_RATIO), 0);
    
    if (_mouseJoint) {
        world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new HelloWorld();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}
