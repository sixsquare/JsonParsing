//
//  Scene_common.h
//  Demo
//
//  Created by Neil D on 21/05/13.
#ifndef __Demo__Scene_common__
#define __Demo__Scene_common__
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
class Scene_common : public CCScene
{
    public:
    virtual void RunWithScene(const char *pCCBFilename,const char *pCCNodename,CCNodeLoader *pCCNodeLoader,bool has_swipe,SEL_CallFuncO obj_selector);
    virtual void PushScene(const char *pCCBFilename,const char *pCCNodename,CCNodeLoader *pCCNodeLoader,CCObject *objowner,bool has_swipe,SEL_CallFuncO obj_selector);
    virtual void ReplaceScene(const char *pCCBFilename,const char *pCCNodename,CCNodeLoader *pCCNodeLoader,CCObject *objowner,bool has_swipe,SEL_CallFuncO obj_selector);
};
#define CF(className) create##className
#endif /* defined(__Demo__Scene_common__) */
