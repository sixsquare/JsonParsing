//
//  HelloCocosBuilderLayerLoader.h
//  Demo
//  Created by Neil D on 20/05/13.

#ifndef Demo_MainLayerLoader_h
#define Demo_MainLayerLoader_h
#include "../Classes/MainLayer.h"
/* Forward declaration.*/
class CCBReader;
class MainLayerLoader : public cocos2d::extension::CCLayerLoader
{
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MainLayerLoader, loader);
    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MainLayer);
};
#endif
