//
//  HelloCocosBuilderLayerLoader.h
//  Demo
//
//  Created by Neil D on 20/05/13.
//
//
#ifndef Demo_SelectPlayerLayerLoader_h
#define Demo_SelectPlayerLayerLoader_h
#include "../Classes/SelectPlayerLayer.h"
/* Forward declaration. */
class CCBReader;
class SelectPlayerLayerLoader : public cocos2d::extension::CCLayerLoader
{
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SelectPlayerLayerLoader, loader);
    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SelectPlayerLayer);
};
#endif
