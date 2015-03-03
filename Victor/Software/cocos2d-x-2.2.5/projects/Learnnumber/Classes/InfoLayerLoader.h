//
//  HelloCocosBuilderLayerLoader.h
//  Demo
//
//  Created by Neil D on 20/05/13.
//
//
#ifndef Demo_InfoLayerLoader_h
#define Demo_InfoLayerLoader_h
#include "InfoLayer.h"
/* Forward declaration. */
class CCBReader;
class InfoLayerLoader : public cocos2d::extension::CCLayerLoader
{
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(InfoLayerLoader, loader);
    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(InfoLayer);
};
#endif