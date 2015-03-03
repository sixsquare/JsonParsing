//
//  HelloCocosBuilderLayer.cpp
//  Demo
//  Created by Neil D on 20/05/13.

#include "InfoLayer.h"
#include "SimpleAudioEngine.h"
#include "SelectPlayerLayerLoader.h"
#include  "MainLayerLoader.h"
#include "CCSwipeGestureRecognizer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
InfoLayer::InfoLayer():back(NULL)
{
    //CCLog("constructor called");
}
InfoLayer::~InfoLayer()
{
    //CCLog("destracture called");
}
void InfoLayer::onEnter()
{
    CCMenuItemSprite *image=CCMenuItemSprite::create(CCSprite::create(), CCSprite::create(), this, menu_selector(InfoLayer::onBackClicked));
    CCMenu *menu;
    menu=CCMenu::create(image,NULL);
    image->setPosition(back->getPosition());
    this->addChild(menu);
   // back->setTarget(this, menu_selector(InfoLayer::onBackClicked));
}
void InfoLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    
}
SEL_CallFuncN InfoLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}
SEL_MenuHandler InfoLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBackClicked", InfoLayer::onBackClicked);
    return NULL;
}
SEL_CCControlHandler InfoLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    //CCLog("onResolveCCBCCControlSelector");
    return NULL;
}
bool InfoLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "back", CCMenuItemImage *, this->back);
     return false;
}
bool InfoLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
       bool bRet = false;
       return bRet;
}
void  InfoLayer::onBackClicked(CCObject *pSender)
{
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    //CCLog("back button clicked");
    SimpleAudioEngine::sharedEngine()->playEffect("sound/on_button_clicked.mp3");
    Scene_common* pScene = new Scene_common();
    if (pScene)
    {
        pScene->ReplaceScene("MainLayer.ccbi","MainLayer", MainLayerLoader::loader(),this,true,callfuncO_selector(MainLayer::onDidSwipe));
        pScene->release();
    }
    
    
//    Scene_common* pScene = new Scene_common();
//    if (pScene)
//    {
//        pScene->replaceScene("SelectPlayerLayer.ccbi","SelectPlayerLayer", SelectPlayerLayerLoader::loader(),this);
//        pScene->release();
//    }
}
void InfoLayer::onDidSwipe(CCObject* pSender)
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
