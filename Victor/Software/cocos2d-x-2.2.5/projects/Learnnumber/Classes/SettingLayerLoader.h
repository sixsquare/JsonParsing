//
//  HelloCocosBuilderLayerLoader.h
//  Demo
//  Created by Neil D on 20/05/13.
#ifndef Demo_SettingLayerLoader_h
#define Demo_SettingLayerLoader_h
#include "../Classes/SettingLayer.h"
/* Forward declaration. */
class CCBReader;
class SettingLayerLoader : public cocos2d::extension::CCLayerLoader
{
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SettingLayerLoader, loader);
    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SettingLayer);
};
#endif
