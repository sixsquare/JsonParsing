//
//  HelloCocosBuilderLayer.h
//  Demo
//
//  Created by Neil D on 20/05/13.
//
//

#ifndef __Demo__LeaderBoardNode__
#define __Demo__LeaderBoardNode__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Scene_common.h"
#include "CCSprite+animation.h"
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
class LeaderBoardNode: public cocos2d::CCNode
, public cocos2d::extension::CCBSelectorResolver 
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener,public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{

public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(LeaderBoardNode, create);
    LeaderBoardNode();
    virtual ~LeaderBoardNode();
    virtual void onEnter();
  
    //CCB methods...
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);

    //object animation...
    CCSize size;

    //table view...
    CCTableView* tableView;
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    //initial setup
    void set_initial_data();
    void set_last_score_profilepic(CCDictionary *dict);
    //Track  leaderBoard
    void leaderboard_manipulate();
    CCArray *leaderBoard_Stat;
    CCArray*   get_leaderboard_list();
    void updateleaderboard(CCDictionary *rank);
    void save_rank(CCDictionary *rank);
    //bool sortIndexAsc(CCObject *a, CCObject *b);
    void sortArray(CCArray *array);
    void onHomeClicked(cocos2d::CCObject * pSender);
    void onRestartClicked(cocos2d::CCObject * pSender);
    void onDidSwipe(CCObject* pSender);
    
    // animation manager and Callbak events
    void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);
    void callback_show_objects(CCNode* sender);

    CCLabelTTF *lblCurrent_score;
    private:
        cocos2d::extension::CCBAnimationManager *mAnimationManager;
};
#endif /* defined(__Demo__CountNumberLayer__) */
