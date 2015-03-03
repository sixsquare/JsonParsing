
#import "ImagePickerIOS.h"
#import <QuartzCore/QuartzCore.h>
#include "ImagePicker.h"
#import "AppController.h"

@implementation ImagePickerIOS

UIWindow *window2;
UIPopoverController *pop;

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
}

-(void) takePicture
{
    UIActionSheet *actionsheet=[[UIActionSheet alloc]initWithTitle:@"Select Photo" delegate:self cancelButtonTitle:@"Cancel" destructiveButtonTitle:Nil otherButtonTitles:@"Gallery",@"Camera", nil ];
    [actionsheet showInView:[[[[UIApplication sharedApplication] keyWindow] subviews] lastObject]];
}
-(void )takePicture:(int )source
{
    UIView *view=[[[[UIApplication sharedApplication] keyWindow] subviews] lastObject];
    UIImagePickerController *imagePicker = [[UIImagePickerController alloc] init];
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]&&source==1)
    {
        imagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;
    }
    else
    {
        imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    }
    [imagePicker setDelegate:self];
    pop = [[UIPopoverController alloc] initWithContentViewController:imagePicker];
    [pop setDelegate:self];
    [pop presentPopoverFromRect:CGRectMake(view.frame.size.width/3.5, view.frame.size.height/2.5, 10.0f, 10.0f) inView:view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
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
    if (buttonIndex==0||buttonIndex==1)
    {
        [self takePicture:buttonIndex];
    }
    else
    {
        ImagePicker::dismissed();
       
    }
}
-(void) imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
   image = [self imageWithImage:[info objectForKey:UIImagePickerControllerOriginalImage] scaledToSize:CGSizeMake(193, 193)];
    [image retain];
    [self cleanup];
    ImagePicker::setReady();
}
-(UIImage *)imageWithImage:(UIImage*)image1 scaledToSize:(CGSize)newSize
{
    float oldWidth = image1.size.width;
    float scaleFactor = newSize.width / oldWidth;
    
    float newHeight = image1.size.height * scaleFactor;
    float newWidth = oldWidth * scaleFactor;
    
//    UIGraphicsBeginImageContext(CGSizeMake(newWidth, newHeight));
    UIGraphicsBeginImageContextWithOptions(newSize, YES, [UIScreen mainScreen].scale);
    [image1 drawInRect:CGRectMake(0, 0, newWidth, newHeight)];
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
//    
//    // Create a bitmap context.
//    UIGraphicsBeginImageContextWithOptions(newSize, YES, [UIScreen mainScreen].scale);
//    [image1 drawInRect:CGRectMake(0,0,newSize.width,newSize.height)];
//    
//    UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
//    UIGraphicsEndImageContext();
    return newImage;
    
}
- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController
{
    ImagePicker::dismissed();
    [self cleanup];
}
-(void) cleanup
{
    if (pop)
    {
        [pop dismissPopoverAnimated:YES];
        [pop release];
    }
    
//    [window2 setHidden:YES];
//    [window2 release];
    
}
-(UIImage*) getImage
{
    return image;
}
- (void)viewDidUnload
{
    [super viewDidUnload];
}
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}
- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}
- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
}
- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

@end
