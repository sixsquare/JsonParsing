//
//  GamePlayLayer.cpp
//  LearnNumber
//  Created by Neil D on 14/06/13.

#include "GamePlayLayer.h"
#include "../Classes/homeLayerLoader.h"
#include "../Classes/Scene_common.h"
#include "../Classes/Extension/CCGestureRecognizer/CCSwipeGestureRecognizer.h"
#include "../Classes/SettingLayerLoader.h"
#include "SimpleAudioEngine.h"
#include "CCArrow.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define PTM_RATIO 32
using namespace CocosDenshion;
#pragma mark -
#pragma mark lifecycle 
#pragma mark -
GamePlayLayer::GamePlayLayer():
_mAnimationManager(NULL),
touch(NULL),
sprite_latter(NULL),
sprite_mask(NULL),
total_segments(0),
current_segment(0),
sprite_arrow(NULL),
_anim_batch_sprite(NULL),
m_nSoundId(0),
current_number(1),
int_play_no(0),
current_play_level(0),
is_completed(false),
lbl_header(false)
{
    is_completed=false;
    points_passed= CCPointArray::create(0);
    points_remain= CCPointArray::create(0);
    points_segment= CCPointArray::create(0);
    //set all animating arrrows out there...
    arr_anim_arrow=CCArray::createWithCapacity(100);
    arr_anim_arrow->retain();
    arr_char_anim=CCArray::createWithCapacity(10);
    arr_char_anim->retain();
}
GamePlayLayer::~GamePlayLayer()
{
    CC_SAFE_RELEASE(_mAnimationManager);
    CC_SAFE_RELEASE(arr_char_anim);
    CC_SAFE_RELEASE(arr_anim_arrow);
}
ccColor3B static getColor(int color)
{
    ccColor3B color_val;
    switch (color)
    {
        case 1:
            color_val=ccc3(42,155,105);
            break;
        case 2:
            color_val=ccc3(210,184,040);
                       break;
        case 3:
            color_val=ccc3(198,100,76);
           
            break;
        case 4:
            color_val=ccc3(39,189,178);
           

            break;
        case 5:
            color_val=ccc3(247,139,123);
                       break;
        case 6:
            color_val=ccc3(213,122,239);
           
            break;
        case 7:
            color_val=ccc3(249,83,92);
           

            break;
        case 8:
            color_val=ccc3(154,162,244);

            break;
        case 9:
            color_val=ccc3(60,184,120);
            break;
        case 10:
            color_val=ccc3(240,130,192);
            break;
            
        default:
            color_val=ccc3(240,130,192);
            break;
    }
    return color_val;
    
}
const char*  GamePlayLayer::getnumberword(int currentpos)
{
    CCString *strnumber;
    switch (currentpos)
    {
        case 1:
            strnumber=CCString::create("ONE");
            break;
        case 2:
            strnumber=CCString::create("TWO");
            break;
        case 3:
            strnumber=CCString::create("THREE");
            break;
        case 4:
            strnumber=CCString::create("FOUR");
            break;
        case 5:
            strnumber=CCString::create("FIVE");
            break;
        case 6:
            strnumber=CCString::create("SIX");
            break;
        case 7:
            strnumber=CCString::create("SEVEN");
            break;
        case 8:
            strnumber=CCString::create("EIGHT");
            break;
        case 9:
            strnumber=CCString::create("NINE");
            break;
        case 10:
            strnumber=CCString::create("TEN");
            break;
        default:
            break;
            
        
            
    }
    return strnumber->getCString();
}
void GamePlayLayer::onEnter()
{
    ObjCCalls::full_screen_banner(true);
    setTouchEnabled(true);
    size= CCDirector::sharedDirector()->getWinSize();
    this->initPhysics();
    scheduleUpdate();
    target_mask = CCRenderTexture::create(size.width, size.height, kCCTexture2DPixelFormat_RGBA8888);
    target_mask->retain();
    target_mask->setPosition(ccp(size.width / 2, size.height / 2));
    this->addChild(target_mask,3,777);
    brush = CCSprite::create("circle_100x100.png");
    brush->setScale(0.99);
    brush->retain();
    //change current number and call loading assets for next char to display...
    current_number=CCUserDefault::sharedUserDefault()->getIntegerForKey("number");
    brush->setColor(getColor(current_number));
    this->loading_assets();
    CCString *str=CCString::createWithFormat("Write Number \"%s\"",this->getnumberword(current_number));
    lbl_header->setString(str->getCString());
    this->reorderChild(lbl_header, 1111);
    CCLog("%f",CCDirector::sharedDirector()->getContentScaleFactor());
    lbl_header->setFontSize(lbl_header->getFontSize()*CCDirector::sharedDirector()->getContentScaleFactor());
    CCLayer::onEnter();
}
void GamePlayLayer::initPhysics()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    world = new b2World(gravity);
    // Do we want to let bodies sleep?
    world->SetAllowSleeping(true);
    world->SetContinuousPhysics(true);
    //m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    //world->SetDebugDraw(m_debugDraw);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;
    //m_debugDraw->SetFlags(flags);
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    
    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    
    // Define the ground box shape.
    b2EdgeShape groundBox;
    
    // bottom
    
    groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
    
    // top
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);
    // left
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);
    // right
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
}
void GamePlayLayer::loading_assets()
{
    //load digit for drawing...
    CCString *str_number=CCString::createWithFormat("%d/%d2.png",current_number,current_number);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CCString::createWithFormat("%d.plist",current_number)->getCString());
    this->sprite_latter->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str_number->getCString()));

    CCString *str_number_mask=CCString::createWithFormat("%d/%d_%d.png",current_number,current_number,current_number);
    this->sprite_mask->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str_number_mask->getCString()));
        this->sprite_latter->setPosition(ccp(size.width/2,size.height/2));
    this->reorderChild(this->sprite_latter, 2);
    this->reorderChild(sprite_arrow, 5);
    str_number=CCString::createWithFormat("anim_%d.png",current_number);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CCString::createWithFormat("anim_%d.plist",current_number)->getCString());
    _anim_batch_sprite= CCSpriteBatchNode::create(str_number->getCString());
    this->addChild(_anim_batch_sprite, 5, 100);
    draw_segment=CCDrawNode::create();
    this->addChild(draw_segment, 3);
    this->play_animation();
    this->play_for_level(++current_play_level);
}
void GamePlayLayer::onExit()
{
    SimpleAudioEngine::sharedEngine()->stopEffect(m_sound_effect);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    CCLayer::onExit();
}
void GamePlayLayer::draw()
{
    if(touch&&is_drag_started)
    {
        //CCPoint location = touch->getLocation();
        //cr_srt_pnt=this->shortest_distance(location, start_pnt,end_pnt);
        //sprite_arrow->setPosition(cr_srt_pnt);
        this->remove_current_arrow();
        //#warning remove drawsegment and add TouchPoint...
        // draw_segment->drawSegment(start_pnt, cr_srt_pnt,42.5,ccc4f(0.1,0.32,0.45,1));
    }
    CCLayer::draw();
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    kmGLPushMatrix();
    world->DrawDebugData();
    kmGLPopMatrix();
    CCLayer::draw();
}
void GamePlayLayer::update(CCTime dt)
{
    if(is_drag_started==true&&touch)
    {
        CCPoint location = touch->getLocation();
        float total_dist=ccpDistance(cr_srt_pnt,end_pnt);
        float space_allowance=ccpDistance(location,cr_srt_pnt);
        if (total_dist==0.0000)
        {
            this->get_next_destination(current_destination);
            ++current_segment;
            if (current_segment==total_segments)
            {
                if (current_number==4&&is_completed==false)
                {
                    is_completed=true;
                    is_drag_started=false;
                    CCSpawn *spwn=CCSpawn::create(CCRepeat::create(CCRotateBy::create(0.2,-200),4), CCScaleBy::create(0.3,0.0),NULL);
                    CCSequence *sq=CCSequence::create(spwn,CCCallFunc::create(this, callfunc_selector(GamePlayLayer::segment_change)),NULL);
                    sprite_arrow->runAction(sq);
                    
                }
                else if (current_number==10&&is_completed==false)
                {
                     is_completed=true;
                    //change arrow positions
                    //after final slot add animations..
                    is_drag_started=false;
                    CCSpawn *spwn=CCSpawn::create(CCRepeat::create(CCRotateBy::create(0.2,-200),4), CCScaleBy::create(0.3,0.0),NULL);
                    CCSequence *sq=CCSequence::create(spwn,CCCallFunc::create(this, callfunc_selector(GamePlayLayer::segment_change)),NULL);
                    sprite_arrow->runAction(sq);

                }
                else
                {
                   this->unschedule(schedule_selector(GamePlayLayer::update));
                   _mAnimationManager->runAnimationsForSequenceNamedTweenDuration("draw_complete_anim", 0.5f);
                   is_drag_started=false;
                   sprite_arrow->runAction(CCSpawn::create(CCRepeat::create(CCRotateBy::create(0.3,-200),1), CCScaleBy::create(0.3,0.0),NULL));
                   draw_segment->clear();
                   //sprite_mask->setVisible(false);
                   sprite_latter->setVisible(true);
                }
            }
        }
        else if (space_allowance<1000)
        {
            //CCLog("continue");
            //cr_srt_pnt=this->shortest_distance(location, start_pnt,end_pnt);
            //sprite_arrow->setPosition(cr_srt_pnt);
            ////#warning remove drawsegment and add TouchPoint...
            //draw_segment->drawSegment(start_pnt, cr_srt_pnt,41.5,ccc4f(0.1,0.32,0.45,1));
            //draw_segment->setPosition(cr_srt_pnt);
        }
        else
        {
            CCLog("arrow bounce effect");
            //this->touch->getLocation();
             CCPoint location1 = touch->getLocation();
              location1 = CCDirector::sharedDirector()->convertToGL(location);
             CCPoint location2 =sprite_arrow->getPosition();
             CCLOG("%f %f",location1.x,location1.y);
            //location_from
            SimpleAudioEngine::sharedEngine()->playEffect("sound/punch.mp3");
            //sprite_arrow->setPosition(ccp(cr_srt_pnt.x, cr_srt_pnt.y));
            sprite_arrow->runAction(CCMoveTo::create(0.3, ccp(cr_srt_pnt.x, cr_srt_pnt.y)));
            //CCMoveTo *mv=CCMoveTo::create(0.4,location1);
            //sprite_arrow->runAction(CCSequence::create(mv,,NULL));
            //CCEaseElasticInOut *act=CCEaseElasticInOut::create(CCScaleTo::create(0.8,2.2), 2.0);
            //CCEaseElasticInOut *act1=CCEaseElasticInOut::create(CCScaleTo::create(0.8,0.2), 2.0);
            //sprite_arrow->runAction(CCSequence::create(act,act1,NULL));
            //animation for grip removed effect...
            is_drag_started=false;
        }
    }
}
void GamePlayLayer::segment_change()
{
     this->change_segment(current_number);
     _mAnimationManager->runAnimationsForSequenceNamedTweenDuration("animatepath_anim", 0.003f);
    // CCSpawn *spwn=CCSpawn::create(CCRepeat::create(CCRotateBy::create(0.2,200),4), CCScaleBy::create(0.3,1.0),NULL);
    sprite_arrow->setScale(1.0);
}
void GamePlayLayer::play_animation()
{
    int kMax;
    CCPointArray *position=CCPointArray::create(10);
    if (current_number==1)
    {
        kMax=7;
        position->addControlPoint(CCPointMake(679*(size.width/1024), 390*(size.height/768)));
    }
    else if (current_number==2)
    {
        kMax=7;
        position->addControlPoint(CCPointMake(565*(size.width/1024), 380*(size.height/768)));
        position->addControlPoint(CCPointMake(785*(size.width/1024), 380*(size.height/768)));
    }
    else if (current_number==3)
    {
        kMax=4;
        position->addControlPoint(CCPointMake(580*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(580*(size.width/1024), 280*(size.height/768)));
        position->addControlPoint(CCPointMake(780*(size.width/1024), 380*(size.height/768)));
    }
    else if(current_number==4)
    {
        kMax=13;
        position->addControlPoint(CCPointMake(570*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(780*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(570*(size.width/1024), 300*(size.height/768)));
        position->addControlPoint(CCPointMake(780*(size.width/1024), 300*(size.height/768)));
    }
    else if(current_number==5)
    {
        kMax=2;
        position->addControlPoint(CCPointMake(600*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(760*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(530*(size.width/1024), 290*(size.height/768)));
        position->addControlPoint(CCPointMake(670*(size.width/1024), 290*(size.height/768)));
        position->addControlPoint(CCPointMake(815*(size.width/1024), 290*(size.height/768)));
    }
    else if(current_number==6)
    {
        kMax=19;
        position->addControlPoint(CCPointMake(530*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(670*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(815*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(530*(size.width/1024), 290*(size.height/768)));
        position->addControlPoint(CCPointMake(670*(size.width/1024), 290*(size.height/768)));
        position->addControlPoint(CCPointMake(815*(size.width/1024), 290*(size.height/768)));
    }
    else if(current_number==7)
    {
        kMax=10;
        position->addControlPoint(CCPointMake(536.6*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(666.6*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(796.6*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(520*(size.width/1024), 300*(size.height/768)));
        position->addControlPoint(CCPointMake(620*(size.width/1024), 300*(size.height/768)));
        position->addControlPoint(CCPointMake(720*(size.width/1024), 300*(size.height/768)));
        position->addControlPoint(CCPointMake(820*(size.width/1024), 300*(size.height/768)));

    }
    else if(current_number==8)
    {
        kMax=5;
        position->addControlPoint(CCPointMake(520*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(620*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(720*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(820*(size.width/1024), 480*(size.height/768)));
        position->addControlPoint(CCPointMake(520*(size.width/1024), 300*(size.height/768)));
        position->addControlPoint(CCPointMake(620*(size.width/1024), 300*(size.height/768)));
        position->addControlPoint(CCPointMake(720*(size.width/1024), 300*(size.height/768)));
        position->addControlPoint(CCPointMake(820*(size.width/1024), 300*(size.height/768)));
    }
    else if(current_number==9)
    {
        kMax=9;
        position->addControlPoint(CCPointMake(515*(size.width/1024), 510*(size.height/768)));
        position->addControlPoint(CCPointMake(635*(size.width/1024), 510*(size.height/768)));
        position->addControlPoint(CCPointMake(755*(size.width/1024), 510*(size.height/768)));
        position->addControlPoint(CCPointMake(550*(size.width/1024), 380*(size.height/768)));
        position->addControlPoint(CCPointMake(670*(size.width/1024), 380*(size.height/768)));
        position->addControlPoint(CCPointMake(790*(size.width/1024), 380*(size.height/768)));
        position->addControlPoint(CCPointMake(590*(size.width/1024), 255*(size.height/768)));
        position->addControlPoint(CCPointMake(710*(size.width/1024), 255*(size.height/768)));
        position->addControlPoint(CCPointMake(830*(size.width/1024), 255*(size.height/768)));
    }
    else if(current_number==10)
    {
        kMax=5;
        position->addControlPoint(CCPointMake(513.9*(size.width/1024),513.9*(size.height/768)));
        position->addControlPoint(CCPointMake(636.9*(size.width/1024), 513.9*(size.height/768)));
        position->addControlPoint(CCPointMake(759.3*(size.width/1024), 513.9*(size.height/768)));
        position->addControlPoint(CCPointMake(507.7*(size.width/1024), 384*(size.height/768)));
        position->addControlPoint(CCPointMake(620.3*(size.width/1024), 384*(size.height/768)));
        position->addControlPoint(CCPointMake(730.2*(size.width/1024), 384*(size.height/768)));
        position->addControlPoint(CCPointMake(829.8*(size.width/1024), 384*(size.height/768)));
        position->addControlPoint(CCPointMake(588.6*(size.width/1024), 265.2*(size.height/768)));
        position->addControlPoint(CCPointMake(711.5*(size.width/1024), 265.2*(size.height/768)));
        position->addControlPoint(CCPointMake(833.9*(size.width/1024), 265.2*(size.height/768)));
    }
    animFrames = CCArray::createWithCapacity(kMax);
    for(int k = 1; k <=kMax; k++)
    {
        CCString *str_frame=CCString::createWithFormat("anim_%d/%d.png",current_number,k);
        animFrames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str_frame->getCString()));
        str_frame->release();
    }
    for (int i=0; i<current_number;i++)
    {
          CCSprite_animation *sprt_animation=CCSprite_animation::create(CCString::createWithFormat("anim_%d/%d.png",current_number,1)->getCString());
          sprt_animation->setVisible(false);
          char_anim_act=CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(animFrames, 0.3f)));
           sprt_animation->setPosition(position->getControlPointAtIndex(i));
          sprt_animation->runAction(char_anim_act);
          _anim_batch_sprite->addChild(sprt_animation, 1, 2000+i);
          arr_char_anim->addObject(sprt_animation);
     }
}
//reset and hide parameters based on the requirement...
void GamePlayLayer::play_for_level(int level)
{
    sprite_mask->setVisible(false);
    if(level==1)
    {
        this->schedule(schedule_selector(GamePlayLayer::update));
        this->sprite_arrow->setVisible(true);
        this->sprite_arrow->setOpacity(255.0f);
        this->sprite_arrow->setPosition(ccp(size.width/2,size.height/2));
        this->sprite_latter->setVisible(true);
        this->sprite_arrow->setOpacity(255.0f);
        this->sprite_latter->setColor(ccWHITE);
        this->get_char_detail();
    }
    else if(level==2)
    {
        this->schedule(schedule_selector(GamePlayLayer::update));
        total_segments=0;
        current_segment=0;
        m_nSoundId=0;
        int_play_no=0;
        is_drag_started=false;
        this->sprite_arrow->setVisible(true);
        this->sprite_arrow->setOpacity(255.0f);
        this->sprite_arrow->setPosition(ccp(size.width/2,size.height/2));
        this->sprite_latter->setVisible(false);
        this->sprite_latter->setColor(getColor(current_number));
        this->sprite_latter->setPosition(ccp(size.width/2,size.height/2));
        _mAnimationManager->runAnimationsForSequenceNamedTweenDuration("animatepath_anim", 0.003f);
        this->get_char_detail();
        
    }
    else if(level==3)
    {
        this->schedule(schedule_selector(GamePlayLayer::update));
        CCSprite *base=CCSprite::create("number-steps.png");
        this->addChild(base, 9,789);
        base->setPosition(ccp(size.width/2,size.height/2));
        total_segments=0;
        current_segment=0;
        m_nSoundId=0;
        int_play_no=0;
        is_drag_started=false;
        this->sprite_arrow->setVisible(true);
        this->sprite_arrow->setOpacity(255.0f);
        this->sprite_arrow->setPosition(ccp(size.width/2,size.height/2));
        this->sprite_latter->setPosition(ccp(size.width/2,size.height/2));
        this->sprite_latter->setVisible(false);
        this->sprite_latter->setColor(getColor(current_number));
        _mAnimationManager->runAnimationsForSequenceNamedTweenDuration("animatepath_anim", 0.003f);
        this->get_char_detail();
    }
}
#pragma mark -
#pragma mark Charactor Data
#pragma mark -
void GamePlayLayer::get_char_detail()
{
    points_remain=NULL;
    points_segment=NULL;
    points_passed=NULL;
    points_passed=CCPointArray::create(0);
    points_remain=CCPointArray::create(0);
    points_segment=CCPointArray::create(0);
    if ( current_number==1)
    {
        total_segments=10;
        current_segment=0;
        //temp points for one  segment...
        points_segment->addControlPoint(CCPointMake(512.027222*(size.width/1024) ,513.202332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.027222*(size.width/1024) ,490.202332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.027222*(size.width/1024) ,460.202332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.027222*(size.width/1024) ,430.202332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.027222*(size.width/1024) ,400.202332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.027222*(size.width/1024) ,370.202332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.027222*(size.width/1024) ,340.202332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.027222*(size.width/1024) ,310.202332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.027222*(size.width/1024) ,280.202332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.027222*(size.width/1024) ,251.136169*(size.height/768)));
        this->change_segment(current_segment);
    }
    else if ( current_number==2)
    {
        total_segments=48;
        current_segment=0;
        //temp points for one  segment...
        points_segment->addControlPoint(CCPointMake(435.151367*(size.width/1024),449.823944*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(439.042419*(size.width/1024),463.766937*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(446.013947*(size.width/1024),474.791626*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(452.012665*(size.width/1024),489.707397*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(460.119049*(size.width/1024),496.759979*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(470.008881*(size.width/1024),507.703613*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(482.006348*(size.width/1024),511.675751*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(495.949341*(size.width/1024),516.701721*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(506.974030*(size.width/1024),514.675110*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(515.891052*(size.width/1024),515.728943*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(527.888550*(size.width/1024),512.729553*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(538.913208*(size.width/1024),509.730194*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(548.965149*(size.width/1024),503.731476*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(558.854980*(size.width/1024),496.759979*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(567.934143*(size.width/1024),489.707397*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(574.905640*(size.width/1024),478.763763*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(580.904358*(size.width/1024),468.792908*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(580.904358*(size.width/1024),456.795441*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(581.877136*(size.width/1024),445.851807*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(579.931580*(size.width/1024),434.827118*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(576.851196*(size.width/1024),420.884094*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(569.879700*(size.width/1024),410.832184*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(562.908142*(size.width/1024),400.861298*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(556.909424*(size.width/1024),389.917664*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(548.965149*(size.width/1024),380.919556*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(541.993652*(size.width/1024),370.948700*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(534.049377*(size.width/1024),361.950592*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(524.970215*(size.width/1024),348.980377*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(517.998718*(size.width/1024),339.009491*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(510.054474*(size.width/1024),330.011414*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(500.975311*(size.width/1024),320.040527*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(491.085510*(size.width/1024),312.015198*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(483.141235*(size.width/1024),300.098785*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(474.062073*(size.width/1024),292.073456*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(466.117798*(size.width/1024),281.129822*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(455.093109*(size.width/1024),269.132355*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(447.148834*(size.width/1024),262.160858*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(439.204559*(size.width/1024),253.162766*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(456.065857*(size.width/1024),251.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(472.116516*(size.width/1024),252.108932*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(486.059540*(size.width/1024),253.162766*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(500.002533*(size.width/1024),254.135529*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.972778*(size.width/1024),253.162766*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(523.997437*(size.width/1024),255.108292*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(536.967712*(size.width/1024),254.135529*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(551.883484*(size.width/1024),256.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(565.988586*(size.width/1024),255.108292*(size.height/768)));
//        points_segment->addControlPoint(CCPointMake(577.986084*(size.width/1024),257.134888*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(590.956299*(size.width/1024),255.108292*(size.height/768)));
        this->change_segment(current_segment);
    }
    else if (current_number==3)
    {
        total_segments=45;
        current_segment=0;
        points_segment->addControlPoint(CCPointMake(446.013947*(size.width/1024),466.766296*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(451.039886*(size.width/1024),480.790375*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(457.038635*(size.width/1024),492.706787*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(467.090576*(size.width/1024),500.732117*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(477.953125*(size.width/1024),508.676392*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(492.058258*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(506.001282*(size.width/1024),516.701721*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(520.917053*(size.width/1024),516.701721*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(533.887268*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(545.884766*(size.width/1024),506.730835*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(556.909424*(size.width/1024),499.678284*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(563.880920*(size.width/1024),489.707397*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(568.906921*(size.width/1024),479.736542*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(572.960083*(size.width/1024),468.792908*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(573.932861*(size.width/1024),457.768219*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(573.932861*(size.width/1024),444.797974*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(569.879700*(size.width/1024),432.800507*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(564.853699*(size.width/1024),422.829651*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(558.854980*(size.width/1024),414.885376*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(551.883484*(size.width/1024),408.886627*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(541.993652*(size.width/1024),401.915131*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(531.941711*(size.width/1024),396.889160*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(517.025940*(size.width/1024),394.862579*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(530.968994*(size.width/1024),388.944916*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(543.939209*(size.width/1024),385.945526*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(554.963928*(size.width/1024),377.920197*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(563.880920*(size.width/1024),370.948700*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(570.852417*(size.width/1024),361.950592*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(574.905640*(size.width/1024),348.980377*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(578.958862*(size.width/1024),335.037354*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(578.958862*(size.width/1024),321.986053*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(576.851196*(size.width/1024),309.015839*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(574.905640*(size.width/1024),298.072205*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(569.879700*(size.width/1024),286.074738*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.962646*(size.width/1024),277.076630*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(550.910706*(size.width/1024),267.105774*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(538.913208*(size.width/1024),261.107025*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(523.997437*(size.width/1024),257.134888*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(508.108917*(size.width/1024),257.134888*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(490.112732*(size.width/1024),258.107666*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(475.034821*(size.width/1024),261.107025*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(463.037354*(size.width/1024),268.159607*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(455.093109*(size.width/1024),277.076630*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(447.148834*(size.width/1024),286.074738*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(440.177338*(size.width/1024),301.071564*(size.height/768)));
        this->change_segment(current_segment);
    }
    else if (current_number==4)
    {
        is_completed=false;
        total_segments=25;
        current_segment=0;
        points_segment->addControlPoint(CCPointMake(578.796692*(size.width/1024),517.674500*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(569.879700*(size.width/1024),503.731476*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(562.908142*(size.width/1024),490.761230*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(553.991150*(size.width/1024),478.763763*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(544.911987*(size.width/1024),464.739716*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(532.914490*(size.width/1024),449.823944*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(524.970215*(size.width/1024),433.854340*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.972778*(size.width/1024),417.884735*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(503.082977*(size.width/1024),401.915131*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(492.058258*(size.width/1024),381.892334*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(481.033569*(size.width/1024),369.975922*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(473.089294*(size.width/1024),355.951874*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(463.037354*(size.width/1024),345.008240*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(455.093109*(size.width/1024),332.037994*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(446.176056*(size.width/1024),320.040527*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(436.124115*(size.width/1024),307.070312*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(426.234314*(size.width/1024),294.100067*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(437.207092*(size.width/1024),294.100067*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(457.207092*(size.width/1024),294.100067*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(477.207092*(size.width/1024),294.100067*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(497.207092*(size.width/1024),294.100067*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(517.207092*(size.width/1024),294.100067*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(537.207092*(size.width/1024),294.100067*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(557.207092*(size.width/1024),294.100067*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(577.207092*(size.width/1024),294.100067*(size.height/768)));
        //points_segment->addControlPoint(CCPointMake(587.207092*(size.width/1024),304.070923*(size.height/768)));
        //points_segment->addControlPoint(CCPointMake(590.207092*(size.width/1024),304.070923*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(604.899292*(size.width/1024),294.100067*(size.height/768)));
        this->change_segment(current_segment);
    }
    else if (current_number==5)
    {
        total_segments=46;
        current_segment=0;
        points_segment->addControlPoint(CCPointMake(589.821411*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(579.821411*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(569.821411*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(559.821411*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(549.821411*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(539.821411*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(529.821411*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(519.821411*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(509.821411*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(501.948059*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(496.922089*(size.width/1024),497.732727*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(492.058258*(size.width/1024),480.790375*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(486.059540*(size.width/1024),465.793549*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(482.979095*(size.width/1024),451.769470*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(478.115265*(size.width/1024),437.826477*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(473.089294*(size.width/1024),424.856232*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(488.005066*(size.width/1024),425.829010*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(500.975311*(size.width/1024),427.855591*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.972778*(size.width/1024),426.882843*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(523.997437*(size.width/1024),425.829010*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(533.887268*(size.width/1024),421.856873*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(543.939209*(size.width/1024),418.857513*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(553.991150*(size.width/1024),412.858765*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(561.935425*(size.width/1024),404.914490*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(569.879700*(size.width/1024),395.916412*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(577.823914*(size.width/1024),385.945526*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(580.904358*(size.width/1024),371.921478*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(586.903076*(size.width/1024),361.950592*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(588.848633*(size.width/1024),348.980377*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(589.821411*(size.width/1024),336.982910*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(586.903076*(size.width/1024),326.039276*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(584.957581*(size.width/1024),314.041809*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(579.931580*(size.width/1024),302.044342*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(574.905640*(size.width/1024),292.073456*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(566.961365*(size.width/1024),284.129181*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(559.989868*(size.width/1024),277.076630*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(549.937927*(size.width/1024),270.105133*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(537.940491*(size.width/1024),264.106384*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(523.024719*(size.width/1024),259.161499*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(509.081696*(size.width/1024),258.107666*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(494.976562*(size.width/1024),257.134888*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(480.060791*(size.width/1024),258.107666*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(467.090576*(size.width/1024),262.160858*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(458.173523*(size.width/1024),268.159607*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(445.203308*(size.width/1024),275.131104*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(435.151367*(size.width/1024),287.047485*(size.height/768)));

        this->change_segment(current_segment);
    }
    else if (current_number==6)
    {
        total_segments=41;
        current_segment=0;
        points_segment->addControlPoint(CCPointMake(549.937927*(size.width/1024),517.674500*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(539.885986*(size.width/1024),504.704254*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(524.970215*(size.width/1024),484.762512*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.972778*(size.width/1024),469.765686*(size.height/768)));
        
        points_segment->addControlPoint(CCPointMake(504.055725*(size.width/1024),453.796082*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(492.058258*(size.width/1024),436.853699*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(478.115265*(size.width/1024),420.884094*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(467.090576*(size.width/1024),403.860657*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(455.093109*(size.width/1024),388.944916*(size.height/768)));
      
        points_segment->addControlPoint(CCPointMake(453.147552*(size.width/1024),384.891693*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(445.041168*(size.width/1024),370.948700*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(437.096893*(size.width/1024),355.951874*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(435.151367*(size.width/1024),340.955048*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(434.178589*(size.width/1024),326.039276*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(437.096893*(size.width/1024),313.069031*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(440.177338*(size.width/1024),301.071564*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(446.176056*(size.width/1024),290.046875*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(452.174805*(size.width/1024),280.075989*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(460.119049*(size.width/1024),273.104492*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(468.063324*(size.width/1024),266.132996*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(479.088043*(size.width/1024),263.133636*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(488.005066*(size.width/1024),258.107666*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(500.975311*(size.width/1024),255.108292*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.972778*(size.width/1024),256.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(528.050659*(size.width/1024),256.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(536.967712*(size.width/1024),259.161499*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(546.046875*(size.width/1024),264.106384*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(555.936646*(size.width/1024),269.132355*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(565.015808*(size.width/1024),276.103851*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(571.987305*(size.width/1024),284.129181*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(579.931580*(size.width/1024),294.100067*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(582.849915*(size.width/1024),310.069672*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(585.930359*(size.width/1024),324.985443*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(584.957581*(size.width/1024),336.982910*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(583.822693*(size.width/1024),354.006348*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(576.851196*(size.width/1024),368.922089*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(567.934143*(size.width/1024),381.892334*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(554.963928*(size.width/1024),391.944275*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(539.885986*(size.width/1024),399.888550*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(519.944275*(size.width/1024),403.860657*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(506.001282*(size.width/1024),405.887268*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(488.977844*(size.width/1024),404.914490*(size.height/768)));
        this->change_segment(current_segment);
    }
    else if (current_number==7)
    {
        total_segments=29;
        current_segment=0;
        points_segment->addControlPoint(CCPointMake(428.099426*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(448.099426*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(468.099426*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(488.099426*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(508.099426*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(528.099426*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(548.099426*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(568.099426*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(588.099426*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(593.874634*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(582.849915*(size.width/1024),494.733368*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(575.878418*(size.width/1024),480.790375*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(567.934143*(size.width/1024),468.792908*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(559.989868*(size.width/1024),454.768829*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(551.883484*(size.width/1024),442.852448*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(543.939209*(size.width/1024),430.854980*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(535.994934*(size.width/1024),419.830261*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(528.050659*(size.width/1024),405.887268*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(519.944275*(size.width/1024),393.889801*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.000000*(size.width/1024),377.920197*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(504.055725*(size.width/1024),365.922729*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(496.111450*(size.width/1024),352.952515*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(488.005066*(size.width/1024),339.982269*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(481.033569*(size.width/1024),327.984802*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(474.062073*(size.width/1024),315.014557*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(467.090576*(size.width/1024),303.098175*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(459.146301*(size.width/1024),293.046234*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(451.039886*(size.width/1024),281.129822*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(444.068390*(size.width/1024),267.105774*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(433.178589*(size.width/1024),249.190628*(size.height/768)));
        this->change_segment(current_segment);
    }
    else if (current_number==8)
    {
        total_segments=64;
        current_segment=0;
        points_segment->addControlPoint(CCPointMake(511.027222*(size.width/1024),513.702332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(496.922089*(size.width/1024),512.729553*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(482.006348*(size.width/1024),509.730194*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(470.008881*(size.width/1024),499.678284*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(461.091827*(size.width/1024),488.734650*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(454.120331*(size.width/1024),477.791016*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(451.039886*(size.width/1024),464.739716*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(449.094360*(size.width/1024),451.769470*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(449.094360*(size.width/1024),440.825836*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(453.147552*(size.width/1024),429.801147*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(461.091827*(size.width/1024),417.884735*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(470.008881*(size.width/1024),408.886627*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(479.088043*(size.width/1024),400.861298*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(490.112732*(size.width/1024),393.889801*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(501.948059*(size.width/1024),390.890442*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.972778*(size.width/1024),387.891083*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(522.051941*(size.width/1024),386.918304*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(532.914490*(size.width/1024),384.891693*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(543.939209*(size.width/1024),380.919556*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(554.963928*(size.width/1024),374.920837*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(563.880920*(size.width/1024),366.976562*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(568.906921*(size.width/1024),358.951233*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(574.905640*(size.width/1024),351.006958*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(577.986084*(size.width/1024),339.982269*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(580.904358*(size.width/1024),329.038635*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(580.904358*(size.width/1024),318.013947*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(579.931580*(size.width/1024),308.043060*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(576.851196*(size.width/1024),298.072205*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(571.987305*(size.width/1024),289.074097*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(565.988586*(size.width/1024),280.075989*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(559.989868*(size.width/1024),272.131714*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(551.883484*(size.width/1024),266.132996*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(543.939209*(size.width/1024),262.160858*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(534.049377*(size.width/1024),257.134888*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(522.051941*(size.width/1024),254.135529*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(510.054474*(size.width/1024),254.135529*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(499.029755*(size.width/1024),254.135529*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(487.032288*(size.width/1024),257.134888*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(477.142487*(size.width/1024),263.133636*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(467.090576*(size.width/1024),269.132355*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(461.091827*(size.width/1024),278.130463*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(453.147552*(size.width/1024),287.047485*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(447.148834*(size.width/1024),302.044342*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(443.095642*(size.width/1024),321.986053*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(447.148834*(size.width/1024),338.036743*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(452.174805*(size.width/1024),351.006958*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(459.146301*(size.width/1024),364.949982*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(469.036102*(size.width/1024),374.920837*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(482.006348*(size.width/1024),381.892334*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(500.975311*(size.width/1024),386.918304*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(519.944275*(size.width/1024),390.890442*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(533.887268*(size.width/1024),396.889160*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(545.884766*(size.width/1024),401.915131*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(555.936646*(size.width/1024),409.859406*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(562.908142*(size.width/1024),418.857513*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(567.934143*(size.width/1024),429.801147*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(571.825195*(size.width/1024),439.853058*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(572.960083*(size.width/1024),453.796082*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(571.825195*(size.width/1024),466.766296*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(568.906921*(size.width/1024),476.737183*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(562.908142*(size.width/1024),490.761230*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(553.828979*(size.width/1024),502.677643*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(542.966431*(size.width/1024),509.730194*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(525.942993*(size.width/1024),516.701721*(size.height/768)));
        this->change_segment(current_segment);
    }
    else if (current_number==9)
    {
        total_segments=45;
        current_segment=0;
        points_segment->addControlPoint(CCPointMake(526.915771*(size.width/1024),365.922729*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(511.027222*(size.width/1024),362.923370*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(495.949341*(size.width/1024),363.977203*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(482.979095*(size.width/1024),367.949341*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(471.143768*(size.width/1024),373.948059*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(458.011414*(size.width/1024),382.946167*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(448.121582*(size.width/1024),393.889801*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(442.122864*(size.width/1024),408.886627*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(436.124115*(size.width/1024),421.856873*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(436.124115*(size.width/1024),437.826477*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(437.096893*(size.width/1024),455.822662*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(441.150085*(size.width/1024),468.792908*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(448.121582*(size.width/1024),481.763153*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(455.093109*(size.width/1024),492.706787*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(463.037354*(size.width/1024),499.678284*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(474.062073*(size.width/1024),506.730835*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(482.979095*(size.width/1024),510.702972*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(494.976562*(size.width/1024),512.729553*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(506.974030*(size.width/1024),515.728943*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(517.025940*(size.width/1024),515.728943*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(526.915771*(size.width/1024),515.728943*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(536.967712*(size.width/1024),511.675751*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(547.992371*(size.width/1024),506.730835*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(556.909424*(size.width/1024),500.732117*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(568.906921*(size.width/1024),492.706787*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(578.796692*(size.width/1024),480.790375*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(582.849915*(size.width/1024),468.792908*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(585.930359*(size.width/1024),456.795441*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(587.875854*(size.width/1024),446.824585*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(588.848633*(size.width/1024),436.853699*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(587.875854*(size.width/1024),425.829010*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(584.957581*(size.width/1024),414.885376*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(580.904358*(size.width/1024),407.913879*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(570.852417*(size.width/1024),393.889801*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(562.908142*(size.width/1024),381.892334*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(554.963928*(size.width/1024),371.921478*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(547.992371*(size.width/1024),361.950592*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(539.885986*(size.width/1024),348.980377*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(531.941711*(size.width/1024),336.982910*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(523.997437*(size.width/1024),326.039276*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(516.053223*(size.width/1024),314.041809*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(506.974030*(size.width/1024),303.098175*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(500.002533*(size.width/1024),291.100708*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(492.058258*(size.width/1024),279.103241*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(481.033569*(size.width/1024),263.133636*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(468.063324*(size.width/1024),244.164658*(size.height/768)));
        this->change_segment(current_segment);
    }
    else if (current_number==10)
    {
        total_segments=14;
        current_segment=0;
        is_completed=false;
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,513.202332*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,491.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,471.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,451.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,431.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,411.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,391.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,371.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,351.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,331.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,311.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,291.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,271.136169*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(383.108307*(size.width/1024) ,251.136169*(size.height/768)));
        this->change_segment(current_segment);
    }
    else
    {
        total_segments=1;
        current_segment=0;
        this->change_segment(current_segment);
    }
}
void GamePlayLayer::change_segment(int segment)
{
    if (segment==4)
    {
        total_segments=14;
        current_segment=0;
        points_remain=NULL;
        points_segment=NULL;
        points_passed=NULL;
        points_passed=CCPointArray::create(0);
        points_remain=CCPointArray::create(0);
        points_segment=CCPointArray::create(0);
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,511.675751*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,496.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,476.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,456.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,436.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,416.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,396.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,376.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,356.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,336.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,316.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,296.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,276.162140*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(560.017090*(size.width/1024) ,256.162140*(size.height/768)));
    }
    else if (segment==10)
    {
        total_segments=34;
        current_segment=0;
        points_remain=NULL;
        points_segment=NULL;
        points_passed=NULL;
        points_passed=CCPointArray::create(0);
        points_remain=CCPointArray::create(0);
        points_segment=CCPointArray::create(0);
        points_segment->addControlPoint(CCPointMake(550.910706*(size.width/1024) ,515.728943*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(532.914490*(size.width/1024) ,512.729553*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(517.998718*(size.width/1024) ,504.704254*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(506.001282*(size.width/1024) ,492.706787*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(497.084229*(size.width/1024) ,478.763763*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(485.086761*(size.width/1024) ,461.821411*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(479.088043*(size.width/1024) ,442.852448*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(475.034821*(size.width/1024) ,421.856873*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(474.062073*(size.width/1024) ,394.862579*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(474.062073*(size.width/1024) ,367.949341*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(476.007599*(size.width/1024) ,346.953766*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(482.006348*(size.width/1024) ,327.984802*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(487.032288*(size.width/1024) ,307.070312*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(497.084229*(size.width/1024) ,291.100708*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(505.028503*(size.width/1024) ,276.103851*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(512.972778*(size.width/1024) ,266.132996*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(525.942993*(size.width/1024) ,258.107666*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(541.020874*(size.width/1024) ,255.108292*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(556.909424*(size.width/1024) ,253.162766*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(571.987305*(size.width/1024) ,255.108292*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(589.983521*(size.width/1024) ,263.133636*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(605.872070*(size.width/1024) ,281.129822*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(615.924011*(size.width/1024) ,296.045593*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(624.841064*(size.width/1024) ,314.041809*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(630.839783*(size.width/1024) ,335.037354*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(632.785278*(size.width/1024) ,358.951233*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(635.865723*(size.width/1024) ,377.920197*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(634.730835*(size.width/1024) ,404.914490*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(633.758057*(size.width/1024) ,427.855591*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(627.759338*(size.width/1024) ,449.823944*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(618.842285*(size.width/1024) ,471.792267*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(605.872070*(size.width/1024) ,493.760620*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(580.904358*(size.width/1024) ,512.729553*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(569.879700*(size.width/1024) ,517.674500*(size.height/768)));
        points_segment->addControlPoint(CCPointMake(550.910706*(size.width/1024) ,515.728943*(size.height/768)));

    }
    points_remain=points_segment;
    points_remain->retain();
    current_destination=0;
    total_segment_point=points_segment->count();
    this->get_next_destination(current_destination);
    current_segment++;
    //draw all the segment points
   // this->draw_segment_points();
}
void GamePlayLayer::draw_segment_points()
{
    //Draw entire segment first with diff. color.
        CCDrawNode *draw = CCDrawNode::create();
        this->addChild(draw, 6);
        for (int i=0; i<=total_segment_point;i++)
        {
            draw->drawSegment(points_segment->getControlPointAtIndex(i), points_segment->getControlPointAtIndex(i+1),4,  ccc4f(1,0,0,1));
        }
}
void GamePlayLayer::get_next_destination(int currentdestination)
{
    //current_destination
    //initial start and end points
    start_pnt=CCPoint(points_segment->getControlPointAtIndex(current_destination));
    end_pnt=CCPoint(points_segment->getControlPointAtIndex(++current_destination));
    float angleRadians = atan2(end_pnt.x - start_pnt.x, end_pnt.y - start_pnt.y);
    float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);
    float cocosAngle = -1 * angleDegrees;
    sprite_arrow->setRotation(180-cocosAngle);
    cr_srt_pnt=start_pnt;
    pv_srt_pnt=cr_srt_pnt;
    sprite_arrow->setPosition(start_pnt);
 
}
void GamePlayLayer::addRectangleBetweenPointsToBody(b2Body *body, CCPoint start, CCPoint end)
{
    float distance = sqrt( pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
    float sx=start.x;
    float sy=start.y;
    float ex=end.x;
    float ey=end.y;
    float dist_x=sx-ex;
    float dist_y=sy-ey;
    float angle= atan2(dist_y,dist_x);
    
    float px= (sx+ex)/2/PTM_RATIO - body->GetPosition().x;
    float py = (sy+ey)/2/PTM_RATIO - body->GetPosition().y;
    
    float width = abs(distance)/PTM_RATIO;
    float height =  brush->boundingBox().size.height/PTM_RATIO;
    
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width / 2, height / 2, b2Vec2(px,py),angle);
    
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 5;
    boxFixtureDef.filter.categoryBits = 2;
    body->CreateFixture(&boxFixtureDef);
}
CCRect GamePlayLayer::getBodyRectangle(b2Body* body)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float minX2 = s.width;
    float maxX2 = 0;
    float minY2 = s.height;
    float maxY2 = 0;
    const b2Transform& xf = body->GetTransform();
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
    {
        b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
        int32 vertexCount = poly->m_vertexCount;
        b2Assert(vertexCount <= b2_maxPolygonVertices);
        for (int32 i = 0; i < vertexCount; ++i)
        {
            b2Vec2 vertex = b2Mul(xf, poly->m_vertices[i]);
            if(vertex.x < minX2)
            {
                minX2 = vertex.x;
            }
            if(vertex.x > maxX2)
            {
                maxX2 = vertex.x;
            }
            if(vertex.y < minY2)
            {
                minY2 = vertex.y;
            }
            if(vertex.y > maxY2)
            {
                maxY2 = vertex.y;
            }
        }
    }
    maxX2 *= PTM_RATIO;
    minX2 *= PTM_RATIO;
    maxY2 *= PTM_RATIO;
    minY2 *= PTM_RATIO;
    float width2 = maxX2 - minX2;
    float height2 = maxY2 - minY2;
    float remY2 = s.height - maxY2;
    return CCRectMake(minX2, remY2, width2, height2);
}
CCPoint GamePlayLayer::shortest_distance(cocos2d::CCPoint current_location, cocos2d::CCPoint start_location, cocos2d::CCPoint end_location)
{
    float point_x=current_location.x;
    float point_y=current_location.y;
    float line_x1=start_location.x;
    float line_y1=start_location.y;
    float line_x2=end_location.x;
    float line_y2=end_location.y;
    CCPoint sortest_dist;
    //find the slope12
    float slope_of_line = (line_y1 - line_y2) / (line_x1 - line_x2);
    //line is parpendicular   equation will be x=a;
    if (slope_of_line==INFINITY||slope_of_line==-INFINITY)
    {
        float x_intercept=line_x1;
        float y_intercept=point_y;
        sortest_dist=ccp(x_intercept, y_intercept);
        //      return ccp(x_intercept, y_intercept);
    }
    //line is horizontal equation will be y=b;
    else if(slope_of_line==0||slope_of_line==-0)
    {
        float x_intercept=point_x;
        float y_intercept=line_y1;
        sortest_dist=ccp(x_intercept, y_intercept);
        //return ccp(x_intercept, y_intercept);
    }
    else
    {
        // find the perpendicular slope
        float perpendicular_slope = (line_x1 - line_x2) / (line_y1 - line_y2) * -1;
        // find the y_intercept of line BC
        float y_intercept = (-slope_of_line * line_x2) + line_y2;
        // find the y_intercept of line AX
        float new_line_y_intercept = -(perpendicular_slope * point_x)+point_y;
        //get the x_coordinate of point X
        float intersect_x = (y_intercept - new_line_y_intercept) / (perpendicular_slope - slope_of_line);
        // get the y_coordinate of point X32
        float intersect_y = slope_of_line * intersect_x + y_intercept;
        sortest_dist=ccp(intersect_x,intersect_y);
        //return ccp(intersect_x,intersect_y);
    }
    //check if point is between end points...
    float total_dist=ccpDistance(CCPoint(line_x1,line_y1),CCPoint(line_x2,line_y2));
    float dist_from_a=ccpDistance(CCPoint(sortest_dist.x,sortest_dist.y),CCPoint(line_x1,line_y1));
    float dist_from_b=ccpDistance(CCPoint(sortest_dist.x,sortest_dist.y),CCPoint(line_x2,line_y2));
    //point is within a segment...
    if (total_dist==dist_from_a+dist_from_b)
    {
        start_pnt=sortest_dist;
        return sortest_dist;
    }
    else
    {
        //retun object which is nearest to sortest point...
        return (dist_from_a>dist_from_b)? CCPoint(line_x2,line_y2):CCPoint(line_x1,line_y1);
    }
}
#pragma mark -
#pragma mark Touch Delegate
#pragma mark -
void GamePlayLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it;
    CCTouch* touch;
    for( it = pTouches->begin(); it != pTouches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        if (sprite_arrow->boundingBox().containsPoint(touch->getLocation()))
        {
            this->touch=touch;
            is_drag_started=true;
        }
        if(!touch)
        break;
		plataformPoints.clear();
        CCPoint location = touch->getLocationInView();
        location = CCDirector::sharedDirector()->convertToGL(location);
		plataformPoints.push_back(location);
        previousLocation = location;
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_staticBody;
        myBodyDef.position.Set(location.x/PTM_RATIO,location.y/PTM_RATIO);
        currentPlatformBody = world->CreateBody(&myBodyDef);
    }
}
void GamePlayLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (is_drag_started)
    {
        CCTouch *touch = (CCTouch *)pTouches->anyObject();
        if (sprite_arrow->boundingBox().containsPoint(touch->getLocation()))
        {
          this->touch=touch;
          CCPoint location = touch->getLocation();
          cr_srt_pnt=this->shortest_distance(location, start_pnt,end_pnt);
          target_mask->begin();
          float distance = ccpDistance(pv_srt_pnt, cr_srt_pnt);
          for (int i = 0; i < distance; i++)
          {
               float difx = cr_srt_pnt.x - pv_srt_pnt.x;
               float dify = cr_srt_pnt.y - pv_srt_pnt.y;
               float delta = (float)i / distance;
                brush->setPosition(ccp(cr_srt_pnt.x + (difx * delta), cr_srt_pnt.y + (dify * delta)));
                // Draw
                brush->visit();
           }
          target_mask->end();
          //sprite_arrow->setPosition(ccp(cr_srt_pnt.x, cr_srt_pnt.y));
          CCSetIterator it;
          for( it = pTouches->begin(); it != pTouches->end(); it++)
          {
            float distance = sqrt( pow(cr_srt_pnt.x - pv_srt_pnt.x, 2) + pow(cr_srt_pnt.y - pv_srt_pnt.y, 2));
            if(distance > 10)
            {
                addRectangleBetweenPointsToBody(currentPlatformBody, pv_srt_pnt, cr_srt_pnt);
                plataformPoints.push_back(cr_srt_pnt);
                pv_srt_pnt=cr_srt_pnt;
            }
          }
          location_from=location;
          sprite_arrow->setPosition(location);
          //sprite_arrow->setPosition(ccp(cr_srt_pnt.x, cr_srt_pnt.y));
        }
   }
}



void GamePlayLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator iter = pTouches->begin();
    for (; iter != pTouches->end(); iter++)
    {
        CCTouch* pTouch = (CCTouch*)(*iter);
        CCPoint location=pTouch->getLocation();
        if (sprite_arrow->boundingBox().containsPoint(location))
        {
            this->touch=pTouch;
            CCLOG("Touch ended");
            SimpleAudioEngine::sharedEngine()->playEffect("sound/punch.mp3");
            sprite_arrow->runAction(CCMoveTo::create(0.3, ccp(cr_srt_pnt.x, cr_srt_pnt.y)));
//          sprite_arrow->setPosition(ccp(cr_srt_pnt.x, cr_srt_pnt.y));
            is_drag_started=false;
        }
    }
}
void GamePlayLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
    CCLOG("Touch Cancel");
    //ccTouchesEnded(pTouches, pEvent);
    CCSetIterator iter = pTouches->begin();
    for (; iter != pTouches->end(); iter++)
    {
        CCTouch* pTouch = (CCTouch*)(*iter);
        CCPoint location=pTouch->getLocation();
        if (sprite_arrow->boundingBox().containsPoint(location))
        {
            this->touch=pTouch;
            CCLOG("Touch ended");
             SimpleAudioEngine::sharedEngine()->playEffect("sound/punch.mp3");
            is_drag_started=false;
        }
    }

}
#pragma mark -
#pragma mark animation call back methods
#pragma mark -
//animation after draw is completed...
void GamePlayLayer::show_char_anim(CCNode* sender)
{
    target_mask->setVisible(false);
    if(current_play_level==3)
    {
        this->removeChildByTag(789,true);
        m_sound_effect=SimpleAudioEngine::sharedEngine()->playEffect("sound/hurray.mp3");
    }
    else if(current_play_level==2)
    {
        m_sound_effect=SimpleAudioEngine::sharedEngine()->playEffect("sound/clapping.mp3");
    }
}
//for playing number sound...
void GamePlayLayer::complete_char_anim(CCNode* sender)
{
    target_mask->clearDepth(0);
    target_mask->clear(0.0,0.0,0.0,0);
    target_mask->setVisible(true);
    int_play_no=0;
    m_fInterval=1.0f;
    SimpleAudioEngine::sharedEngine()->stopEffect(m_sound_effect);
    this->schedule(schedule_selector(GamePlayLayer::play_number_sound), m_fInterval, current_number, 1.00);
}
void GamePlayLayer::play_number_sound(float dt)
{
    int_play_no++;
    m_fInterval += 1.0f;
    if (int_play_no>current_number)
    {
        this->scheduleOnce(schedule_selector(GamePlayLayer::next_level), 1.0f);
    }
    else
    {
        CCSprite_animation *sprt_animation=(CCSprite_animation*)arr_char_anim->objectAtIndex(int_play_no-1);
        sprt_animation->setVisible(true);
        CCString *str_playeffect=CCString::createWithFormat("sound/%d.mp3",int_play_no);
        m_nSoundId =SimpleAudioEngine::sharedEngine()->playEffect(str_playeffect->getCString());
    }
}
void GamePlayLayer::next_level()
{
    this->unschedule(schedule_selector(GamePlayLayer::play_number_sound));
    if (current_play_level==3)
    {
        CCLog("completed three times play");
        if (current_number==10)
        {
        }
//        else if (current_number==1)
//        {
//        }
        else
        {
            this->onNextClicked(NULL, NULL);
        }
    }
    else
    {
        _mAnimationManager->runAnimationsForSequenceNamedTweenDuration("completion_char", 0.3f);
        for (int i=0; i<_anim_batch_sprite->getChildrenCount(); i++)
        {
            CCSprite_animation *sprt_animation=(CCSprite_animation*)_anim_batch_sprite->getChildByTag(2000+i);
            //sprt_animation->setVisible(false);
            sprt_animation->runAction(CCSequence::create(CCToggleVisibility::create(),0));
        }
    }
}
//move to the next level...
void GamePlayLayer::next_level_play(CCNode* sender)
{
    current_play_level++;
    this->play_for_level(current_play_level);
}
#pragma mark -
#pragma mark animate Arrow Guide
#pragma mark -
//arrow animations...
void GamePlayLayer::start_animating(CCNode* sender)
{
    this->reorderChild(this->sprite_mask, 3);
    _mAnimationManager->runAnimationsForSequenceNamedTweenDuration("animatepath_anim", 0.003f);
    //animating sparrow...
   // this->sparrow_animate();
}
void GamePlayLayer::animate_path(CCNode* sender)
{
   //sprt_animation_sparrow->setVisible(true);
   arr_anim_arrow->removeAllObjects();
   m_fInterval=1.0f;
   current_animating=0;
   int total_points=points_segment->count();
   int stepper=2;
   if (current_play_level==3)
   {
       stepper=4;
   }
   for (int i=0;i<points_segment->count(); i=i+stepper)
   {
       if (this->getChildByTag(989+i))
       {
          this->removeChildByTag(989+i, true);
       }
       CCArrow *anim_arrow;
       if (i==total_points)
       {
           //CCLog("Last:%d",i);
       }
       else
       {
           //CCLog("others:%d",i);
       }
       anim_arrow=CCArrow::create("arrowguid.png");
       anim_arrow->setColor(getColor(current_number));
       CCPoint  pnt_1=CCPoint(points_segment->getControlPointAtIndex(current_animating));
       CCPoint  pnt_2=CCPoint(points_segment->getControlPointAtIndex(current_animating+stepper));
       float angleRadians = atan2(pnt_2.x - pnt_1.x, pnt_2.y - pnt_1.y);
       float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);
       float cocosAngle = -1 * angleDegrees;
       anim_arrow->setRotation(180-cocosAngle);
       anim_arrow->setPosition(pnt_1);
       current_animating=current_animating+stepper;
       this->addChild(anim_arrow, 4, 989+i);
       arr_anim_arrow->addObject(anim_arrow);
    }
   current_animating=0;
   this->schedule(schedule_selector(GamePlayLayer::animate), 0.1, arr_anim_arrow->count()-1, 0.2);
}
void GamePlayLayer::animate(CCTime dt)
{
    CCObject *obj_sprite=NULL;
    obj_sprite=arr_anim_arrow->objectAtIndex(current_animating);
    CCArrow *anim_sprite=(CCArrow*)obj_sprite;
    current_animating=current_animating+1;
    CCFiniteTimeAction  *action2 = CCSequence::create(CCFadeIn::create(2),NULL);
    CCFiniteTimeAction  *act     = CCRepeatForever::create(CCSequence::create( action2,CCDelayTime::create(0.7), NULL));
    anim_sprite->runAction(act);
}
void GamePlayLayer::remove_current_arrow()
{ 
    CCObject *arrow=NULL;
    items_to_delete=CCArray::createWithCapacity(arr_anim_arrow->count());
    CCARRAY_FOREACH(arr_anim_arrow, arrow)
    {
        if (arrow)
        {
            CCArrow *_arrow=(CCArrow*)arrow;
            if (_arrow)
            {
                if (_arrow->boundingBox().intersectsRect(sprite_arrow->boundingBox()))
                {
                    items_to_delete->addObject(_arrow);
                }
            }
        }
    }
    //remove hited arrow
    if (items_to_delete)
    {
        CCObject *dirty_arrow=NULL;
        CCARRAY_FOREACH(items_to_delete, dirty_arrow)
        {
             CCArrow *_dirty_arrow=(CCArrow *)dirty_arrow;
            _dirty_arrow->runAction(CCScaleBy::create(0.3, 2.4));
            float duration = 1;
            float rotateDuration = 0.2f;
             int repeatTime = 5;
            CCPoint endPos(- size.width / 4.0f, size.height * (0.5 + (float)rand() / (2.0f * RAND_MAX)));
            CCAction * seq = CCSequence::create(CCSpawn::create(CCMoveTo::create(duration, endPos),CCRepeat::create(CCRotateBy::create(rotateDuration, (rand()%2) ? 360 : -360),repeatTime),CCFadeOut::create(duration),0),CCRemoveSelf::create(true),0);
            _dirty_arrow->runAction(seq);
        }
        items_to_delete->removeAllObjects();
    }
}
void GamePlayLayer::remove_all_arrow()
{
    CCObject *arrow=NULL;
    items_to_delete=CCArray::createWithCapacity(arr_anim_arrow->count());
    CCARRAY_FOREACH(arr_anim_arrow, arrow)
    {
        if (arrow)
        {
            CCArrow *_arrow=(CCArrow*)arrow;
            if (_arrow)
            {
//                if (_arrow->boundingBox().intersectsRect(sprite_arrow->boundingBox()))
//                {
                    items_to_delete->addObject(_arrow);
//                }
            }
        }
    }
    //remove hited arrow
    if (items_to_delete)
    {
        CCObject *dirty_arrow=NULL;
        CCARRAY_FOREACH(items_to_delete, dirty_arrow)
        {
            CCArrow *_dirty_arrow=(CCArrow *)dirty_arrow;
            _dirty_arrow->runAction(CCScaleBy::create(0.3, 2.4));
            float duration = 1;
            float rotateDuration = 0.2f;
            int repeatTime = 5;
            CCPoint endPos(- size.width / 4.0f, size.height * (0.5 + (float)rand() / (2.0f * RAND_MAX)));
            CCAction * seq = CCSequence::create(CCSpawn::create(CCMoveTo::create(duration, endPos),CCRepeat::create(CCRotateBy::create(rotateDuration, (rand()%2) ? 360 : -360),repeatTime),CCFadeOut::create(duration),0),CCRemoveSelf::create(true),0);
            _dirty_arrow->runAction(seq);
        }
        items_to_delete->removeAllObjects();
    }
}
#pragma mark -
#pragma mark  menu click events
#pragma mark -
void GamePlayLayer::onDidSwipe(CCObject* pSender)
{
    CCSwipe * swipe = (CCSwipe*)pSender;
    CCLOG("%f %f",swipe->location.x,swipe->location.y);
    CCPoint nodePoint = sprite_latter->convertToNodeSpace(swipe->location);
    CCRect rect = CCRectMake(0, 0, sprite_latter->getContentSize().width, sprite_latter->getContentSize().height);
    if(rect.containsPoint(nodePoint))
    {
        CCLOG("Return");
        return;
    }
    if(swipe->direction==kSwipeGestureRecognizerDirectionRight)
    {
        if (current_play_level!=1)
            return;
        if (current_number!=1)
        {
            target_mask->clearDepth(0);
            target_mask->clear(0.0,0.0,0.0,0);
            target_mask->setVisible(true);
            this->unschedule(schedule_selector(GamePlayLayer::animate));
             this->unschedule(schedule_selector(GamePlayLayer::play_number_sound));
            this->remove_all_arrow();
            this->onPreviousClicked(NULL, NULL);
        }
    }
    else if (swipe->direction==kSwipeGestureRecognizerDirectionLeft)
    {
        if (current_play_level!=1)
            return;
        if (current_number!=10)
        {
            target_mask->clearDepth(0);
            target_mask->clear(0.0,0.0,0.0,0);
            target_mask->setVisible(true);
            this->unschedule(schedule_selector(GamePlayLayer::animate));
            this->unschedule(schedule_selector(GamePlayLayer::play_number_sound));
            this->remove_all_arrow();
            this->onNextClicked(NULL, NULL);
           // this->onPreviousClicked(NULL, NULL);
        }
    }
    else if (swipe->direction==kSwipeGestureRecognizerDirectionUp)
    {
    }
    else  if (swipe->direction==kSwipeGestureRecognizerDirectionDown)
    {
         SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
        CCDirector::sharedDirector()->popScene();
    }
}
void GamePlayLayer::onHomeClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    Scene_common* pScene = new Scene_common();
    if (pScene)
    {
        pScene->ReplaceScene("homeLayer.ccbi","homeLayer", homeLayerLoader::loader(),this,true,callfuncO_selector(homeLayer::onDidSwipe));
        pScene->release();
    }
}
#pragma mark -
#pragma mark change number
#pragma mark -
void GamePlayLayer::onNextClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
   SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
   current_play_level=0;
   CCString *str_number=CCString::createWithFormat("anim_%d.plist",current_number);
   CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(str_number->getCString());
   CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(CCString::createWithFormat("%d.plist",current_number)->getCString());
   total_segments=0;
   current_segment=0;
   m_nSoundId=0;
   int_play_no=0;
   is_drag_started=false;
   this->sprite_arrow->setVisible(true);
   this->sprite_arrow->setOpacity(255.0f);
   this->sprite_arrow->setPosition(ccp(size.width/2,size.height/2));
   this->sprite_latter->setVisible(true);
   this->sprite_latter->setPosition(ccp(size.width/2,size.height/2));
   this->sprite_latter->setColor(ccWHITE);
   this->sprite_latter->setColor(getColor(current_number+1));
   brush->setColor(getColor(current_number+1));
   CCUserDefault::sharedUserDefault()->setIntegerForKey("number", current_number+1);
   current_number=current_number+1;
    _anim_batch_sprite->removeAllChildrenWithCleanup(true);
    arr_char_anim->removeAllObjects();
    points_remain=NULL;
    points_segment=NULL;
    points_passed=NULL;
    points_passed=CCPointArray::create(0);
    points_remain=CCPointArray::create(0);
    points_segment=CCPointArray::create(0);
    CCString *str=CCString::createWithFormat("Write Number \"%s\"",this->getnumberword(current_number));
    lbl_header->setString(str->getCString());
   //current_play_level++;
   this->loading_assets();
   _mAnimationManager->runAnimationsForSequenceNamedTweenDuration("change_char", 0.3f);
   _mAnimationManager->runAnimationsForSequenceNamedTweenDuration("animatepath_anim", 0.003f);
}
void GamePlayLayer::onPreviousClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    current_play_level=0;
    CCString *str_number=CCString::createWithFormat("anim_%d.plist",current_number);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(str_number->getCString());
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(CCString::createWithFormat("%d.plist",current_number)->getCString());
    total_segments=0;
    current_segment=0;
    m_nSoundId=0;
    int_play_no=0;
    is_drag_started=false;
    this->sprite_arrow->setVisible(true);
    this->sprite_arrow->setOpacity(255.0f);
    this->sprite_arrow->setPosition(ccp(size.width/2,size.height/2));
    this->sprite_latter->setVisible(true);
    this->sprite_latter->setPosition(ccp(size.width/2,size.height/2));
    CCUserDefault::sharedUserDefault()->setIntegerForKey("number", current_number-1);
    current_number=current_number-1;
    _anim_batch_sprite->removeAllChildrenWithCleanup(true);
    arr_char_anim->removeAllObjects();
    points_remain=NULL;
    points_segment=NULL;
    points_passed=NULL;
    points_passed=CCPointArray::create(0);
    points_remain=CCPointArray::create(0);
    points_segment=CCPointArray::create(0);
    CCString *str=CCString::createWithFormat("Write Number \"%s\"",this->getnumberword(current_number));
    lbl_header->setString(str->getCString());
    //current_play_level++;
    this->loading_assets();
    _mAnimationManager->runAnimationsForSequenceNamedTweenDuration("change_char", 0);
    _mAnimationManager->runAnimationsForSequenceNamedTweenDuration("animatepath_anim", 0);
//    next->setVisible(false);
//    prev->setVisible(false);
}
#pragma mark -
#pragma mark animationManager
void GamePlayLayer::setAnimationManager(cocos2d::extension::CCBAnimationManager *animationManager)
{
    if (animationManager)
    {
        CC_SAFE_RELEASE_NULL(_mAnimationManager);
        _mAnimationManager = animationManager;
        CC_SAFE_RETAIN(animationManager);
    }
}
#pragma mark -
#pragma mark cocosbuilder
#pragma mark -
void GamePlayLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    
}
SEL_MenuHandler GamePlayLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onHomeClicked", GamePlayLayer::onHomeClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onNextClicked", GamePlayLayer::onNextClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPreviousClicked", GamePlayLayer::onPreviousClicked);
    //CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSettingClicked", GamePlayLayer::onSettingClicked);
    return NULL;
}
SEL_CCControlHandler GamePlayLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool GamePlayLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_arrow", CCSprite*, sprite_arrow);
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "_mAnimationManager", CCBAnimationManager *, this->_mAnimationManager);
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_latter", CCSprite *, this->sprite_latter);
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_mask", CCSprite *, this->sprite_mask);
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_header", CCLabelTTF *, this->lbl_header);
  return false;
}
bool GamePlayLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    bool bRet = false;
    return bRet;
}
SEL_CallFuncN GamePlayLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "show_char_anim", GamePlayLayer::show_char_anim);
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "complete_char_anim", GamePlayLayer::complete_char_anim);
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "next_level_play", GamePlayLayer::next_level_play);
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "animate_path", GamePlayLayer::animate_path);
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "start_animating", GamePlayLayer::start_animating);
    return NULL;
}