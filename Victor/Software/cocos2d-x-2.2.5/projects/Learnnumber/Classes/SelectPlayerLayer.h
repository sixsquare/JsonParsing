//
//  HelloCocosBuilderLayer.h
//  Demo
//
//  Created by Neil D on 20/05/13.

#ifndef __Demo__SelectPlayerLayer__
#define __Demo__SelectPlayerLayer__
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ImagePicker.h"
#include "Scene_common.h"
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
USING_NS_CC;
USING_NS_CC_EXT;
class SelectPlayerLayer: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver 
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener,public CCEditBoxDelegate,public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate,public ImagePickerDelegate
{
   public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SelectPlayerLayer, create);
    SelectPlayerLayer();
    virtual ~SelectPlayerLayer();
    virtual void onEnter();
    
    void onDidSwipe(CCObject* pSender);
    void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);
    void onBackClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onHomeClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onnew_user_clicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onProfilePicClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    virtual void didFinishPickingWithResult(bool result);
    CCLabelTTF *lbl_title;
    CCLabelTTF *lbl_addnew_user;
    CCLabelTTF *lbl_instr_1;
    CCLabelTTF *lbl_instr_2;
    CCLabelTTF *lbl_select_from;
    CCSprite *bg_text_field;
    
    //information screen
    bool is_info_showing;
    void callback_info_show(CCNode* sender);
    void callback_info_hide(CCNode* sender);
    
    CCMenuItemImage *menu_img_profile;
    CCSprite *sprt_img_profile;
    CCMask* masked;
    CCSprite *sprt_image_picker;
    
    void set_textfield();
    void add_user_tableview();
    void create_text_field();
    void play_with_user(CCDictionary *user);
    //select user from text field...
    void on_user_clicked(cocos2d::CCObject * pSender);
    CCString *str_new_user;
    CCSprite *str_new_user_profile;
    CCSize winSize;

    CCArray* get_userlist();
    void save_user(CCDictionary *user);

    // table view delegates....
    CCArray *existing_users;
    CCTableView* tableView;
    void display_pop();
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

    //text field delegates...
    virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);
    cocos2d::extension::CCEditBox* m_pEditName;

    //CCB methods...
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
    private:
        cocos2d::extension::CCBAnimationManager *mAnimationManager;
};
#endif 

