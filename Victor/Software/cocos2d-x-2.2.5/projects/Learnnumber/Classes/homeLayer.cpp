//
//  HelloCocosBuilderLayer.cpp
//  Demo
//
//  Created by Neil D on 20/05/13.
//
//
#include "homeLayer.h"
#include "../Classes/GamePlayLayer.h"
#include "../Classes/GamePlayLayerLoader.h"
#include "../Classes/CountNumberGame/CountNumberLayer.h"
#include "../Classes/CountNumberGame/CountNumberLayerLoader.h"
#include "../Classes/SelectPlayerLayerLoader.h"
#include "../Classes/SelectPlayerLayer.h"
#include "SimpleAudioEngine.h"
#include  "../Classes/MainLayer.h"
#include  "../Classes/MainLayerLoader.h"
#include  "../Classes/SettingLayerLoader.h"
#include "CCSwipeGestureRecognizer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
homeLayer::homeLayer():mAnimationManager(NULL),menuitem_1(NULL),menuitem_2(NULL),menuitem_3(NULL),menuitem_4(NULL),menuitem_5(NULL),menuitem_6(NULL),menuitem_7(NULL),menuitem_8(NULL),menuitem_9(NULL),menuitem_10(NULL),goes_next(false)
{
    game_click=0;
}
homeLayer::~homeLayer()
{
    CC_SAFE_RELEASE(mAnimationManager);
}
void homeLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    //CCLog("on loaded called");
}
SEL_CallFuncN homeLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "animationcompleted", homeLayer::animationcompleted);
    return NULL;
}
void homeLayer::animationcompleted(CCNode* sender)
{
    goes_next=false;
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader=NULL;
    CCNode * node =NULL;
    if(game_click==11)
    {
        
        Scene_common* pScene = new Scene_common();
        if (pScene)
        {
            pScene->ReplaceScene("SelectPlayerLayer.ccbi","SelectPlayerLayer", SelectPlayerLayerLoader::loader(),this,true,callfuncO_selector(SelectPlayerLayer::onDidSwipe));
            pScene->release();
        }
        return;

        ccNodeLoaderLibrary->registerCCNodeLoader("CountNumberLayer", CountNumberLayerLoader::loader());
        ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
        /* Read a ccbi file. */
        // from each of the test scenes.
        node = ccbReader->readNodeGraphFromFile("CountGameLayer.ccbi", this);
        ((CountNumberLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    }
    else if(game_click==12)
    {
        
        Scene_common* pScene = new Scene_common();
        if (pScene)
        {
          pScene->ReplaceScene("MainLayer.ccbi","MainLayer", MainLayerLoader::loader(),this,true,callfuncO_selector(homeLayer::onDidSwipe));
            pScene->release();
        }
        return;
//       ccNodeLoaderLibrary->registerCCNodeLoader("IdentifyNumberLayer", IdentifyNumberLayerLoader::loader());
//       ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
//       /* Read a ccbi file. */
//       // from each of the test scenes.
//       node = ccbReader->readNodeGraphFromFile("ClickGameLayer.ccbi", this);
    }
    else
    {
      ccNodeLoaderLibrary->registerCCNodeLoader("GamePlayLayer", GamePlayLayerLoader::loader());
      ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
      /* Read a ccbi file. */
      // from each of the test scenes.
      node = ccbReader->readNodeGraphFromFile("GamePlayLayer.ccbi", this);
      ((GamePlayLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
        
    }
    CCScene * scene = CCScene::create();
    ccbReader->autorelease();
    if(node != NULL)
    {
        CCSwipeGestureRecognizer * swipe = CCSwipeGestureRecognizer::create();
        swipe->setTarget(node,callfuncO_selector(GamePlayLayer::onDidSwipe));
        swipe->setDirection(kSwipeGestureRecognizerDirectionRight | kSwipeGestureRecognizerDirectionLeft|kSwipeGestureRecognizerDirectionUp|kSwipeGestureRecognizerDirectionDown);
        swipe->setCancelsTouchesInView(true);
        scene->addChild(swipe);

        scene->addChild(node);
        
    }
    //CCLog("moved to next scene");
    /* Push the new scene with a fancy transition. */
    CCDirector::sharedDirector()->pushScene(scene);
}
SEL_MenuHandler homeLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onNumberClicked", homeLayer::onNumberClicked);
    //CCLog("onResolveCCBCCMenuItemSelector");
    return NULL;
}
SEL_CCControlHandler homeLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    //CCLog("onResolveCCBCCControlSelector");
    return NULL;
}
bool homeLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menuitem_1", CCMenuItemImage*, menuitem_1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menuitem_2", CCMenuItemImage*, menuitem_2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menuitem_3", CCMenuItemImage*, menuitem_3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menuitem_4", CCMenuItemImage*, menuitem_4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menuitem_5", CCMenuItemImage*, menuitem_5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menuitem_6", CCMenuItemImage*, menuitem_6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menuitem_7", CCMenuItemImage*, menuitem_7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menuitem_8", CCMenuItemImage*, menuitem_8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menuitem_9", CCMenuItemImage*, menuitem_9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menuitem_10", CCMenuItemImage*, menuitem_10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mAnimationManager", CCBAnimationManager *, this->mAnimationManager);
     return false;
}
void homeLayer::onEnter()
{
    mAnimationManager->runAnimationsForSequenceNamedTweenDuration("enter_anim", 0.3f);
    goes_next=false;
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("isfullpurchased")==true)
    {
        
    }
    else
    {
        ObjCCalls::full_screen_banner(true);
        int ads_count=CCUserDefault::sharedUserDefault()->getIntegerForKey("ads_count", 1);
        if(ads_count%10==1)
            ads_count=9;
        switch (ads_count)
        {
            case 3:
                display_pop();
                break;
            case 6:
                display_pop();
                break;
            case 9:
                display_pop();
                break;
            default:
                //wtf no ads....!!!
                break;
        }
    }
    CCLayer::onEnter();
}
void homeLayer::display_pop()
{
    if (CCUserDefault::sharedUserDefault()->getIntegerForKey("should_display_ads")==0)
    {
        
    }
    else
    {
          #warning revmob
         ObjCCalls::full_screen_banner_revmob(true);
//        revmob::RevMob *revmob = revmob::RevMob::SharedInstance();
//        revmob->ShowFullscreen();
        CCUserDefault::sharedUserDefault()->setIntegerForKey("should_display_ads",0);
    }
}
#pragma mark -
#pragma animationManaget
void homeLayer::setAnimationManager(cocos2d::extension::CCBAnimationManager *animationManager)
{
    if (animationManager)
    {
        CC_SAFE_RELEASE_NULL(mAnimationManager);
        mAnimationManager = animationManager;
        CC_SAFE_RETAIN(animationManager);
    }
}
bool homeLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
       bool bRet = false;
       return bRet;
}
void homeLayer::onDidSwipe(CCObject* pSender)
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
        SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
        Scene_common* pScene = new Scene_common();
        if (pScene)
        {
            pScene->PushScene("SettingLayer.ccbi", "SettingLayer", SettingLayerLoader::loader(), this,true,callfuncO_selector(SettingLayer::onDidSwipe));
            pScene->autorelease();
        }
    }
    else  if (swipe->direction==kSwipeGestureRecognizerDirectionDown)
    {
         SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
        CCDirector::sharedDirector()->popScene();
    }
}
void homeLayer::onNumberClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    if (goes_next)
        return;
    CCMenuItemImage *item=(CCMenuItemImage*)pSender;
    if (item->getTag()==11||item->getTag()==12)
    {
        SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
        game_click=item->getTag();
        item->runAction( CCSequence::create(CCBlink::create(0.2,2),CCCallFunc::create(this, callfunc_selector(homeLayer::animate_seq)),NULL));
    }
    else
    {
        goes_next=true;
        game_click=item->getTag();
        CCString *str_playeffect=CCString::createWithFormat("sound/%d.mp3",item->getTag());
        SimpleAudioEngine::sharedEngine()->playEffect(str_playeffect->getCString());
        CCUserDefault::sharedUserDefault()->setIntegerForKey("number", item->getTag());
        CCUserDefault::sharedUserDefault()->flush();
        item->runAction( CCSequence::create(CCBlink::create(0.2,2),CCCallFunc::create(this, callfunc_selector(homeLayer::animate_seq)),NULL));
    }
}
void homeLayer::animate_seq()
{
    CCLOG("animation started");
    mAnimationManager->runAnimationsForSequenceNamedTweenDuration("dropmenus", 0.3f);
}