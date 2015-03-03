//  HelloCocosBuilderLayer.cpp
//  Demo
//  Created by Neil D on 20/05/13.
#include "CountNumberLayer.h"
#include "homeLayerLoader.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "CCMask.h"
#include "../Extension/CCGestureRecognizer/CCSwipeGestureRecognizer.h"
#include "SimpleAudioEngine.h"
using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

#warning change value
#define TOTALQUE  15
//record for the scale values
static float scale_value[10][12]=
{
    {1, 0.8,    0.5,    0.4,    0.30,   0.30,   0.25,   0.25,   0.2,    0.2,    0.2,    0.5},
    
    {2, 1,      0.80,   0.50,   0.45,   0.40,   0.35,   0.35,   0.30,   0.30,  0.30,    0.20},
    
    {3, 1,      0.80,    0.65,    0.60,   0.60,   0.45,  0.45,   0.40,    0.40,   0.40,    0.5},
    
    {4, 1,      0.7,    0.60,    0.45,   0.40,   0.35,   0.35,   0.35,    0.32,    0.32,    0.5},
    
    {5, 1,      0.8,    0.7,    0.6,   0.55,   0.45,   0.45,   0.35,    0.35,    0.35,    0.5},
    
    {6, 1,      0.9,    0.8,    0.80,   0.55,   0.55,   0.55,   0.43,    0.43,    0.43,    0.5},
    
    {7, 1,    1,    0.7,    0.65,   0.45,   0.45,   0.45,   0.4,    0.4,    0.4,    0.5},
    
    {8, 1,    1,    1,    1,   0.9,   0.80,   0.75,  0.65,  0.65,    0.65,    0.5},
    
    {9, 1,    1,    0.8,    0.8,   0.70,   0.60,   0.5,   0.50,    0.50,    0.50,    0.5},
    
    {10,1,    1,    0.80,    0.80,   0.70,   0.60,   0.55,   0.45,    0.45,    0.45,    0.5},
};
static std::string object_name[] =
{
    "panda",
    "balls",
    "birds",
    "Ice creams",
    "ducks",
    "fishes",
    "roses",
    "stars",
    "dogs",
    "carrots",
};
CountNumberLayer::CountNumberLayer():mAnimationManager(NULL),lbl_question(NULL),sprt_container_1(NULL),
sprt_container_2(NULL),sprt_container_3(NULL),_anim_batch_sprite(NULL),sprt_container_active_1(NULL),sprt_container_active_2(NULL),sprt_container_active_3(NULL),lbl_welcome_text(NULL),lbl_user_name(NULL),
lbl_start_pos(NULL),
lbl_precent(NULL),
lbl_end_pos(NULL),
sprt_progress_timer(NULL),
is_progress_completed(false),
question_no(1),
_leader_board(NULL),
is_setting_visible(false),
user_panel(NULL),
prev_random(0)
{
    arr_char_anim=CCArray::createWithCapacity(10);
    arr_char_anim->retain();
    question_no=1;
    is_running=false;
    arr_strs_crnt=CCArray::createWithCapacity(15);
    arr_strs_crnt->retain();
}
CountNumberLayer::~CountNumberLayer()
{
    CC_SAFE_RELEASE(mAnimationManager);
    CC_SAFE_RELEASE(sprt_container_1);
    CC_SAFE_RELEASE(sprt_container_2);
    CC_SAFE_RELEASE(sprt_container_3);
}
void CountNumberLayer::onEnter()
{
    setTouchEnabled(true);
    user_panel=this->getChildByTag(999);
    size=CCDirector::sharedDirector()->getWinSize();
    //set initial data
    set_initial_data();
    //create new quiz...
    this->create_new_quiz();
    CCLayer::onEnter();
}
void CountNumberLayer::set_initial_data()
{
    //question label settings...
    lbl_question->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    lbl_question->setHorizontalAlignment(kCCTextAlignmentCenter);
    lbl_question->setFontName("fonts/STATION.TTF");
    lbl_question->setFontSize(72);
  
    //user name settings...
    lbl_welcome_text->setFontSize(72);
    lbl_user_name->setFontSize(72);
    lbl_precent->setFontSize(72);
    lbl_user_name->setString(user_name->getCString());
   
    // lbl_welcome_text->setHorizontalAlignment(kCCTextAlignmentRight);
    //  lbl_user_name->setHorizontalAlignment(kCCTextAlignmentLeft);
    //lbl_welcome_text->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    //lbl_user_name->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
 
    lbl_question->enableStroke(ccGREEN, 1);

    //set  user profile picture settings
    std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fullPath = writablePath_temp + CCString::createWithFormat("%s",user_profile_path->getCString())->getCString();
    if (user_profile_path->length()>0&&CCFileUtils::sharedFileUtils()->isFileExist(fullPath.c_str()))
    {
        CCSprite *sprt_img_profile=CCSprite::create("profilepic_bg.png");
        //this->addChild(sprt_img_profile, 10,-1);
        CCSprite *profile_pic=CCSprite::create(fullPath.c_str());
        profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        CCNode *node=user_panel->getChildByTag(189);
        sprt_img_profile->setPosition(node->getPosition());
        CCMask *masked = CCMask::create(sprt_img_profile , profile_pic);
        user_panel->addChild(masked,11);
        masked->setAnchorPoint(ccp(0.5,0.0));
        masked->setPosition(node->getPosition());
        masked->setPosition(ccpAdd(masked->getPosition(), ccp(0, 0)));
        masked->setRotation(180);
        //masked->setScale(0.88);
        masked->setOpacity(0);
        masked->runAction(CCSequence::create(CCDelayTime::create(1.5),CCFadeIn::create(0.3),NULL));
    }
    else
    {
        user_profile_path=CCString::createWithFormat("profilepic_bg.png");
        user_profile_path->retain();
        CCSprite *sprt_img_profile=CCSprite::create("profilepic_bg.png");
        //this->addChild(sprt_img_profile, 10,-1);
        CCSprite *profile_pic=CCSprite::create("place holder.png");
        profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        CCNode *node=user_panel->getChildByTag(189);
        sprt_img_profile->setPosition(node->getPosition());
        CCMask *masked = CCMask::create(sprt_img_profile , profile_pic);
        user_panel->addChild(masked,11);
        masked->setAnchorPoint(ccp(0.5,1.0f));
        masked->setPosition(node->getPosition());
        masked->setPosition(ccpAdd(masked->getPosition(), ccp(0, 0 )));
        //masked->setScale(0.88);
        masked->setOpacity(0);
        masked->runAction(CCSequence::create(CCDelayTime::create(1.5),CCFadeIn::create(0.3),NULL));
    }

    //progress bar data...
    lbl_precent->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    lbl_precent->setHorizontalAlignment(kCCTextAlignmentCenter);
    lbl_precent->setFontName("STATION.TTF");
    lbl_precent->setFontSize(75);
    
    lbl_end_pos->setHorizontalAlignment(kCCTextAlignmentCenter);
    lbl_end_pos->setHorizontalAlignment(kCCTextAlignmentCenter);
    lbl_end_pos->setFontName("STATION.TTF");
    lbl_end_pos->setFontSize(35);
    
    lbl_start_pos->setHorizontalAlignment(kCCTextAlignmentCenter);
    lbl_start_pos->setHorizontalAlignment(kCCTextAlignmentCenter);
    lbl_start_pos->setFontName("STATION.TTF");
    lbl_start_pos->setFontSize(35);

    lbl_precent->enableStroke(ccGREEN, 1);
    lbl_start_pos->enableStroke(ccGREEN, 1);
    lbl_end_pos->enableStroke(ccGREEN, 1);
    
    lbl_start_pos->setVisible(false);
    lbl_precent->setVisible(false);
    lbl_end_pos->setVisible(false);
    
    //CCSprite *sprt=(CCSprite*)user_panel->getChildByTag(2112);
    progress_bar=CCProgressFromTo::create(45, 0, 100);
    progress_bar->retain();
    timer_progress = CCProgressTimer::create(CCSprite::create("timer_100%.png"));
    timer_progress->setType(kCCProgressTimerTypeRadial);
    //timer_progress->setMidpoint(ccp(0, 0));
   // timer_progress->setBarChangeRate(ccp(1, 0));
    timer_progress->setPosition(ccpAdd(sprt_progress_timer->getPosition(),ccp(sprt_progress_timer->getContentSize().height/2,0)));
    //timer_progress->setAnchorPoint(ccp(0.5,1.0));
    user_panel->addChild(timer_progress);
    
    CCSprite *sprt_star=(CCSprite*)user_panel->getChildByTag(19);
    star_gain_right = CCProgressTimer::create(CCSprite::create("star_activ.png"));
    star_gain_right->setType(kCCProgressTimerTypeBar);
    star_gain_right->retain();
    star_gain_right->setMidpoint(ccp(0, 0));
    star_gain_right->setBarChangeRate(ccp(1, 0));
    user_panel->addChild(star_gain_right,111);
    star_gain_right->setRotation(sprt_star->getRotation());
    star_gain_right->setScale(sprt_star->getScale());
    star_gain_right->setPosition(sprt_star->getPosition());

    //toggle setting button
//    setting=CCProgressFromTo::create(1, 0, 100);
//    setting->retain();
//    setting_reverse=CCProgressFromTo::create(1, 100, 0);
//    setting_reverse->retain();
//    setting_progress = CCProgressTimer::create(CCSprite::create("setting_bg.png"));
//    setting_progress->setType(kCCProgressTimerTypeBar);
//    setting_progress->setMidpoint(ccp(0, 0));
//    setting_progress->setBarChangeRate(ccp(1, 0));
//    this->addChild(setting_progress,19);
}
void CountNumberLayer::update_percentage(float dt)
{
    CCString *str=CCString::createWithFormat("%3.0f%s",timer_progress->getPercentage(),"%");
    lbl_precent->setString(str->getCString());
    is_progress_completed=false;
    if (timer_progress->getPercentage()>=100)
    {
        is_progress_completed=true;
        this->unschedule(schedule_selector(CountNumberLayer::update_percentage));
    }
}
void CountNumberLayer::start_timer()
{
    //progress bar data...
    //.lbl_start_pos->setVisible(true);
    lbl_precent->setVisible(true);
    //lbl_end_pos->setVisible(true);
    //sprt_progress_timer->setVisible(true);
    timer_progress->setPosition(ccpAdd(sprt_progress_timer->getPosition(),ccp(0,-sprt_progress_timer->getContentSize().height/2)));
    if (timer_progress->isRunning())
    {
        this->unschedule(schedule_selector(CountNumberLayer::update_percentage));
        timer_progress->stopAllActions();
    }
    timer_progress->runAction(progress_bar);
    this->schedule(schedule_selector(CountNumberLayer::update_percentage));
}
void CountNumberLayer::provide_star(int percent)
{
    //this->removeChild(star_gain_right,true);
    //get actual second from the percentage
    // //CCLog("%d",percent);
    percent=(percent*45)/100;
    if (percent<=15)
    {
        ////CCLog("five star:%d",percent);
        add_star(5);
        percent=100;
    }
    else if (percent<=30)
    {
        ////CCLog("four star:%d",percent);
        add_star(4);
        percent=80;
    }
    else if (percent<=45&&is_progress_completed==false)
    {
        ////CCLog("three star %d",percent);
        add_star(3);
        percent=60;
    }
    else
    {
        add_star(2);
        ////CCLog("two star %d",percent);
        percent=40;
    }
    //star earned by user
    star_gain=CCProgressFromTo::create(0.5, 0, percent);
    star_gain_right->runAction(star_gain);
}
void CountNumberLayer::add_star(int star)
{
 arr_strs_crnt->addObject(CCInteger::create(star));
}
#pragma mark -
#pragma mark core logic
#pragma mark -
void CountNumberLayer::create_new_quiz()
{
    //get three random object...
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8,9,10};
    int n = sizeof(arr)/ sizeof(arr[0]);
    srand ( time(NULL) );
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);
        // Swap arr[i] with the element at random index
        int *a=&arr[i];
        int *b=&arr[j];
        int temp = *a;
        *a = *b;
        *b = temp;
    }
    
    int random_animal_1=arr[0];
   
    int random_no=arr[1];
    if (random_no==prev_random)
    {
        random_no=arr[3];
    }
    prev_random=random_no;
    int random_animal_2=arr[2];
    //correct answer position(1,2 or 3)...
    int trueposition=1+rand()%3;
//    float positions[3][2]={{226.0*(size.width/1024),308.0*(size.height/768)},{516.0*(size.width/1024),307.0*(size.height/768)},{807.0*(size.width/1024),316.0*(size.height/768)},};
//    int position[3]={581*(size.width/1024),290*(size.width/1024),0};

    float positions[3][2]={{668.0*(size.width/1024),499.0*(size.height/768)},{512.0*(size.width/1024),230.0*(size.height/768)},{819.0*(size.width/1024),230.0*(size.height/768)},};
    int position_x[3]={138*(size.width/1024),300*(size.width/1024),-10*(size.width/1024)};
    int position_y[3]={-180*(size.width/1024),80*(size.width/1024),80*(size.width/1024)};
    
    true_pos[0]=0;
    true_pos[1]=0;
    true_pos[2]=0;
    this->load_sprite_animation(random_no);
    float scale;
    switch (trueposition)
    {
        case 1:
            int_fake_no[0]=int_true_no=random_no;
            true_pos[0]=0;
            sprt_container_active_1->setTag(int_fake_no[0]);
            sprt_container_active_1->setPosition(ccp(positions[0][0],positions[0][1]));
            scale=scale_value[int_true_no-1][random_no];
            this->sprites_display(random_no,position_x[0],position_y[0],scale);
            break;
        case 2:
            int_fake_no[1]=int_true_no=random_no;
            true_pos[0]=1;
            sprt_container_active_1->setTag(int_fake_no[1]);
            sprt_container_active_1->setPosition(ccp(positions[1][0],positions[1][1]));
            scale=scale_value[int_true_no-1][random_no];
            this->sprites_display(random_no,position_x[1],position_y[1],scale);
            break;
        case 3:
            int_fake_no[2]=int_true_no=random_no;
            true_pos[0]=2;
            sprt_container_active_1->setTag(int_fake_no[2]);
            sprt_container_active_1->setPosition(ccp(positions[2][0],positions[2][1]));
            scale=scale_value[int_true_no-1][random_no];
            this->sprites_display(random_no,position_x[2],position_y[2],scale);
            break;
        default:
            break;
    }
    if (trueposition==1)
    {
        //second block
        //random_animal_1=1+rand()%10;
        int_fake_no[1]=random_animal_1;
        true_pos[1]=1;
        sprt_container_active_2->setTag(int_fake_no[1]);
        sprt_container_active_2->setPosition(ccp(positions[1][0],positions[1][1]));
        scale=scale_value[int_true_no-1][random_animal_1];
        this->sprites_display(random_animal_1,position_x[1],position_y[1],scale);
        
        //third block
        //random_animal_2=1+rand()%10;
        int_fake_no[2]=random_animal_2;
        true_pos[2]=2;
        sprt_container_active_3->setTag(int_fake_no[2]);
        sprt_container_active_3->setPosition(ccp(positions[2][0],positions[2][1]));
        scale=scale_value[int_true_no-1][random_animal_2];
        this->sprites_display(random_animal_2,position_x[2],position_y[2],scale);
    }
    else if (trueposition==2)
    {
        //second block
        //random_animal_1=1+rand()%10;
        int_fake_no[0]=random_animal_1;
        true_pos[1]=0;
        sprt_container_active_2->setTag(int_fake_no[0]);
        sprt_container_active_2->setPosition(ccp(positions[0][0],positions[0][1]));
        scale=scale_value[int_true_no-1][random_animal_1];
        this->sprites_display(random_animal_1,position_x[0],position_y[0],scale);

        //third block
        //random_animal_2=1+rand()%10;
        int_fake_no[2]=random_animal_2;
         true_pos[2]=2;
        sprt_container_active_3->setTag(int_fake_no[2]);
        sprt_container_active_3->setPosition(ccp(positions[2][0],positions[2][1]));
        scale=scale_value[int_true_no-1][random_animal_2];
        this->sprites_display(random_animal_2,position_x[2],position_y[2],scale);
    }
    else if (trueposition==3)
    {
        //second block
        //random_animal_1=1+rand()%10;
        int_fake_no[0]=random_animal_1;
        true_pos[1]=0;
        sprt_container_active_2->setTag(int_fake_no[0]);
        sprt_container_active_2->setPosition(ccp(positions[0][0],positions[0][1]));
        scale=scale_value[int_true_no-1][random_animal_1];
        this->sprites_display(random_animal_1,position_x[0],position_y[0],scale);
        //third block
        //random_animal_2=1+rand()%10;
        int_fake_no[1]=random_animal_2;
         true_pos[2]=1;
        sprt_container_active_3->setTag(int_fake_no[1]);
        sprt_container_active_3->setPosition(ccp(positions[1][0],positions[1][1]));
        scale=scale_value[int_true_no-1][random_animal_2];
        this->sprites_display(random_animal_2,position_x[1],position_y[1],scale);
    }
    sprt_container_1->setPosition(sprt_container_active_1->getPosition());
    sprt_container_2->setPosition(sprt_container_active_2->getPosition());
    sprt_container_3->setPosition(sprt_container_active_3->getPosition());
    //CCLog("\nposition one:%d\nposition two:%d\nposition three:%d\ntrue position:%d\n",int_fake_no[0],int_fake_no[1],int_fake_no[2],int_true_no);
    CCString *str_question=CCString::createWithFormat("Which picture has %d %s?",random_no,(object_name[random_no-1]).c_str());
    lbl_question->setString(str_question->getCString());
}
int CountNumberLayer::get_sprite_frames(int current_number)
{
    int kMax;
    switch (current_number)
    {
        case 1:
             kMax=7;
            break;
        case 2:
             kMax=7;
            break;
        case 3:
            kMax=4;
            break;
        case 4:
             kMax=13;
            break;
        case 5:
           kMax=2;
            break;
        case 6:
            kMax=19;
            break;
        case 7:
           kMax=10;
            break;
        case 8:
            kMax=5;
            break;
        case 9:
            kMax=9;
            break;
        case 10:
            kMax=5;
            break;
        default:
            kMax=2;
            break;
    }
    return kMax;
}
CCPointArray * CountNumberLayer::assingposition_sprites(int current_number)
{
    CCPointArray *position=CCPointArray::create(10);
    if (current_number==1)
    {
        position->addControlPoint(CCPointMake(808.9*(size.width/1024), 322.9*(size.height/768)));
    }
    else if (current_number==2)
    {
        position->addControlPoint(CCPointMake(752.5*(size.width/1024), 321.7*(size.height/768)));
        position->addControlPoint(CCPointMake(865.3*(size.width/1024), 321.7*(size.height/768)));
    }
    else if (current_number==3)
    {
        //scale panda 0.4
        position->addControlPoint(CCPointMake(853.3*(size.width/1024), 278.5*(size.height/768)));
        position->addControlPoint(CCPointMake(760.9*(size.width/1024), 278.5*(size.height/768)));
        position->addControlPoint(CCPointMake(806.5*(size.width/1024), 385.3*(size.height/768)));
    }
    else if(current_number==4)
    {
       
        //panda scale 0.30
        position->addControlPoint(CCPointMake(765.7*(size.width/1024), 361.3*(size.height/768)));
        position->addControlPoint(CCPointMake(765.7*(size.width/1024), 274.9*(size.height/768)));
        position->addControlPoint(CCPointMake(855.7*(size.width/1024), 361.3*(size.height/768)));
        position->addControlPoint(CCPointMake(855.7*(size.width/1024), 274.9*(size.height/768)));
    }
    else if(current_number==5)
    {
      
        //panda scale 0.30
        position->addControlPoint(CCPointMake(888.1*(size.width/1024), 316.5*(size.height/768)));
        position->addControlPoint(CCPointMake(733.3*(size.width/1024), 316.5*(size.height/768)));
        position->addControlPoint(CCPointMake(813.3*(size.width/1024), 316.5*(size.height/768)));
        position->addControlPoint(CCPointMake(813.3*(size.width/1024), 393.7*(size.height/768)));
        position->addControlPoint(CCPointMake(813.3*(size.width/1024), 240.1*(size.height/768)));
    }
    else if(current_number==6)
    {
       
        //panda scale 0.25
        position->addControlPoint(CCPointMake(740.7*(size.width/1024), 363.2*(size.height/768)));
        position->addControlPoint(CCPointMake(810.6*(size.width/1024), 363.2*(size.height/768)));
        position->addControlPoint(CCPointMake(877.5*(size.width/1024), 363.2*(size.height/768)));
        position->addControlPoint(CCPointMake(740.7*(size.width/1024), 274.6*(size.height/768)));
        position->addControlPoint(CCPointMake(810.6*(size.width/1024), 274.6*(size.height/768)));
        position->addControlPoint(CCPointMake(877.5*(size.width/1024), 274.6*(size.height/768)));
    }
    else if(current_number==7)
    {
       
        //panda scale 0.25
        position->addControlPoint(CCPointMake(807.9*(size.width/1024), 399.2*(size.height/768)));
        position->addControlPoint(CCPointMake(807.9*(size.width/1024), 320.2*(size.height/768)));
        position->addControlPoint(CCPointMake(807.9*(size.width/1024), 236.6*(size.height/768)));
        position->addControlPoint(CCPointMake(740.7*(size.width/1024), 363.2*(size.height/768)));
        position->addControlPoint(CCPointMake(877.5*(size.width/1024), 363.2*(size.height/768)));
        position->addControlPoint(CCPointMake(740.7*(size.width/1024), 274.6*(size.height/768)));
        position->addControlPoint(CCPointMake(877.5*(size.width/1024), 274.6*(size.height/768)));
    }
    else if(current_number==8)
    {
        position->addControlPoint(CCPointMake(722.0*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(779.0*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(835.5*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(894.0*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(722.0*(size.width/1024), 283.1*(size.height/768)));
        position->addControlPoint(CCPointMake(779.0*(size.width/1024), 283.1*(size.height/768)));
        position->addControlPoint(CCPointMake(835.5*(size.width/1024), 283.1*(size.height/768)));
        position->addControlPoint(CCPointMake(894.0*(size.width/1024), 283.1*(size.height/768)));
    }
    else if(current_number==9)
    {
        position->addControlPoint(CCPointMake(806.0*(size.width/1024), 407.1*(size.height/768)));
        position->addControlPoint(CCPointMake(722.0*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(779.0*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(835.5*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(894.0*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(722.0*(size.width/1024), 283.1*(size.height/768)));
        position->addControlPoint(CCPointMake(779.0*(size.width/1024), 283.1*(size.height/768)));
        position->addControlPoint(CCPointMake(835.5*(size.width/1024), 283.1*(size.height/768)));
        position->addControlPoint(CCPointMake(894.0*(size.width/1024), 283.1*(size.height/768)));
    }
    else if(current_number==10)
    {
        position->addControlPoint(CCPointMake(806.5*(size.width/1024),407.1*(size.height/768)));
        position->addControlPoint(CCPointMake(722.0*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(779.0*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(835.5*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(894.0*(size.width/1024), 348.1*(size.height/768)));
        position->addControlPoint(CCPointMake(722.0*(size.width/1024), 283.1*(size.height/768)));
        position->addControlPoint(CCPointMake(779.0*(size.width/1024), 283.1*(size.height/768)));
        position->addControlPoint(CCPointMake(835.5*(size.width/1024), 283.1*(size.height/768)));
        position->addControlPoint(CCPointMake(894.0*(size.width/1024), 283.1*(size.height/768)));
        position->addControlPoint(CCPointMake(806.5*(size.width/1024), 227.1*(size.height/768)));
    }
    return position;
}
void CountNumberLayer::load_sprite_animation(int current_number)
{
    int frame_count=this->get_sprite_frames(current_number);
    CCString *str_number=CCString::createWithFormat("anim_%d.png",current_number);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CCString::createWithFormat("anim_%d.plist",current_number)->getCString());
    _anim_batch_sprite= CCSpriteBatchNode::create(str_number->getCString());
    this->addChild(_anim_batch_sprite, 5, 100);
    animFrames = CCArray::createWithCapacity(frame_count);
    for(int k = 1; k <=frame_count; k++)
    {
        CCString *str_frame=CCString::createWithFormat("anim_%d/%d.png",current_number,k);
        animFrames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str_frame->getCString()));
        str_frame->release();
    }
}
void CountNumberLayer::sprites_display(int current_number,float x_offset,float y_offset,float scale)
{
    CCPointArray *arr_sprt_position=this->assingposition_sprites(current_number);
    for (int i=0; i<current_number;i++)
    {
        CCSprite_animation *sprt_animation=CCSprite_animation::create(CCString::createWithFormat("anim_%d/%d.png",int_true_no,1)->getCString());
        char_anim_act=CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(animFrames, 0.3f)));
        sprt_animation->setVisible(false);
        #warning scale...
        sprt_animation->setScale(scale);
        CCPoint p=arr_sprt_position->getControlPointAtIndex(i);
        #warning offset x position....
         p.x=(p.x-x_offset);
         p.y=(p.y-y_offset);
        sprt_animation->setPosition(p);
        //sprt_animation->setColor(ccRED);
        sprt_animation->runAction(char_anim_act);
        _anim_batch_sprite->addChild(sprt_animation, 1, 2000+i);
        arr_char_anim->addObject(sprt_animation);
    }
}
void CountNumberLayer::play_next_number()
{
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    question_no=question_no+1;
    if (question_no==TOTALQUE)
    {
        //Redirect to Leader Board Class For Arranging data....
        CCNode *leaderboard=this->getChildByTag(333);
        this->reorderChild(leaderboard, 2222);
        leaderboard->setVisible(true);
         mAnimationManager->runAnimationsForSequenceNamedTweenDuration("LeaderBoardTimeLine", 0.0f);
    }
    else
    {
        //check the count  of the question for max number...
        sprt_container_active_1->setVisible(false);
        sprt_container_active_2->setVisible(false);
        sprt_container_active_3->setVisible(false);
        sprt_container_active_1->setColor(ccWHITE);
        sprt_container_active_2->setColor(ccWHITE);
        sprt_container_active_3->setColor(ccWHITE);
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
        _anim_batch_sprite->removeAllChildrenWithCleanup(true);
        this->create_new_quiz();
        this->star_gain_right->setPercentage(0);
        this->callback_show_objects(NULL);
    }
}
void CountNumberLayer::validate_current_number(CCNode* pSender, void* data)
{
    #warning give star first and than move to next question...
    int  pressed_no=(int )data;
    pSender->setVisible(false);
    CCNode *sprite=this->getChildByTag(pressed_no);
    sprite->setVisible(true);  
    if(pressed_no==int_true_no)
    {
       ////CCLog("True answer");
       //give additional stars...
        SimpleAudioEngine::sharedEngine()->playEffect("sound/clapping.mp3");
      this->provide_star(timer_progress->getPercentage());
       sprite->runAction(CCSequence::create(CCTintTo::create(0.01,255,255,255),CCBlink::create(0.5,2),CCDelayTime::create(2),CCCallFunc::create(this, callfunc_selector(CountNumberLayer::play_next_number)),NULL));
    }
    else
    {
        ////CCLog("wrong answer");
        SimpleAudioEngine::sharedEngine()->playEffect("sound/oh_no.mp3");
        sprite->runAction(CCSequence::create(CCTintTo::create(0.01,255,23,44),CCBlink::create(0.5,2),CCDelayTime::create(1.50),CCCallFuncND::create(this, callfuncND_selector(CountNumberLayer::display_true_picture), (void*) int_fake_no[ true_pos[0]]),NULL));
    }
    //int correct_no,int pressed_no
}
void CountNumberLayer::display_true_picture(CCNode* pSender, void* data)
{
    int  pressed_no=(int )data;
    pSender->setVisible(false);
    CCNode *sprite=this->getChildByTag(pressed_no);
    sprite->setVisible(true);
    sprite->runAction(CCSequence::create(CCTintTo::create(0.01,255,255,255),CCBlink::create(0.5,2),CCDelayTime::create(2),CCCallFunc::create(this, callfunc_selector(CountNumberLayer::play_next_number)),NULL));
    
}
#pragma mark -
#pragma mark LeaderBoard Stuff
#pragma mark -
//Track  leaderBoard(Load current leader board at begning from plist file and process at the end of play accordingly)
//Leader board will have name of the user+average stars+profile Image
void CountNumberLayer::leaderboard_manipulate()
{
    //CCLog("leaderboard_manipulate");
}
CCArray*  CountNumberLayer::get_leaderboard_list()
{
    CCArray *arr_users;
    std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fullPath = writablePath_temp + "LeaderBoard.plist";
    if (CCFileUtils::sharedFileUtils()->isFileExist(fullPath))
    {
        CCDictionary *dict=CCDictionary::createWithContentsOfFile(fullPath.c_str());
        dict->retain();
        arr_users=(CCArray*)dict->objectForKey("array");
    }
    else
    {
        arr_users=CCArray::createWithContentsOfFileThreadSafe("Published-iOS/LeaderBoard.plist");
        CCDictionary *root = CCDictionary::create();
        root->setObject(arr_users, "array");
        if(root->writeToFile(fullPath.c_str()))
        {
            //CCLog("see the plist file at %s", fullPath.c_str());
        }
        else
        {
            //CCLog("write plist file failed");
        }
        //CCLog("external file path = %s", fullPath.c_str());
    }
    return arr_users;
}
void CountNumberLayer::save_rank(CCDictionary *rank)
{
    leaderBoard_Stat->addObject(rank);
    CCDictionary *root = CCDictionary::create();
    root->setObject(leaderBoard_Stat, "array");
    std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fullPath = writablePath_temp + "LeaderBoard.plist";
    if(root->writeToFile(fullPath.c_str()))
    {}
        //CCLog("see the plist file at %s", fullPath.c_str());
   // else
        //CCLog("write plist file failed");
}
#pragma mark -
#pragma mark selector methods
#pragma mark -
void CountNumberLayer::onHomeClicked(cocos2d::CCObject * pSender)
{
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    Scene_common* pScene = new Scene_common();
    if (pScene)
    {
        pScene->ReplaceScene("homeLayer.ccbi","homeLayer", homeLayerLoader::loader(),this,true,callfuncO_selector(homeLayer::onDidSwipe));
        pScene->release();
    }
}
//make visible all the animating sprites...
void CountNumberLayer::callback_show_objects(CCNode* sender)
{
    is_running=true;
    sprt_container_1->setPosition(sprt_container_active_1->getPosition());
    sprt_container_2->setPosition(sprt_container_active_2->getPosition());
    sprt_container_3->setPosition(sprt_container_active_3->getPosition());
    CCObject *obj_sprt_anm=NULL;
    CCARRAY_FOREACH(_anim_batch_sprite->getChildren(), obj_sprt_anm)
    {
        CCSprite_animation *_objetc_sprt=(CCSprite_animation*)obj_sprt_anm;
        float s=_objetc_sprt->getScale();
        _objetc_sprt->setScale(0);
        _objetc_sprt->runAction(CCSequence::create(CCToggleVisibility::create(),CCEaseBounceOut::create(CCScaleTo::create(0.8,s)),NULL));
        //_objetc_sprt->setVisible(true);
    }
    //animate timer progressbar...
    this->start_timer();
}
void CountNumberLayer::leaderboard_Callback(CCNode* sender)
{
    //CCLog("LeaderBoard Callback");
    CCObject *obj_int=NULL;
    int aggrigate_star=0;
    CCARRAY_FOREACH(arr_strs_crnt, obj_int)
    {
        CCInteger *int_Str=(CCInteger*)obj_int;
        aggrigate_star=aggrigate_star+int_Str->getValue();
    }
    //total no of question will be 15...
    aggrigate_star=aggrigate_star/(TOTALQUE-1);
    cocos2d::CCDictionary *rank=CCDictionary::create();
    rank->retain();
    if (user_profile_path->isSingleReference())
    {
        rank->setObject(user_profile_path, "user_profile");
    }
    else
    {
        rank->setObject(CCString::createWithFormat("profilepic_bg.png"), "user_profile");
    }
    rank->setObject(user_name, "user_name");
    rank->setObject(CCString::createWithFormat("%d",aggrigate_star), "star_earned");
    //CCLog("%s",user_profile_path->getCString());
    //CCLog("%s",rank->valueForKey("user_profile")->getCString());
    //CCLog("%s",rank->valueForKey("star_earned")->getCString());
    //CCLog("%s",rank->valueForKey("user_name")->getCString());
    _leader_board->save_rank(rank);
}
#pragma mark -
#pragma mark animationManaget
#pragma mark -
void CountNumberLayer::setAnimationManager(cocos2d::extension::CCBAnimationManager *animationManager)
{
    if (animationManager)
    {
        CC_SAFE_RELEASE_NULL(mAnimationManager);
        mAnimationManager = animationManager;
        CC_SAFE_RETAIN(animationManager);
    }
}
#pragma mark -
#pragma mark touch delegate
#pragma mark -
void CountNumberLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCSetIterator it;
    CCTouch* touch;
    //here we will have logic if user click correct count  it will animate backgrond of the same
    //else it will  do some animation on clicked backgrond and than it will blink on the correct answer...
    //after animation it will pass to the next count play
    for( it = pTouches->begin(); it != pTouches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        if (is_running==true)
        {
            if (sprt_container_1->boundingBox().containsPoint(touch->getLocation()))
            {
                is_running=false;
                timer_progress->pauseSchedulerAndActions();
                CCProgressTo *to1 = CCProgressTo::create(1, 100);
                CCProgressTimer *left = CCProgressTimer::create(sprt_container_active_1);
                left->setType( kCCProgressTimerTypeRadial );
                left->setScale(1.1);
                this->addChild(left, 2, 401);
                left->setPosition(sprt_container_active_1->getPosition());
                CCFiniteTimeAction* action = CCSequence::create(to1,CCCallFuncND::create(this, callfuncND_selector(CountNumberLayer::validate_current_number), (void*) int_fake_no[ true_pos[0]]),NULL);
                left->runAction(action);
            }
            else if(sprt_container_2->boundingBox().containsPoint(touch->getLocation()))
            {
                is_running=false;
                timer_progress->pauseSchedulerAndActions();
                CCProgressTo *to1 = CCProgressTo::create(1, 100);
                CCProgressTimer *left = CCProgressTimer::create(sprt_container_active_2);
                left->setType( kCCProgressTimerTypeRadial );
                left->setScale(1.1);
                this->addChild(left, 2, 402);
                left->setPosition(sprt_container_active_2->getPosition());
                CCFiniteTimeAction* action = CCSequence::create(to1,CCCallFuncND::create(this, callfuncND_selector(CountNumberLayer::validate_current_number), (void*)int_fake_no[true_pos[1]]),NULL);
                left->runAction(action);
            }
            else if(sprt_container_3->boundingBox().containsPoint(touch->getLocation()))
            {
                is_running=false;
                timer_progress->pauseSchedulerAndActions();
                CCProgressTo *to1 = CCProgressTo::create(1, 100);
                 CCProgressTimer *left = CCProgressTimer::create(sprt_container_active_3);
                left->setType( kCCProgressTimerTypeRadial );
                left->setScale(1.1);
                this->addChild(left, 2, 403);
                left->setPosition(sprt_container_active_3->getPosition());
                CCFiniteTimeAction* action = CCSequence::create(to1,CCCallFuncND::create(this, callfuncND_selector(CountNumberLayer::validate_current_number), (void*)int_fake_no[true_pos[2]]),NULL);
                left->runAction(action);
            }
        }
    }
}
void CountNumberLayer::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    //CCLog("touch cancelled");
}
#pragma mark -
#pragma mark CCB Delegate
#pragma mark -
void CountNumberLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    ////CCLog("on loaded called");
}
SEL_CallFuncN CountNumberLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "leaderboard_Callback", CountNumberLayer::leaderboard_Callback);
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "callback_show_objects", CountNumberLayer::callback_show_objects);
    return NULL;
}
SEL_MenuHandler CountNumberLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onHomeClicked", CountNumberLayer::onHomeClicked);
 // CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSettingClicked", CountNumberLayer::onSettingClicked);
//  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSoundClicked", CountNumberLayer::onSoundClicked);
//  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onMusicClicked", CountNumberLayer::onMusicClicked);
    return NULL;
}
SEL_CCControlHandler CountNumberLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool CountNumberLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    bool bRet = false;
    return bRet;
}
bool CountNumberLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "_leader_board", LeaderBoardNode *, this->_leader_board);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menu_toggle_sound_fx", CCMenuItemImage *, this->menu_toggle_sound_fx);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menu_toggle_bg_music", CCMenuItemImage *, this->menu_toggle_bg_music);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mAnimationManager", CCBAnimationManager *, this->mAnimationManager);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_question", CCLabelTTF*, lbl_question);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprt_container_1", CCSprite*, sprt_container_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprt_container_2", CCSprite*, sprt_container_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprt_container_3", CCSprite*, sprt_container_3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprt_container_active_1", CCSprite*, sprt_container_active_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprt_container_active_2", CCSprite*, sprt_container_active_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprt_container_active_3", CCSprite*, sprt_container_active_3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_welcome_text", CCLabelTTF*, lbl_welcome_text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_user_name", CCLabelTTF*, lbl_user_name);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_start_pos", CCLabelTTF*, lbl_start_pos);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_precent", CCLabelTTF*, lbl_precent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lbl_end_pos", CCLabelTTF*, lbl_end_pos);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprt_progress_timer", CCSprite*, sprt_progress_timer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "user_panel", CCNode*, user_panel);
    return false;
}
void CountNumberLayer::onDidSwipe(CCObject* pSender)
{
    CCSwipe * swipe = (CCSwipe*)pSender;
    
    CCPoint nodePoint = sprt_container_1->convertToNodeSpace(swipe->location);
    CCRect rect = CCRectMake(0, 0, sprt_container_active_1->getContentSize().width, sprt_container_active_1->getContentSize().height);

    CCPoint nodePoint1 = sprt_container_2->convertToNodeSpace(swipe->location);
    CCRect rect1 = CCRectMake(0, 0, sprt_container_2->getContentSize().width, sprt_container_2->getContentSize().height);

    CCPoint nodePoint2 = sprt_container_3->convertToNodeSpace(swipe->location);
    CCRect rect2 = CCRectMake(0, 0, sprt_container_3->getContentSize().width, sprt_container_3->getContentSize().height);

    if(rect.containsPoint(nodePoint)||rect1.containsPoint(nodePoint1)||rect2.containsPoint(nodePoint2))
    {
        CCLOG("Return");
        return;
    }


    if (swipe->direction==kSwipeGestureRecognizerDirectionRight)
    {
        CCLOG("DirectionRight");
         SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
        CCDirector::sharedDirector()->popScene();
    }
    else if (swipe->direction==kSwipeGestureRecognizerDirectionLeft)
    {
        CCLOG("DirectionLeft");
    }
    else if (swipe->direction==kSwipeGestureRecognizerDirectionUp)
    {
        CCLOG("DirectionUp");
    }
    else  if (swipe->direction==kSwipeGestureRecognizerDirectionDown)
    {
         SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
         CCDirector::sharedDirector()->popScene();
        CCLOG("DirectionDown");
    }
}