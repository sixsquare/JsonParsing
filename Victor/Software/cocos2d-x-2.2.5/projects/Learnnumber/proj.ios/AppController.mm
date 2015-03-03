#import "AppController.h"
#import "EAGLView.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#define REVMOB_ID @"534cfbb3349177b8483ce7d0"

@implementation AppController
@synthesize viewController;
@synthesize window;
@synthesize bannerView_;
#pragma mark -
#pragma mark Application lifecycle
//cocos2d application instance
static AppDelegate s_sharedApplication;
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [self loadInAppPurchase];
    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    self.window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];

    // Init the EAGLView
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0];
    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [self.window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    [window makeKeyAndVisible];
    
    //Create and add the Activity Indicator to loadingView
    activityIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    activityIndicator.alpha = 0.0;
    activityIndicator.center = CGPointMake(window.frame.size.width/2, window.frame.size.height/2);
    activityIndicator.hidesWhenStopped = NO;
    [window addSubview:activityIndicator];

    //[[NSUserDefaults standardUserDefaults] setBool:TRUE forKey:kInAppProductIdentifierRemoveAd];
    if ([[NSUserDefaults standardUserDefaults]boolForKey:kInAppProductIdentifierRemoveAd ]==TRUE)
    {
        cocos2d::CCUserDefault::sharedUserDefault()->setBoolForKey("isfullpurchased", true);
    }
    else
    {
        cocos2d::CCUserDefault::sharedUserDefault()->setBoolForKey("isfullpurchased", false);
        
//        [RevMobAds session].userGender = RevMobUserGenderUndefined;
//        [RevMobAds session].userInterests = @[@"games", @"mobile", @"advertising"];
//        [RevMobAds session].userPage = @"http://www.facebook.com/revmob";
//        [RevMobAds session].userAgeRangeMax = 8;
//        [RevMobAds session].userAgeRangeMin = 2;
        //[RevMobAds session].parallaxMode = RevMobParallaxModeWithBackground;
        [self show_admob];
    }
    cocos2d::CCApplication::sharedApplication()->run();
    return YES;
}
-(void)show_admob
{
    // Create a view of the standard size at the top of the screen.
    // Available AdSize constants are explained in GADAdSize.h.
   bannerView_ = [[GADBannerView alloc] initWithAdSize: kGADAdSizeBanner];
    // Specify the ad unit ID.
    //bannerView_.adUnitID = @"ca-app-pub-7340287447465629/4145651596";
    bannerView_.adUnitID = @"ca-app-pub-7340287447465629/8121205998";
    
    // Let the runtime know which UIViewController to restore after taking
    // the user wherever the ad goes and add it to the view hierarchy.
    // [bannerView_ setFrame:CGRectMake(window.frame.size.width/2-kGADAdSizeBanner.size.width,window.frame.size.height/2, kGADAdSizeBanner.size.width, kGADAdSizeBanner.size.height)];
    [viewController.view addSubview:bannerView_];
    [viewController.view bringSubviewToFront:bannerView_ ];
    bannerView_.center=CGPointMake(viewController.view.frame.size.height/2,viewController.view.frame.size.width-kGADAdSizeLeaderboard.size.height/2);
    bannerView_.rootViewController = viewController;
    // Initiate a generic request to load it with an ad.
    GADRequest *r = [[GADRequest alloc] init];
    r.testing = YES;
    [bannerView_ loadRequest:r];

}
-(void)show_revmob
{
    [RevMobAds startSessionWithAppID:REVMOB_ID];
    RevMobFullscreen *fs = [[RevMobAds session] fullscreen];
    fs.supportedInterfaceOrientations = @[@(UIInterfaceOrientationLandscapeRight), @(UIInterfaceOrientationLandscapeLeft)];
    
    [fs loadWithSuccessHandler:^(RevMobFullscreen *fs) {
        [fs showAd];
        //[self revmobAdDidReceive];
    } andLoadFailHandler:^(RevMobFullscreen *fs, NSError *error) {
        //[self revmobAdDidFailWithError:error];
    } onClickHandler:^{
        //[self revmobUserClickedInTheAd];
    } onCloseHandler:^{
       // [self revmobUserClosedTheAd];
    }];
}
-(void)locationManager:(CLLocationManager *)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status
{
//    BOOL locationAllowed = [CLLocationManager locationServicesEnabled] && ([CLLocationManager authorizationStatus] != kCLAuthorizationStatusDenied);
//      RevMobAds *revmob = [RevMobAds session];
//    if (locationAllowed){
//        CLLocation *location = self.locationManager.location;
//        
//        [self.locationManager setDistanceFilter: kCLDistanceFilterNone];
//        [self.locationManager setDesiredAccuracy: kCLLocationAccuracyHundredMeters];
//        [self.locationManager startUpdatingLocation];
//       
//        [revmob setUserLatitude: location.coordinate.latitude
//                  userLongitude: location.coordinate.longitude
//                   userAccuracy: location.horizontalAccuracy];
//    }
}
- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}
- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [[RevMobAds session] showFullscreen];
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    cocos2d::CCDirector::sharedDirector()->resume();
}
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}
- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}
- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}
#pragma mark -
#pragma mark Memory management
- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}
- (void)dealloc
{
    [window release];
    [super dealloc];
}
#pragma mark -
#pragma mark In App Purchase
#pragma mark -
-(void)loadInAppPurchase
{
	IAPHelper *iapHelper = [IAPHelper sharedHelper];
	iapHelper.delegate = self;
	[iapHelper requestProducts];
    activityIndicator.alpha = 1.0;
    [activityIndicator startAnimating];
}
- (void)finishedProductLoading
{
    IAPHelper *iapHelper = [IAPHelper sharedHelper];
	[iapHelper buyProduct:[iapHelper.products objectAtIndex:0]];
}
-(void)finishdProductPurchased:(NSString*)transaction
{
	[activityIndicator stopAnimating];
	[self finishedProductPurchase];
}
-(void)failedProductPurchased:(SKPaymentTransaction*)transaction
{
    activityIndicator.alpha = 0.0;
   	[activityIndicator stopAnimating];
    [[[UIAlertView alloc] initWithTitle:@"Failure" message:[transaction.error localizedDescription] delegate:self cancelButtonTitle:nil otherButtonTitles:@"Ok", nil] show];
}
- (void)finishedProductPurchase
{
    activityIndicator.alpha = 0.0;
    CCLOG("remove Add::purchased item");
    cocos2d::CCUserDefault::sharedUserDefault()->setBoolForKey("isfullpurchased", true);
    [bannerView_ removeFromSuperview];
    [[NSUserDefaults standardUserDefaults] setBool:TRUE forKey:kInAppProductIdentifierRemoveAd];
    [[NSUserDefaults standardUserDefaults] synchronize];
}
#pragma mark - RevMobAdsDelegate methods
//
//- (void)revmobAdDidReceive {
//    NSLog(@"[RevMob Sample App] Ad loaded.");
//}
//
//- (void)revmobAdDidFailWithError:(NSError *)error {
//    NSLog(@"[RevMob Sample App] Ad failed: %@", error);
//}
//
//- (void)revmobAdDisplayed {
//    NSLog(@"[RevMob Sample App] Ad displayed.");
//}
//
//- (void)revmobUserClosedTheAd {
//    NSLog(@"[RevMob Sample App] User clicked in the close button.");
//}
//
//- (void)revmobUserClickedInTheAd {
//    NSLog(@"[RevMob Sample App] User clicked in the Ad.");
//}
//
//- (void)installDidReceive {
//    NSLog(@"[RevMob Sample App] Install did receive.");
//}
//
//- (void)installDidFail {
//    NSLog(@"[RevMob Sample App] Install did fail.");
//}
@end