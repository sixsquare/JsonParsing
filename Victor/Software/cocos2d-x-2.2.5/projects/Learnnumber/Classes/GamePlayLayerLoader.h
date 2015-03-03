//
//  GamePlayLayerLoaded.h
//  Fishing
//
//  Created by Neil D on 14/06/13.
//
//
#ifndef Fishing_GamePlayLayerLoader_h
#define Fishing_GamePlayLayerLoader_h
#include "GamePlayLayer.h"
/* Forward declaration. */
class CCBReader;
class GamePlayLayerLoader : public cocos2d::extension::CCLayerLoader
{
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GamePlayLayerLoader, loader);
    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GamePlayLayer);
};
#endif
