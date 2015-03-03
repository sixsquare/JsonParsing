//
//  CCArrow.h
//  Learnnumber
//
//  Created by Neil D on 02/08/13.
//
//

#ifndef __Learnnumber__CCArrow__
#define __Learnnumber__CCArrow__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#include <iostream>
class CCArrow:public cocos2d::CCSprite
{
    public:
//        CCArrow(void);
        virtual ~CCArrow(void);
        static CCArrow* create(const char *pszSpriteFrameName);
};

#endif /* defined(__Learnnumber__CCArrow__) */
