//
//  CCArrow.cpp
//  Learnnumber
//
//  Created by Neil D on 02/08/13.
//
//

#include "CCArrow.h"
CCArrow* CCArrow::create(const char *pszSpriteFrameName)
{
    CCTexture2D *texture0 = CCTextureCache::sharedTextureCache()->addImage(pszSpriteFrameName);
    CCArrow *pSprite = new CCArrow();
    if (pSprite && pSprite->initWithTexture(texture0))
    {
        // Set to autorelease
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}
CCArrow::~CCArrow(void)
{
    
}