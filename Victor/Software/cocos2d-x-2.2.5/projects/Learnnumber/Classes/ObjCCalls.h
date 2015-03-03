//PrintMessage.h
#ifndef __PRINTMESSAGE_H__
#define __PRINTMESSAGE_H__

#include <stddef.h>

#include "cocos2d.h"

using namespace cocos2d;

class ObjCCalls
{
public:
    static void get_profile_image();
    static void get_user_image();
    static void openImage();
    static CCSprite* image();
    static void remove_ads();
    static void full_screen_banner(bool should_show);
    static void full_screen_banner_revmob(bool should_show);
};

#endif//__PRINTMESSAGE_H__