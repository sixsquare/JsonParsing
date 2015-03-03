//
//  HelloCocosBuilderLayer.h
//  Demo
//
//  Created by Neil D on 20/05/13.
//
//
#ifndef __Demo__SplashLayer__
#define __Demo__SplashLayer__
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Scene_common.h"
#include "../Classes/MainLayerLoader.h"
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
class SplashLayer: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver 
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SplashLayer, create);
        SplashLayer();
        virtual ~SplashLayer();
        virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
        virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
        virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
        virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
        virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
        virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
        virtual void onEnter();
    
        void onGameClicked(CCObject *pSender);
        void onSettingClicked(CCObject *pSender);
        void onPlayClicked(CCObject *pSender);
};
#endif 
