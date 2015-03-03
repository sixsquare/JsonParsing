//
//  GamePlayLayer.h
//  LearnNumber
//
//  Created by Neil D on 14/06/13.
#ifndef __Fishing__GamePlayLayer__
#define __Fishing__GamePlayLayer__
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../../external/Box2D/Box2D.h"
#include "CCSprite+animation.h"
#include "CCMask.h"
USING_NS_CC;
USING_NS_CC_EXT;
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
class GamePlayLayer: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GamePlayLayer, create);
    //Update method
        void update(CCTime dt);
    //read data from plist file for particular charactor...'
       int current_play_level;
       void play_for_level(int level);
       void get_char_detail();
       void change_segment(int segment);
       void segment_change();
       void draw_segment_points();
       void get_next_destination(int currentdestination);
    //    CCSprite_animation *sprt_animation_sparrow;
    //    void sparrow_animate();
    //find sortest distance...
        CCPoint shortest_distance(CCPoint current_location, CCPoint start_location,CCPoint end_location);
    //lines we have passed and lines we need to pass will be stacked accordingly in respective array...
        CCPointArray*    points_passed;
        CCPointArray *   points_remain;
    //points on one entire segment
        CCPointArray * points_segment;
        bool is_drag_started;
        int current_number;
        int total_segments;
        int current_segment;
        int current_destination;
        int total_segment_point;
        bool is_completed;
    //start and end point of the line ...
        CCPoint start_pnt;
        CCPoint end_pnt;
        CCPoint pv_srt_pnt;
        CCPoint cr_srt_pnt;
        CCSprite *sprite_arrow;
    
        CCPoint location_from;
    
    //alphabet image
        CCSprite *sprite_latter;
        CCSprite *sprite_mask;
    
        CCLabelTTF *lbl_header;
        void remove_all_arrow();
        const char* getnumberword(int currentpos);
        //        CCMenuItemImage *next;
        //        CCMenuItemImage *prev;
        //    //Accessory items
        //        CCMenu *menuall;
        //        CCMenuItemImage *sound;
        //        CCMenuItemImage *home;
    

    //current touch
        CCTouch *touch;
        CCDrawNode *draw_segment;
        CCSize size;
        void loading_assets();

    //Object Animation
        void play_animation();
        //animate sprite sheet for char..
        CCRepeatForever *char_anim_act;
        CCSpriteBatchNode *_anim_batch_sprite;
        CCArray *arr_char_anim;
        CCArray* animFrames;
        void next_level();
        void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);
    
    //call back methods...
        void show_char_anim(CCNode* sender);
        void complete_char_anim(CCNode* sender);
        void next_level_play(CCNode* sender);
        void start_animating(CCNode* sender);

    //animate arrow guide...
        void remove_current_arrow();
        void animate_path(CCNode* sender);
        void animate(CCTime dt);
        float y_pos;
        int current_animating;
    
    //sound instances
        unsigned int n_twit_sound_id;
        unsigned int m_nSoundId;
        int int_play_no;
        void play_number_sound(float dt);
        float m_fInterval;
        unsigned int m_sound_effect;
    
    //menu selector methods
    
    void onDidSwipe(CCObject* pSender);
    void onHomeClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onNextClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onPreviousClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
   // void onSettingClicked(cocos2d::CCObject * pSender);
    
    

    //Touch Delegates
    //inherited from super class...
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

    //CCB methods...
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
    
    GamePlayLayer();
    virtual ~GamePlayLayer();
    virtual void onEnter();
    virtual void onExit();
    virtual void draw();

//from box2d mask demo
    void addRectangleBetweenPointsToBody(b2Body* body, cocos2d::CCPoint start, cocos2d::CCPoint end);
	cocos2d::CCRect getBodyRectangle(b2Body* body);
    void initPhysics();
    //void update(float dt);

private:
    cocos2d::extension::CCBAnimationManager *_mAnimationManager;
    //animated arrows...
    CCArray *items_to_delete;
    CCArray *arr_anim_arrow;
    
    
    //from box2d mask demo...
    b2World* world;
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
    
	cocos2d::CCRenderTexture *target_mask;
    cocos2d::CCSprite *brush;
    cocos2d::CCSprite *dragger;
    CCMask  *masked_profile;
	//GLESDebugDraw* debugDraw;
	cocos2d::CCPoint previousLocation;
	b2Body* currentPlatformBody;
	std::vector<cocos2d::CCPoint> plataformPoints;
};
#endif /* defined(__Fishing__GamePlayLayer__) */
