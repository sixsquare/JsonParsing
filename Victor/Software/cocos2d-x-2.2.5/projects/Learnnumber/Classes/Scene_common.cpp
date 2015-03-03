//  Scene_common.cpp
//  Demo
//  Created by Neil D on 21/05/13.
#include "Scene_common.h"
#include "../Classes/homeLayer.h"
#include "../Classes/MainLayer.h"
#include "../Classes/Extension/CCGestureRecognizer/CCSwipeGestureRecognizer.h"
void Scene_common::RunWithScene(const char *pCCBFilename,const char *pCCNodename,CCNodeLoader *pCCNodeLoader,bool has_swipe,SEL_CallFuncO obj_selector)
{
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader(pCCNodename, pCCNodeLoader);
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    /* Read a ccbi file. */
    CCNode * node = ccbReader->readNodeGraphFromFile(pCCBFilename, this);
    ccbReader->release();
    CCScene * scene = CCScene::create();
    if(node != NULL)
    {
        scene->addChild(node);
    }
    CCDirector::sharedDirector()->runWithScene(scene);
}
void Scene_common::PushScene(const char *pCCBFilename,const char *pCCNodename,CCNodeLoader *pCCNodeLoader,CCObject *objowner,bool has_swipe,SEL_CallFuncO obj_selector)
{
    /* Create an autorelease CCNodeLoaderLibrary. */
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader(pCCNodename, pCCNodeLoader);
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    /* Read a ccbi file. */
    // from each of the test scenes.
    
    CCNode * node = ccbReader->readNodeGraphFromFile(pCCBFilename, objowner);
    if (strcmp(pCCNodename,"homeLayer")==0)
    {
        ((homeLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    }
    else if (strcmp(pCCNodename,"GamePlayLayer")==0)
    {
        ((GamePlayLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    }
    else if (strcmp(pCCNodename,"MainLayer")==0)
    {
        ((MainLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    }
    else if (strcmp(pCCNodename,"CountNumberLayer")==0)
    {
        ((CountNumberLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    }
    CCScene * scene = CCScene::create();
    //ccbReader->autorelease();
    if(node != NULL)
    {
        if (has_swipe)
        {
            CCSwipeGestureRecognizer * swipe = CCSwipeGestureRecognizer::create();
            swipe->setTarget(node,obj_selector);
            swipe->setDirection(kSwipeGestureRecognizerDirectionRight | kSwipeGestureRecognizerDirectionLeft|kSwipeGestureRecognizerDirectionUp|kSwipeGestureRecognizerDirectionDown);
            swipe->setCancelsTouchesInView(true);
            scene->addChild(swipe);
        }
        scene->addChild(node);
    }
    /* Push the new scene with a fancy transition. */
     CCDirector::sharedDirector()->pushScene(CCTransitionSlideInB::create(0.7f, scene));
    //CCDirector::sharedDirector()->pushScene(scene);
}
void Scene_common::ReplaceScene(const char *pCCBFilename,const char *pCCNodename,CCNodeLoader *pCCNodeLoader,CCObject *objowner,bool has_swipe,SEL_CallFuncO obj_selector)
{
    /* Create an autorelease CCNodeLoaderLibrary. */
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader(pCCNodename, pCCNodeLoader);
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    /* Read a ccbi file. */
    // from each of the test scenes.
    CCLOG("pCCNodename %s",pCCNodename);
    CCNode * node = ccbReader->readNodeGraphFromFile(pCCBFilename, objowner);
    if (strcmp(pCCNodename,"homeLayer")==0)
    {
        ((homeLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    }
    else if (strcmp(pCCNodename,"GamePlayLayer")==0)
    {
        ((GamePlayLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    }
    else if (strcmp(pCCNodename,"MainLayer")==0)
    {
        ((MainLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    }
    else if (strcmp(pCCNodename,"CountNumberLayer")==0)
    {
        ((CountNumberLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    }
    CCScene * scene = CCScene::create();
    ccbReader->autorelease();
    if(node != NULL)
    {
        if (has_swipe)
        {
            CCSwipeGestureRecognizer * swipe = CCSwipeGestureRecognizer::create();
            swipe->setTarget(node,obj_selector);
            swipe->setDirection(kSwipeGestureRecognizerDirectionRight | kSwipeGestureRecognizerDirectionLeft|kSwipeGestureRecognizerDirectionUp|kSwipeGestureRecognizerDirectionDown);
            swipe->setCancelsTouchesInView(true);
            scene->addChild(swipe);
        }
        scene->addChild(node);
    }
    /* Push the new scene with a fancy transition. */
    CCDirector::sharedDirector()->replaceScene(scene);

}