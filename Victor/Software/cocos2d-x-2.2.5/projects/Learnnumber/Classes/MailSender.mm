//
//  MailSender.m
//  GetSocial
//
//  Created by Mikel Eizagirre on 24/03/13.
//
//
#import "MailSender.h"
#import "../cocos2dx/platform/ios/EAGLView.h"
#import "../Classes/SelectPlayerLayer.h"
#import "../Classes/SettingLayer.h"
#import <MobileCoreServices/UTCoreTypes.h>

@implementation MailSender
@synthesize popOver;
- (void) imagePickerController:(UIImagePickerController*) picker didFinishPickingMediaWithInfo:(NSDictionary*) info
{
    NSString *mediaType = [info objectForKey: UIImagePickerControllerMediaType];
    UIImage *originalImage, *editedImage, *imageToSave;
    if (CFStringCompare ((CFStringRef) mediaType, kUTTypeImage, 0) == kCFCompareEqualTo)
    {
        editedImage = (UIImage *) [info objectForKey:UIImagePickerControllerEditedImage];
        originalImage = (UIImage *) [info objectForKey:UIImagePickerControllerOriginalImage];
        imageToSave = editedImage != nil ? editedImage : originalImage;
        CGImageRef imageRef = [originalImage CGImage];
        NSUInteger width = CGImageGetWidth(imageRef);
        NSUInteger height = CGImageGetHeight(imageRef);
        //CCLog("%i,%i",width,height);
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        unsigned char* rawData = (unsigned char*) calloc(height * width * 4, sizeof(unsigned char));
        NSUInteger bytesPerPixel = 4;
        NSUInteger bytesPerRow = bytesPerPixel * width;
        NSUInteger bitsPerComponent = 8;
        CGContextRef context = CGBitmapContextCreate(rawData, width, height, bitsPerComponent, bytesPerRow, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        CGColorSpaceRelease(colorSpace);
        CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
        CGContextRelease(context);
        cocos2d::CCTexture2D *pickedImage = new cocos2d::CCTexture2D();
        pickedImage->initWithData(rawData, cocos2d::kCCTexture2DPixelFormat_RGBA8888, width, height, cocos2d::CCSizeMake(width, height));
        CCSprite *sprit = CCSprite::createWithTexture(pickedImage);
       
        // Save the new image (original or edited) to the Camera Roll  if the image have metadata, which means it comes from the library
        if([info objectForKey: UIImagePickerControllerMediaMetadata] != nil)
        {
            NSLog(@"Saving photo in library");
            UIImageWriteToSavedPhotosAlbum (imageToSave, nil, nil , nil);
        }
        CGSize targetSize = imageToSave.size;
        UIImage* resultImage = nil;
        //Here, width and height are your size limit. The image will fit in those bounds
        //float scaleX = width/imageToSave.size.width;
        //float scaleY = height/imageToSave.size.height;
        float scale =1;// MIN(scaleX, scaleY);
        if(scale != 1)
        {
            targetSize.width *= scale;
            targetSize.height *= scale;
            //hard resize because the image can be way too heavy (1536x2048 on 3GS ....)
            resultImage =imageToSave;// [imageToSave resizedImage:targetSize interpolationQuality:kCGInterpolationHigh]; //see vocaro.com/trevor/blog/2009/10/12/resize-a-uiimage-the-right-way/
        }
        else
        {
            resultImage = imageToSave;
        }
        /*        NSString* fileName = [NSString stringWithFormat:@"Documents/%@.png", saveName];
            NSString  *pngPath = [NSHomeDirectory() stringByAppendingPathComponent:fileName];
            NSError* error = NULL;
            BOOL result = [UIImagePNGRepresentation(resultImage) writeToFile:pngPath options:NSDataWritingAtomic error:&error];
            //BOOL result = [UIImagePNGRepresentation(resultImage) writeToFile:pngPath atomically:YES];
        //    NSLog(@"Write result for file %@ : %@, fullPath : %@", fileName, (result ? @"OK" : @"Problem"), pngPath);
            if(result)
            {
                //Save in the Documents directory
               // CCString* fullPath = ScreateF("%s/Documents/%s.png", getenv("HOME"), [saveName UTF8String]);
                //Remove the texture from the cache so that if you reuse a name, you'll have the new one next time you create a CCSprite with it
                //CCTextureCache::sharedTextureCache()->removeTextureForKey(fullPath->getCString());
                //change that call by your own notification
                //notifyImagePicked([saveName UTF8String], [identifier UTF8String]);
            }
            else
            {
                NSLog(@"Write error description: %@, reason: %@", [error localizedDescription], [error localizedFailureReason]);
            }
    */
        //        controller = [[UIImagePickerController alloc] init];
        //        controller.delegate = self;
    }
    else
    {
        NSLog(@"Problem : picked a media which is not an image");
    }
    if (self.popOver)
    {
        [self.popOver dismissPopoverAnimated:YES];
        
    }
    [self imagePickerControllerDidCancel:picker];
}
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    // Dismiss the image selection and close the program
    [picker dismissModalViewControllerAnimated:YES];
    if(self.popOver)
    {
        [self.popOver dismissPopoverAnimated:YES];
    }
}
- (void)get_image_from_library
{
    UIActionSheet *actionsheet=[[UIActionSheet alloc]initWithTitle:@"Select Photo" delegate:self cancelButtonTitle:@"Cancel" destructiveButtonTitle:Nil otherButtonTitles:@"Gallery",@"Camera", nil ];
    ;
    [actionsheet showInView:[[[[UIApplication sharedApplication] keyWindow] subviews] lastObject]];

}
- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    NSLog(@"%d",buttonIndex);
}
- (void)actionSheetCancel:(UIActionSheet *)actionSheet
{

}
- (void)willPresentActionSheet:(UIActionSheet *)actionSheet
{

}
- (void)didPresentActionSheet:(UIActionSheet *)actionSheet
{

}
- (void)actionSheet:(UIActionSheet *)actionSheet willDismissWithButtonIndex:(NSInteger)buttonIndex
{
    NSLog(@"%d",buttonIndex);
}
- (void)actionSheet:(UIActionSheet *)actionSheet didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    [self get_image:buttonIndex];
}
-(void)get_image:(int)k
{
    AppController *app=(AppController*)[[UIApplication sharedApplication] delegate];
    UIImagePickerController  *picker = [[UIImagePickerController alloc] init];
    picker.delegate = self;
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]&&k==1)
    {
        picker.sourceType = UIImagePickerControllerSourceTypeCamera;
    }
    else
    {
        picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    }
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
    {
        UIPopoverController *popover = [[UIPopoverController alloc] initWithContentViewController:picker];
        UIView *view=[[[[UIApplication sharedApplication] keyWindow] subviews] lastObject];
       [popover presentPopoverFromRect:CGRectMake(view.frame.size.width/3.5, view.frame.size.height/2.5, 10.0f, 10.0f) inView:view permittedArrowDirections:UIPopoverArrowDirectionLeft animated:YES];
        self.popOver = popover;
    } else {
        [app.viewController presentModalViewController:picker animated:YES];
    }
    //[app.viewController presentViewController:picker animated:true completion:^{}];
}
-(void)addAdMobBanner
{
//    objBanner=[[GADBannerView alloc]initWithAdSize:kGADAdSizeBanner];
//    [objBanner setAdUnitID:@"ca-app-pub-7340287447465629/4145651596"];
//    AppController *app=(AppController*)[[UIApplication sharedApplication] delegate];
//    NSLog(@"%@",app.window.rootViewController);
//    [objBanner setRootViewController:app.window.rootViewController];
//    [objBanner setFrame:CGRectMake(50, 50, 320, 50)];
//    [app.window addSubview:objBanner];
//    GADRequest *req=[GADRequest request];
//    [objBanner loadRequest:Nil];
//    [objBanner loadRequest:req];
    
}
-(void)RemoveAdd_with_IAP
{
    AppController *app=(AppController*)[[UIApplication sharedApplication] delegate];
    [app loadInAppPurchase];
    
}
-(void)full_banner:(BOOL )should_show
{
    AppController *app=(AppController*)[[UIApplication sharedApplication] delegate];
    if (should_show)
    {
        app.bannerView_.alpha=1;
    }
    else
    {
        app.bannerView_.alpha=0;
    }
}
-(void)full_banner_revmob:(BOOL )should_show
{
    AppController *app=(AppController*)[[UIApplication sharedApplication] delegate];
    [app show_revmob];
}
@end