//
//  HelloCocosBuilderLayerLoader.h
//  Demo
//  Created by Neil D on 20/05/13.

#ifndef Demo_homeLayerLoader_h
#define Demo_homeLayerLoader_h
#include "../Classes/homeLayer.h"
/* Forward declaration.*/

class CCBReader;
class homeLayerLoader : public cocos2d::extension::CCLayerLoader
{
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(homeLayerLoader, loader);   
    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(homeLayer);
};
#endif
