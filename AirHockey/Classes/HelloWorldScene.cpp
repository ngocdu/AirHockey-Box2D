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
    
    CCSprite *court = CCSprite::create("court.png");
    court->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
    this->addChild(court);
    
    _player1 = CCSprite::create("mallet.png");
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
    circle1.m_radius = m_radius1/PTM_RATIO;
    player1FixtureDef.shape = &circle1;
    player1FixtureDef.density = 10.0f;
    player1FixtureDef.friction = 10.0f;
    player1FixtureDef.restitution = 0.2f;
    _player1Fixture = _player1Body->CreateFixture(&player1FixtureDef);
    
    
    _player2 = CCSprite::create("mallet.png");
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
    circle2.m_radius = m_radius2/PTM_RATIO;
    player2FixtureDef.shape = &circle2;
    player2FixtureDef.density = 10.0f;
    player2FixtureDef.friction = 10.0f;
    player2FixtureDef.restitution = 0.2f;
    _player2Fixture = _player2Body->CreateFixture(&player2FixtureDef);
    
    
    _ball = CCSprite::create("puck.png");
    _ball->setPosition(ccp(_screenSize.width * 0.5,
                           _screenSize.height * 0.5 -_ball->getContentSize().width/2));
    this->addChild(_ball, 1, 10);
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(_screenSize.width * 0.5 / PTM_RATIO,
                             (_screenSize.height * 0.5 -
                              _ball->getContentSize().width/2 ) / PTM_RATIO);
    ballBodyDef.userData = _ball;
    _ballBody = world->CreateBody(&ballBodyDef);
    b2FixtureDef ballFixtureDef;
    b2CircleShape circle3;
    float m_radius3 = _ball->getContentSize().height/2;
    circle3.m_radius = m_radius3/PTM_RATIO;
    ballFixtureDef.shape = &circle3;
    ballFixtureDef.density = 10.0f;
    ballFixtureDef.friction = 0.7f;
    ballFixtureDef.restitution = 0.8f;
    ballFixtureDef.filter.groupIndex = -10;
    _ballFixture = _ballBody->CreateFixture(&ballFixtureDef);
    
    
    _players = CCArray::create(_player1, _player2, NULL);
    _players->retain();
    
    _player1ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player1ScoreLabel->setPosition(ccp(_screenSize.width - 60,
                                        _screenSize.height * 0.5 - 80));
    _player1ScoreLabel->setRotation(90);
    this->addChild(_player1ScoreLabel);
    
    _player2ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player2ScoreLabel->setPosition(ccp(_screenSize.width - 60,
                                        _screenSize.height * 0.5 + 80));
    _player2ScoreLabel->setRotation(90);
    this->addChild(_player2ScoreLabel);

    
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

    CCSize s = CCDirector::sharedDirector()->getWinSize();

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

    // bottom

    groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
    _groundBody->CreateFixture(&groundBox,0);

    // top
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
    _groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    _groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    _groundBody->CreateFixture(&groundBox,0);
    
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
    int positionIterations = 1;
    
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
            md.maxForce = 1000.0f * _player1Body->GetMass();
            
            _mouseJoint = (b2MouseJoint *)world->CreateJoint(&md);
            _player1Body->SetAwake(true);
        }
        if (_player2Fixture->TestPoint(locationWorld)) {
            b2MouseJointDef md;
            md.bodyA = _groundBody;
            md.bodyB = _player2Body;
            md.target = locationWorld;
            md.collideConnected = true;
            md.maxForce = 1000.0f * _player2Body->GetMass();
            
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
