//
//  HelloCocosBuilderLayer.cpp
//  Demo
//  Created by Neil D on 20/05/13.

#include "SplashLayer.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
SplashLayer::SplashLayer()
{
    //CCLog("constructor called");
    // preload background music and effect
}
SplashLayer::~SplashLayer()
{
    //CCLog("destracture called");
}
void SplashLayer::onEnter()
{
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/bg_sound_1.mp3");
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/bg_sound_2.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/1.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/2.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/3.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/4.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/5.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/6.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/7.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/8.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/9.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/10.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/on_button_clicked.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sound/bird_tweet.mp3");
    // set default volume
    float isbg_on=CCUserDefault::sharedUserDefault()->getFloatForKey("bgmusic", 1.0);
    float issoundFX_on=CCUserDefault::sharedUserDefault()->getFloatForKey("soundfx", 1.0);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(isbg_on);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(issoundFX_on);
//    CCUserDefault::sharedUserDefault()->setFloatForKey("bgmusic", 1.0);
//    CCUserDefault::sharedUserDefault()->setFloatForKey("soundfx", 1.0);
    CCUserDefault::sharedUserDefault()->flush();
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/bg_sound_2.mp3",true);
    Scene_common* pScene = new Scene_common();
    if (pScene)
    {
        pScene->ReplaceScene("MainLayer.ccbi","MainLayer", MainLayerLoader::loader(),this,false,NULL);
        pScene->release();
    }
}
void SplashLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    //CCLog("on loaded called");
}
SEL_CallFuncN SplashLayer::onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}
SEL_MenuHandler SplashLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
   return NULL;
}
SEL_CCControlHandler SplashLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    //CCLog("onResolveCCBCCControlSelector");
    return NULL;
}
bool SplashLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
     return false;
}
bool SplashLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
       bool bRet = false;
       return bRet;
}