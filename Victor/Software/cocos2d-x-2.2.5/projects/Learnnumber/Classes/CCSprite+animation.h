//
//  CCSprite+animation.h
//  Learnnumber
//
//  Created by Neil D on 06/08/13.
//
//

#ifndef __Learnnumber__CCSprite_animation__
#define __Learnnumber__CCSprite_animation__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class CCSprite_animation:public cocos2d::CCSprite
{
    public:
    //CCArrow(void);
    virtual ~CCSprite_animation(void);
    static CCSprite_animation* create(const char *pszSpriteFrameName);
};
#endif /* defined(__Learnnumber__CCSprite_animation__) */
