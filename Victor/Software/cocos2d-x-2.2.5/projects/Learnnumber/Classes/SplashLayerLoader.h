//
//  HelloCocosBuilderLayerLoader.h
//  Demo
//
//  Created by Neil D on 20/05/13.
//
//
#ifndef Demo_SplashLayerLoader_h
#define Demo_SplashLayerLoader_h
#include "SplashLayer.h"
/* Forward declaration. */
class CCBReader;
class SplashLayerLoader : public cocos2d::extension::CCLayerLoader
{
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SplashLayerLoader, loader);   
    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SplashLayer);
};
#endif