//
//  CCSprite+animation.cpp
//  Learnnumber
//
//  Created by Neil D on 06/08/13.

#include "CCSprite+animation.h"
CCSprite_animation* CCSprite_animation::create(const char *pszSpriteFrameName)
{
    CCSprite_animation *pSprite = new CCSprite_animation();
    if (pSprite && pSprite->initWithSpriteFrameName(pszSpriteFrameName))
    {
        // Set to autorelease
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}
CCSprite_animation::~CCSprite_animation(void)
{
    
}