#include "ObjCCalls.h" 
#import "ImagePickerIOS.h"
#import "../cocos2dx/platform/ios/EAGLView.h" 
#import "MailSender.h"

ImagePickerIOS *sharedPicker = nil;
void ObjCCalls::openImage()
{
    if (sharedPicker == nil)
    {
        sharedPicker = [[ImagePickerIOS alloc] init];
        [sharedPicker retain];
    }
    [sharedPicker takePicture];
}
CCSprite* ObjCCalls::image()
{
    CCImage *imf =new CCImage();
    NSData *imgData = UIImagePNGRepresentation([sharedPicker getImage]);
    NSUInteger len = [imgData length];
    Byte *byteData = (Byte*)malloc(len);
    memcpy(byteData, [imgData bytes], len);
    imf->initWithImageData(byteData,imgData.length);
    imf->autorelease();
    CCTexture2D* pTexture = new CCTexture2D();
    pTexture->initWithImage(imf);
    pTexture->autorelease();
    CCSprite *sprit = CCSprite::createWithTexture(pTexture);
    return sprit;
}
void ObjCCalls::get_profile_image()
{
    MailSender *mailsender=[MailSender alloc];
    [mailsender get_image_from_library];
}

void ObjCCalls::get_user_image()
{
    MailSender *mailsender=[MailSender alloc];
    [mailsender RemoveAdd_with_IAP];
}
void ObjCCalls::full_screen_banner(bool should_show)
{
    MailSender *mailsender=[MailSender alloc];
    [mailsender full_banner:should_show];
}
void ObjCCalls::full_screen_banner_revmob(bool should_show)
{
    MailSender *mailsender=[MailSender alloc];
    [mailsender full_banner_revmob:should_show];

}
