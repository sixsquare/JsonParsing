//
//  HelloCocosBuilderLayerLoader.h
//  Demo
//
//  Created by Neil D on 20/05/13.
//
//
#ifndef Demo_LeaderBoardNodeLoader_h
#define Demo_LeaderBoardNodeLoader_h
#include "../CountNumberGame/LeaderBoardNode.h"

/* Forward declaration. */
class CCBReader;
class LeaderBoardNodeLoader : public cocos2d::extension::CCNodeLoader
{
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LeaderBoardNodeLoader, loader);
    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LeaderBoardNode);
};
#endif
