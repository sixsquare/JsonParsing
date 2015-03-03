//
//  HelloCocosBuilderLayerLoader.h
//  Demo
//
//  Created by Neil D on 20/05/13.
//
//
#ifndef Demo_HelloCocosBuilderLayerLoader_h
#define Demo_HelloCocosBuilderLayerLoader_h
#include "../CountNumberGame/CountNumberLayer.h"

/* Forward declaration. */
class CCBReader;
class CountNumberLayerLoader : public cocos2d::extension::CCLayerLoader
{
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CountNumberLayerLoader, loader);   
    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CountNumberLayer);
};
#endif
