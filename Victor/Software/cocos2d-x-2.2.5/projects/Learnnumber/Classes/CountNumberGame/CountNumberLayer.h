//
//  HelloCocosBuilderLayer.h
//  Demo
//
//  Created by Neil D on 20/05/13.
//
//

#ifndef __Demo__CountNumberLayer__
#define __Demo__CountNumberLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Scene_common.h"
#include "CCSprite+animation.h"
#include "LeaderBoardNode.h"
/*
 * Note: for some pretty hard fucked up reason, the order of inheritance is important!
 * When CCLayer is the 'first' inherited object:
 * During runtime the method call to the (pure virtual) 'interfaces' fails jumping into a bogus method or just doing nothing:
 *  #0    0x000cf840 in non-virtual thunk to HelloCocos....
 *  #1    ....
 *
 * This thread describes the problem:
 * http://www.cocoabuilder.com/archive/xcode/265549-crash-in-virtual-method-call.html
 */
USING_NS_CC;
USING_NS_CC_EXT;
class CountNumberLayer: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver 
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CountNumberLayer, create);
    CountNumberLayer();
    virtual ~CountNumberLayer();
    virtual void onEnter();
  
    //CCB methods...
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);

    //Touch Delegates
    //inherited from super class...
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    
    CCSprite *sprt_container_1;
    CCSprite *sprt_container_2;
    CCSprite *sprt_container_3;
    
    CCSprite *    sprt_container_active_1;
    CCSprite *    sprt_container_active_2;
    CCSprite *    sprt_container_active_3;
    
    //object animation...
    CCSpriteBatchNode *_anim_batch_sprite;
    CCSize size;
    
    //cclabel question
    CCLabelTTF *lbl_question;
    CCLabelTTF *lbl_welcome_text;
    CCLabelTTF *lbl_user_name;
    
    //progress
    CCLabelTTF *lbl_start_pos;
    CCLabelTTF *lbl_precent;
    CCLabelTTF *lbl_end_pos;

    CCSprite *sprt_progress_timer;
    CCProgressTimer *timer_progress;
    CCProgressFromTo *progress_bar;
    void update_percentage(float dt);
    
    //star gained by user...
    CCProgressFromTo *star_gain;
    CCProgressTimer *star_gain_right;
    
    void start_timer();
    void provide_star(int percent);
    //initial setup
    void set_initial_data();
    
    //create random quiz...
    void create_new_quiz();
    void load_sprite_animation(int current_number);
    void sprites_display(int current_number,float x_offset,float y_offset,float scale);
    CCPointArray * assingposition_sprites(int current_number);
    int get_sprite_frames(int current_number);
   
    //sprite frames object...
    int int_true_no;
    int int_fake_no[3];
    int true_pos[3];
    CCArray* animFrames;
    CCArray* arr_char_anim;
    CCRepeatForever *char_anim_act;
    CCSprite_animation *sprt_animation_sparrow;

    //get user name and profile pic path
    CC_SYNTHESIZE(CCString*, user_name, user_name);
    CC_SYNTHESIZE(CCString*, user_profile_path, user_profile_path);
    CC_SYNTHESIZE(LeaderBoardNode *, _leader_board, _leader_board);
    bool is_running;
    bool is_progress_completed;

    CCArray *arr_strs_crnt;
    void add_star(int star);
    //Track  leaderBoard
    void leaderboard_manipulate();
    CCArray *leaderBoard_Stat;
    CCArray*   get_leaderboard_list();
    void save_rank(CCDictionary *rank);
    
    //question number
    int question_no;
    int prev_random;
    //play next Question..
    void play_next_number();
    void validate_current_number(CCNode* pSender, void* data);
    void display_true_picture(CCNode* pSender, void* data);

    //menu selector methods
    void onDidSwipe(CCObject* pSender);
    void onHomeClicked(cocos2d::CCObject * pSender);
   
    CCNode *user_panel;
    CCProgressFromTo  *setting;
    CCProgressFromTo  *setting_reverse;
    
    CCProgressTimer  *setting_progress;
    bool is_setting_visible;
    // animation manager and Callbak events
    void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);
    void callback_show_objects(CCNode* sender);
    void leaderboard_Callback(CCNode* sender);
    private:
        cocos2d::extension::CCBAnimationManager *mAnimationManager;
};
#endif /* defined(__Demo__CountNumberLayer__) */

