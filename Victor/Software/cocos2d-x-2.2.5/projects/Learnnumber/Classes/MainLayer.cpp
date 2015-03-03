//Created by Neil D on 20/05/13.
#include "MainLayer.h"
#include "SettingLayer.h"
#include "SettingLayerLoader.h"
#include "LeaderBoardNodeLoader.h"
#include "SimpleAudioEngine.h"
#include "ObjCCalls.h"
#include "../Classes/Extension/CCGestureRecognizer/CCSwipeGestureRecognizer.h"
USING_NS_CC;
USING_NS_CC_EXT;
#ifndef CC_TARGET_PLATFORM
#error "CC_TARGET_PLATFORM should be defined!!!"
#endif
using namespace CocosDenshion;
MainLayer::MainLayer():mAnimationManager(NULL)
{
    game_click=0;
}
MainLayer::~MainLayer()
{
   
    CC_SAFE_RELEASE(mAnimationManager);
}
void MainLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    //CCLog("on loaded called");
}
SEL_CallFuncN MainLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "animationcompleted", MainLayer::animationcompleted);
    return NULL;
}
void MainLayer::animationcompleted(CCNode* sender)
{
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader=NULL;
    CCNode * node =NULL;
    if(game_click==11)
    {
        ccNodeLoaderLibrary->registerCCNodeLoader("LeaderBoardNode", LeaderBoardNodeLoader::loader());
        ccNodeLoaderLibrary->registerCCNodeLoader("CountNumberLayer", CountNumberLayerLoader::loader());
        ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
        /* Read a ccbi file. */
        // from each of the test scenes.
        node = ccbReader->readNodeGraphFromFile("CountGameLayer.ccbi", this);
        ((CountNumberLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    }
    else if(game_click==12)
    {
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
        scene->addChild(node);
    }
    //CCLog("moved to next scene");
    /* Push the new scene with a fancy transition. */
    CCDirector::sharedDirector()->replaceScene(scene);
}
SEL_MenuHandler MainLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onNumberClicked", MainLayer::onNumberClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onGameClicked", MainLayer::onGameClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPlayClicked", MainLayer::onPlayClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSettingClicked", MainLayer::onSettingClicked);
    //CCLog("onResolveCCBCCMenuItemSelector");
    return NULL;
}
SEL_CCControlHandler MainLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    //CCLog("onResolveCCBCCControlSelector");
    return NULL;
}
bool MainLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mAnimationManager", CCBAnimationManager *, this->mAnimationManager);
     return false;
}
void MainLayer::onEnter()
{
    ObjCCalls::full_screen_banner(false);

   // ObjCCalls::full_screen_banner_revmob(true);
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("isfullpurchased")==true)
    {
        
    }
    else
    {
        int ads_count=CCUserDefault::sharedUserDefault()->getIntegerForKey("ads_count", 1);
        if (ads_count%10==0)
          ads_count=9;
        switch (ads_count)
        {
            case 2:
                display_pop();
                break;
            case 5:
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
void MainLayer::display_pop()
{
    if (CCUserDefault::sharedUserDefault()->getIntegerForKey("should_display_ads")==0)
    {
        
    }
    else
    {
        //        revmob::RevMob *revmob = revmob::RevMob::SharedInstance();
        //        revmob->ShowFullscreen();
       // ObjCCalls::full_screen_banner_revmob(true);
       // CCUserDefault::sharedUserDefault()->setIntegerForKey("should_display_ads",0);
    }

}
#pragma mark -
#pragma animation Managet
#pragma mark -
void MainLayer::setAnimationManager(cocos2d::extension::CCBAnimationManager *animationManager)
{
    if(animationManager)
    {
        CC_SAFE_RELEASE_NULL(mAnimationManager);
        mAnimationManager = animationManager;
        CC_SAFE_RETAIN(animationManager);
    }
}
bool MainLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
       //CCLog("onAssignCCBCustomProperty");
       bool bRet = false;
       return bRet;
}
void MainLayer::onNumberClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    CCMenuItemImage *item=(CCMenuItemImage*)pSender;
    if(item->getTag()==11||item->getTag()==12)
    {
        game_click=item->getTag();
        item->runAction( CCSequence::create(CCBlink::create(0.4,2),CCCallFunc::create(this, callfunc_selector(MainLayer::animate_seq)),NULL));
    }
    else
    {
        game_click=item->getTag();
        CCUserDefault::sharedUserDefault()->setIntegerForKey("number", item->getTag());
        CCUserDefault::sharedUserDefault()->flush();
        item->runAction( CCSequence::create(CCBlink::create(0.4,2),CCCallFunc::create(this, callfunc_selector(MainLayer::animate_seq)),NULL));
    }
}
void MainLayer::animate_seq()
{
    mAnimationManager->runAnimationsForSequenceNamedTweenDuration("dropmenus", 0.3f);
}
#pragma mark -
#pragma mark Action methods
#pragma mark -
void MainLayer::onDidSwipe(CCObject* pSender)
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
void MainLayer::onGameClicked(CCObject *pSender)
{
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    Scene_common* pScene = new Scene_common();
    if (pScene)
    {
        pScene->PushScene("SelectPlayerLayer.ccbi","SelectPlayerLayer", SelectPlayerLayerLoader::loader(),this,true,callfuncO_selector(SelectPlayerLayer::onDidSwipe));
        pScene->release();
    }
    return;
}
void MainLayer::onPlayClicked(CCObject *pSender)
{
    ObjCCalls::full_screen_banner(true);
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    Scene_common* pScene = new Scene_common();
    if (pScene)
    {
        pScene->PushScene("homeLayer.ccbi","homeLayer", homeLayerLoader::loader(),this,true,callfuncO_selector(homeLayer::onDidSwipe));
        pScene->release();
    }
}
void MainLayer::onSettingClicked(CCObject *pSender)
{
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    Scene_common* pScene = new Scene_common();
    if (pScene)
    {
         pScene->PushScene("SettingLayer.ccbi","SettingLayer", SettingLayerLoader::loader(),this,true,callfuncO_selector(SettingLayer::onDidSwipe));
        pScene->autorelease();
    }
}