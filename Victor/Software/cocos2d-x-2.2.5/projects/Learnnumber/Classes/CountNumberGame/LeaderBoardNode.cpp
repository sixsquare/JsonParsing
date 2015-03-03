//  HelloCocosBuilderLayer.cpp
//  Demo
//  Created by Neil D on 20/05/13.

#include "LeaderBoardNode.h"
#include "homeLayerLoader.h"
#include "CountNumberLayer.h"
#include "CountNumberLayerLoader.h"
#include  "LeaderBoardNodeLoader.h"
#include  "SimpleAudioEngine.h"
#include "CustomTableViewCell.h"
#include "../Extension/CCGestureRecognizer/CCSwipeGestureRecognizer.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "CCMask.h"
#include "ObjCCalls.h"


using namespace std;
using namespace CocosDenshion;
USING_NS_CC;
USING_NS_CC_EXT;
//record for the scale values
LeaderBoardNode::LeaderBoardNode():lblCurrent_score(NULL)
{
    leaderBoard_Stat=CCArray::create();
    leaderBoard_Stat->retain();
}
LeaderBoardNode::~LeaderBoardNode()
{
    CC_SAFE_RELEASE(mAnimationManager);
}
void LeaderBoardNode::onEnter()
{
    ObjCCalls::full_screen_banner(true);
    size=CCDirector::sharedDirector()->getWinSize();
    //set initial data
    set_initial_data();
    //create new quiz...
    CCNode::onEnter();
}
void LeaderBoardNode::set_initial_data()
{

}
#pragma mark LeaderBoard Stuff
#pragma mark -
//Track  leaderBoard(Load current leader board at begning from plist file and process at the end of play accordingly)
//Leader board will have name of the user+average stars+profile Image
void LeaderBoardNode::leaderboard_manipulate()
{
    //CCLog("leaderboard_manipulate");
}
CCArray* LeaderBoardNode::get_leaderboard_list()
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
void LeaderBoardNode::save_rank(CCDictionary *rank)
{
    //display current score...
    lblCurrent_score->setString(rank->valueForKey("user_name")->getCString());
    lblCurrent_score->setFontSize(lblCurrent_score->getFontSize()*2);
    this->set_last_score_profilepic(rank);
    const CCString *str=rank->valueForKey("star_earned");
    CCProgressFromTo *progress_bar=CCProgressFromTo::create(2, 0, (str->intValue()*100)/5);
    progress_bar->retain();
    CCProgressTimer *timer_progress = CCProgressTimer::create(CCSprite::create("star_activ.png"));
    timer_progress->setType(kCCProgressTimerTypeBar);
    timer_progress->setMidpoint(ccp(0, 0));
    timer_progress->setBarChangeRate(ccp(1, 0));
    this->addChild(timer_progress);
    timer_progress->setPosition(this->getChildByTag(120)->getPosition());
    timer_progress->runAction(progress_bar);
    CCDictionary *dict_detail=CCDictionary::create();
    dict_detail->retain();
    CCLOG("First:::%s",rank->valueForKey("user_profile")->getCString());
    dict_detail->setObject(rank->objectForKey("user_name"), "user_name");
    dict_detail->setObject(rank->objectForKey("star_earned"), "star_earned");
    dict_detail->setObject(rank->objectForKey("user_profile"), "user_profile");
    
    
    size=CCDirector::sharedDirector()->getWinSize();
    CCSprite *sprt_bg=CCSprite::create("user_select_bg.png");
    tableView =CCTableView::create(this, CCSizeMake(sprt_bg->getContentSize().width*2,3*size.height/4));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(CCPointMake(size.width/2.5,sprt_bg->getContentSize().height));
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView);
    tableView->reloadData();

    this->updateleaderboard(dict_detail);
    tableView->reloadData();
}
void LeaderBoardNode::set_last_score_profilepic(CCDictionary *dict)
{
    //set  user profile picture settings
    std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fullPath = writablePath_temp + CCString::createWithFormat("%s",dict->valueForKey("user_profile")->getCString())->getCString();
    CCSprite *sprt=(CCSprite*)this->getChildByTag(2639);
    sprt->setVisible(true);
    if(dict->valueForKey("user_profile")->length()>0&&CCFileUtils::sharedFileUtils()->isFileExist(fullPath.c_str()))
    {
        CCSprite *sprt_img_profile=CCSprite::create("place holder.png");
        CCSprite *profile_pic=CCSprite::create(fullPath.c_str());
        profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        CCMask *masked = CCMask::create(sprt_img_profile , profile_pic);
        this->addChild(masked,11);
        //masked->setPosition(ccpAdd(sprt->getPosition(),ccp(-masked->getContentSize().width/2,0) ));
        masked->setPosition(sprt->getPosition());
        //masked->setAnchorPoint(ccp(1,0.5));
        masked->setRotation(180);
        masked->setScale(1.1);
        masked->setOpacity(0);
        masked->runAction(CCSequence::create(CCDelayTime::create(1.5),CCFadeIn::create(0.3),NULL));
    }
    else
    {
        CCSprite *sprt_img_profile=CCSprite::create("place holder.png");
        CCSprite *profile_pic=CCSprite::create("place holder.png");
        profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        sprt_img_profile->setPosition(sprt->getPosition());
        CCMask *masked = CCMask::create(sprt_img_profile , profile_pic);
        this->addChild(masked,11);
        masked->setAnchorPoint(ccp(0.5,0.5));
        masked->setPosition(sprt->getPosition());
        //masked->setPosition(ccpAdd(sprt->getPosition(),ccp(-masked->getContentSize().width/2,0) ));
        masked->setScale(1.1);
        masked->setOpacity(0);
        masked->runAction(CCSequence::create(CCDelayTime::create(1.5),CCFadeIn::create(0.3),NULL));
    }

}
void LeaderBoardNode::updateleaderboard(CCDictionary *rank)
{
    CCArray *arr=CCArray::createWithArray(this->get_leaderboard_list());
    arr->retain();
    arr->addObject(rank);

    //update leaderboard list
    CCDictionary *root = CCDictionary::create();
    root->setObject(arr, "array");
    std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string fullPath = writablePath_temp + "LeaderBoard.plist";
    if(root->writeToFile(fullPath.c_str()))
    {
    }
    CCArray *arr1=this->get_leaderboard_list();
    arr1->retain();
    this->sortArray(arr1);
    leaderBoard_Stat->addObjectsFromArray(arr1);
    return;
    int i1=0;
    if (arr1->count()<=9)
        i1=arr1->count()+1;
    else
        i1=10;
    for (int i=1; i<i1; i++)
    {
        CCLabelTTF *lbl=(CCLabelTTF*)this->getChildByTag(i+(i*10)+(i*100));
        lbl->setString("");
        lbl->setFontSize(35);
        lbl->setVisible(true);
        CCSprite *sprt=(CCSprite*)this->getChildByTag(i+(i*10)+(i*100)+(i*(1000)));
        sprt->setVisible(true);
        CCDictionary *dict=CCDictionary::create();
        dict->retain();
        dict=(CCDictionary*)arr1->objectAtIndex(i-1);
        CCProgressFromTo *progress_bar=CCProgressFromTo::create(2, 0, (dict->valueForKey("star_earned")->intValue()*100)/5);
        progress_bar->retain();
        CCProgressTimer *timer_progress = CCProgressTimer::create(CCSprite::create("star_activ.png"));
        timer_progress->setType(kCCProgressTimerTypeBar);
        timer_progress->setMidpoint(ccp(0, 0));
        timer_progress->setBarChangeRate(ccp(1, 0));
        timer_progress->setScale(sprt->getScale());
       this->addChild(timer_progress);
       timer_progress->setPosition(sprt->getPosition());
       timer_progress->runAction(progress_bar);
       lbl->setString(dict->valueForKey("user_name")->getCString());
        //set user profile picture settings
        std::string writablePath_temp = CCFileUtils::sharedFileUtils()->getWritablePath();
        std::string fullPath = writablePath_temp + CCString::createWithFormat("%s",dict->valueForKey("user_profile")->getCString())->getCString();
        CCLOG("last::%s",rank->valueForKey("user_profile")->getCString());
        if (dict->valueForKey("user_profile")->length()>0&&CCFileUtils::sharedFileUtils()->isFileExist(fullPath.c_str()))
        {
            CCSprite *sprt_img_profile=CCSprite::create("profilepic_bg.png");
            CCSprite *profile_pic=CCSprite::create(fullPath.c_str());
            profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
            CCMask *masked = CCMask::create(sprt_img_profile , profile_pic);
            this->addChild(masked,11);
            masked->setPosition(ccpAdd(sprt->getPosition(),ccp(-masked->getContentSize().width/2,0) ));
            masked->setAnchorPoint(ccp(0.1,0.7));
            masked->setRotation(180);
            masked->setScale(0.4);
            masked->setOpacity(0);
            masked->runAction(CCSequence::create(CCDelayTime::create(1.5),CCFadeIn::create(0.3),NULL));
        }
        else
        {
            CCSprite *sprt_img_profile=CCSprite::create("profilepic_bg.png");
            CCSprite *profile_pic=CCSprite::create("place holder.png");
            profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
            sprt_img_profile->setPosition(sprt->getPosition());
            CCMask *masked = CCMask::create(sprt_img_profile , profile_pic);
            this->addChild(masked,11);
            masked->setAnchorPoint(ccp(0.85,0.3));
            masked->setPosition(ccpAdd(sprt->getPosition(),ccp(-masked->getContentSize().width/2,0) ));
            masked->setScale(0.4);
            masked->setOpacity(0);
            masked->runAction(CCSequence::create(CCDelayTime::create(1.5),CCFadeIn::create(0.3),NULL));
        }
    }
}
bool static sortIndexAsc(CCObject *a, CCObject *b)
{
     CCDictionary *firstObject = static_cast< CCDictionary *>(a);
     CCDictionary *secondObject = static_cast< CCDictionary *>(b);
     int k=firstObject->valueForKey("star_earned")->intValue();
     int l=secondObject->valueForKey("star_earned")->intValue();
     return k>l;
}
void LeaderBoardNode::sortArray(CCArray *array)
{
    std::sort(array->data->arr,
              array->data->arr + array->data->num,
              sortIndexAsc);
    
}
#pragma mark -
#pragma mark tableview delegate
#pragma mark -
void LeaderBoardNode::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}
CCSize LeaderBoardNode::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
    CCSprite *sprt_bg=CCSprite::create("user_select_bg.png");
    return CCSizeMake(sprt_bg->getContentSize().width+20, sprt_bg->getContentSize().height*1.3);
}
CCTableViewCell* LeaderBoardNode::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CustomTableViewCell *cell = (CustomTableViewCell*)table->dequeueCell();
    if (!cell)
    {
       cell = new CustomTableViewCell();
       cell->autorelease();
       CCSprite *sprt_bg=CCSprite::create("user_select_bg.png");
       sprt_bg->setPosition(CCPointMake(300*(size.width/1024), 0));
       cell->addChild(sprt_bg,0,121);
        //cell data at current index
        CCDictionary *dict=(CCDictionary*)leaderBoard_Stat->objectAtIndex(idx);
        //user profile pic...
        std::string str_profile_pic =dict->valueForKey("user_profile")->getCString();
        std::string fullPath;
        bool is_rotated=false;
        std::string writablePath_temp= CCFileUtils::sharedFileUtils()->getWritablePath();
        fullPath = writablePath_temp + CCString::createWithFormat("%s",str_profile_pic.c_str())->getCString();
        CCSprite *profile_pic;
        if (CCFileUtils::sharedFileUtils()->isFileExist(fullPath.c_str())&&str_profile_pic.length()>0)
        {
            is_rotated=true;
            profile_pic=CCSprite::create(fullPath.c_str());
            profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        }
        else
        {
            is_rotated=false;
            fullPath="place holder.png";
            profile_pic=CCSprite::create("place holder.png");
            profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        }
        CCSprite *sprt_img_profile=CCSprite::create("profilepic_bg.png");
        sprt_img_profile->setPosition(sprt_bg->getPosition());
        CCMask *masked = CCMask::create(sprt_img_profile , profile_pic);
        cell->addChild(masked,11,124);
        masked->setPosition(CCPointMake(sprt_bg->getPositionX()-sprt_bg->getContentSize().width/2+100, 0));
        if (is_rotated)
            masked->setRotation(180);
        masked->setScale(0.35);
        //user name...
        std::string ret =dict->valueForKey("user_name")->getCString();
        if (strcmp(ret.c_str(),"")==0)
        {
            ret=CCString::createWithFormat("----")->getCString();
        }
        CCLabelTTF *name = CCLabelTTF::create(ret.c_str(), "Station-Medium", 50.0);
        name->setPosition(ccp(masked->getPositionX()+masked->getContentSize().width/2+15, sprt_bg->getPositionY()));
        name->setHorizontalAlignment(kCCTextAlignmentLeft);
        name->setColor(ccWHITE);
        name->setTag(123);
        cell->addChild(name);
        
        CCSprite *sprt=CCSprite::create("star_deactiv.png");
        cell->addChild(sprt);
        sprt->setPosition(ccpAdd(ccp(sprt_bg->getPositionX()+35,sprt_bg->getPositionY()),ccp(sprt_bg->getContentSize().width/2-sprt->getContentSize().width/2, 0)));
        sprt->setScale(0.4);
        CCProgressFromTo *progress_bar=CCProgressFromTo::create(2, 0, (dict->valueForKey("star_earned")->intValue()*100)/5);
        progress_bar->retain();
        CCProgressTimer *timer_progress = CCProgressTimer::create(CCSprite::create("star_activ.png"));
        timer_progress->setType(kCCProgressTimerTypeBar);
        timer_progress->setMidpoint(ccp(0, 0));
        timer_progress->setBarChangeRate(ccp(1, 0));
        timer_progress->setScale(sprt->getScale());
        cell->addChild(timer_progress);
        timer_progress->setPosition(sprt->getPosition());
        timer_progress->runAction(progress_bar);
    }
    else
    {
        CCLabelTTF *name = (CCLabelTTF*)cell->getChildByTag(123);
        CCDictionary *dict=(CCDictionary*)leaderBoard_Stat->objectAtIndex(idx);
        std::string ret =dict->valueForKey("user_name")->getCString();
        if (strcmp(ret.c_str(),"")==0)
        {
            ret=CCString::createWithFormat("--")->getCString();
        }
        name->setString(ret.c_str());
        std::string str_profile_pic =dict->valueForKey("user_profile")->getCString();
        std::string fullPath;
        bool is_rotated=false;
        std::string writablePath_temp= CCFileUtils::sharedFileUtils()->getWritablePath();
        fullPath = writablePath_temp + CCString::createWithFormat("%s",str_profile_pic.c_str())->getCString();
        CCSprite *profile_pic;
        if (CCFileUtils::sharedFileUtils()->isFileExist(fullPath))
        {
            is_rotated=true;
            profile_pic=CCSprite::create(fullPath.c_str());
            if (profile_pic)
                profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        }
        else
        {
            is_rotated=false;
            fullPath="place holder.png";
            profile_pic=CCSprite::create("place holder.png");
            profile_pic->setPosition(ccp(profile_pic->getContentSize().width/2, profile_pic->getContentSize().height/2));
        }
        CCMask *masked = (CCMask*)cell->getChildByTag(124);
        masked->changeObject(profile_pic);
        
    }
    return cell;
}
unsigned int LeaderBoardNode::numberOfCellsInTableView(CCTableView *table)
{
    return leaderBoard_Stat->count();
}
#pragma mark -
#pragma mark selector methods
#pragma mark -
void LeaderBoardNode::onHomeClicked(cocos2d::CCObject * pSender)
{
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    Scene_common* pScene = new Scene_common();
    if (pScene)
    {
        pScene->ReplaceScene("homeLayer.ccbi","homeLayer", homeLayerLoader::loader(),this,true,callfuncO_selector(homeLayer::onDidSwipe));
        pScene->release();
    }
}
void LeaderBoardNode::onRestartClicked(cocos2d::CCObject * pSender)
{
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    //CCLog("Restart Game clicked");
    CountNumberLayer *obj=(CountNumberLayer*)this->getParent();
    //CCLog("%s",obj->getuser_name()->getCString());
    /// add user data in plist and move along for playing game...
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader=NULL;
    CountNumberLayer * node =NULL;
    ccNodeLoaderLibrary->registerCCNodeLoader("LeaderBoardNode", LeaderBoardNodeLoader::loader());
    ccNodeLoaderLibrary->registerCCNodeLoader("CountNumberLayer", CountNumberLayerLoader::loader());
    ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    /* Read a ccbi file.*/
    //from each of the test scenes.
    node = (CountNumberLayer*)ccbReader->readNodeGraphFromFile("CountGameLayer.ccbi", this);
    ((CountNumberLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    CCScene * scene = CCScene::create();
    ccbReader->autorelease();
    if(node != NULL)
    {
        node->setuser_name(obj->getuser_name());
        node->setuser_profile_path(obj->getuser_profile_path());
        scene->addChild(node);
    }
    CCDirector::sharedDirector()->replaceScene(scene);
}
void LeaderBoardNode::onDidSwipe(CCObject* pSender)
{
    CCSwipe * swipe = (CCSwipe*)pSender;
    if (swipe->direction==kSwipeGestureRecognizerDirectionRight)
    {
        
    }
    else if (swipe->direction==kSwipeGestureRecognizerDirectionLeft)
    {
        
    }
    else if (swipe->direction==kSwipeGestureRecognizerDirectionUp)
    {
        
    }
    else  if (swipe->direction==kSwipeGestureRecognizerDirectionDown)
    {
        
    }
}
//make visible all the animating sprites...
void LeaderBoardNode::callback_show_objects(CCNode* sender)
{
 
}
#pragma mark -
#pragma mark animationManaget
#pragma mark -
void LeaderBoardNode::setAnimationManager(cocos2d::extension::CCBAnimationManager *animationManager)
{
    if (animationManager)
    {
        CC_SAFE_RELEASE_NULL(mAnimationManager);
        mAnimationManager = animationManager;
        CC_SAFE_RETAIN(animationManager);
    }
}
#pragma mark -
#pragma mark CCB Delegate
#pragma mark -
void LeaderBoardNode::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    ////CCLog("on loaded called");
}
SEL_CallFuncN LeaderBoardNode::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "callback_show_objects", LeaderBoardNode::callback_show_objects);
    
    return NULL;
}
SEL_MenuHandler LeaderBoardNode::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onHomeClicked", LeaderBoardNode::onHomeClicked);
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onRestartClicked", LeaderBoardNode::onRestartClicked);
  return NULL;
}
SEL_CCControlHandler LeaderBoardNode::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}
bool LeaderBoardNode::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    bool bRet = false;
    return bRet;
}
bool LeaderBoardNode::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mAnimationManager", CCBAnimationManager *, this->mAnimationManager);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lblCurrent_score", CCLabelTTF *, this->lblCurrent_score);
    
    return false;
}