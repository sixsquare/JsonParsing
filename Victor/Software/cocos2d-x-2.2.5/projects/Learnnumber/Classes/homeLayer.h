//
//  HelloCocosBuilderLayer.h
//  Demo
//
//  Created by Neil D on 20/05/13.
//
//

#ifndef __Demo__HelloCocosBuilderLayer__
#define __Demo__HelloCocosBuilderLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Scene_common.h"

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
class homeLayer: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver 
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
//    , public ChartboostXDelegate
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(homeLayer, create);
    homeLayer();
    virtual ~homeLayer();
    virtual void onEnter();
    //CCB methods...
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);

    //all digits...
    CCMenuItemImage *menuitem_1;
    CCMenuItemImage *menuitem_2;
    CCMenuItemImage *menuitem_3;
    CCMenuItemImage *menuitem_4;
    CCMenuItemImage *menuitem_5;
    CCMenuItemImage *menuitem_6;
    CCMenuItemImage *menuitem_7;
    CCMenuItemImage *menuitem_8;
    CCMenuItemImage *menuitem_9;
    CCMenuItemImage *menuitem_10;

    //Check game click
    int game_click;
    bool goes_next;
    void onDidSwipe(CCObject* pSender);
    //menu selector methods
    void onNumberClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    //animationcompleted
    void animationcompleted(CCNode* sender);
    void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);
    void animate_seq();
    void display_pop();
    
    // ChartboostXDelegate method
//    bool shouldDisplayInterstitial(const char* location);
//    void didCacheInterstitial(const char* location);
//    void didFailToLoadInterstitial(const char* location);
//    void didCloseInterstitial(const char* location);
//    void didClickInterstitial(const char* location);
//    void continueGameLogic();

private:
    cocos2d::extension::CCBAnimationManager *mAnimationManager;
//private:
//    bool m_interstitialShowed;
//    bool m_gameLogicContinued;

};
#endif /* defined(__Demo__HelloCocosBuilderLayer__) */
