#import <UIKit/UIKit.h>
#import "GADBannerView.h"
#import "IAPHelper.h"
#import <RevMobAds/RevMobAds.h>
//#import <RevMobAds/RevMobAdsDelegate.h>

#import <CoreLocation/CoreLocation.h>
#define kInAppProductIdentifierRemoveAd @"com.r3app.tracelearning.remove.ads"
@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate,IAPTransactionDelegate, CLLocationManagerDelegate>
{
    UIWindow *window;
    RootViewController    *viewController;
    UIActivityIndicatorView *activityIndicator;
    // Declare one as an instance variable
    GADBannerView *bannerView_;
}
-(void)loadInAppPurchase;
-(void)show_admob;
-(void)show_revmob;
@property (assign, nonatomic) CLLocationManager *locationManager;
//@property (nonatomic, strong)RevMobFullscreen *fullscreen;
//@property (nonatomic, strong)RevMobBannerView *banner;
//@property (nonatomic, strong)RevMobBanner *bannerWindow;
//@property (nonatomic, strong)RevMobAdLink *link;
@property(nonatomic,strong)UIWindow *window;
@property(nonatomic,strong)GADBannerView *bannerView_;
@property(nonatomic,strong)RootViewController    *viewController;
@end

