//
//  MailSender.h
//  GetSocial
//
//  Created by Mikel Eizagirre on 24/03/13.
//
//

#import <UIKit/UIKit.h>
#import <MessageUI/MessageUI.h>
#import "AppController.h"
#import "GADBannerView.h"
@interface MailSender : UIViewController <UIImagePickerControllerDelegate,UINavigationControllerDelegate,UIActionSheetDelegate>
{
    
    bool is_profile_pic;
    GADBannerView *objBanner;
    
}
@property (nonatomic, strong) UIPopoverController *popOver;
- (void)get_image_from_library;
-(void)addAdMobBanner;
-(void)RemoveAdd_with_IAP;
-(void)full_banner:(BOOL )should_show;
-(void)full_banner_revmob:(BOOL )should_show;
@end
