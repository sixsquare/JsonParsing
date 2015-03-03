//
//  HelloCocosBuilderLayer.h
//  Demo
//
//  Created by Neil D on 20/05/13.
//
//
#ifndef __Demo__SettingLayer__
#define __Demo__SettingLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Scene_common.h"
#include  "ImagePicker.h"
#include "ObjCCalls.h"
#include "CCMask.h"
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
class SettingLayer: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
,public ImagePickerDelegate,public CCEditBoxDelegate
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SettingLayer, create);
    SettingLayer();
    virtual ~SettingLayer();
    virtual void onEnter();
    void initial_setting();
    
    //CCB methods...
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
    
    
    void valueChanged(CCObject *sender, CCControlEvent controlEvent);
    void onDidSwipe(cocos2d::CCObject * pSender);
    void oncrossClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onProfilePicClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onRestorepurchasedClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onRemoveAdsClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onYesAdsremoveClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onNoAdsremoveClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    float isbg_on;
    float issoundFX_on;
    void add_on_Player_list(CCDictionary *dict , bool is_updating);
    CCArray*  get_userlist();
    void set_user_profile();
    virtual void didFinishPickingWithResult(bool result);
    
    
    void set_textfield();
    void set_slider();
    
    //Edit user profile name
    void create_text_field();
    virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);
    cocos2d::extension::CCEditBox* m_pEditName;
    cocos2d::extension::CCEditBox* m_pEditpassword;
    cocos2d::extension::CCEditBox* m_pEditpassword_enter;
    
    void update_for_iap(CCTime dt);
    bool is_authenticated;
    
    CCLabelTTF *lbl_profile;
    CCLabelTTF *lbl_instract_1;
    CCLabelTTF *lbl_instract_2;
    CCLabelTTF *lbl_about;
    CCLabelTTF *lbl_password;
    CCLabelTTF *lbl_sound;
    CCLabelTTF *lbl_vibration;
    CCSprite *sprt_text_bg;
    CCLayerColor *layer_pwd;
    CCLayerColor *layer_in_app;
    CCMenuItemImage *itm_remove_ads;
    CCSprite *sprt_img_profile;
    CCMask* masked_profile;

    //login password..
    CCScale9Sprite *sprt_password;
    CCLayerColor *layer_seprator;
    CCMenuItemImage *remove_ads;
    CCMenuItemImage *restore_purchase;
    CCLabelTTF  *lbladvertisement;
};
#endif