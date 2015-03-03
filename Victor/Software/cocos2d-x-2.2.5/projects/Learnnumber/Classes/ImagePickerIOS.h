#ifndef __IMAGE_PICKER_IOS__
#define __IMAGE_PICKER_IOS__

#import <UIKit/UIKit.h>

@interface ImagePickerIOS : UIViewController<UINavigationControllerDelegate, UIImagePickerControllerDelegate, UIPopoverControllerDelegate,UIActionSheetDelegate>
{
    UIImage *image;
}

-(void) takePicture;
-(void) cleanup;
-(UIImage*) getImage;

@end

#endif  // __IMAGE_PICKER_IOS__