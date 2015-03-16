//
//  AcceptCallViewController.m
//  Holaout
//
//  Created by Amit Parmar on 09/01/14.
//  Copyright (c) 2014 N-Tech. All rights reserved.
//

#import "AcceptCallViewController.h"
#import "SettingsViewController.h"
#import "LeftMenuViewController.h"
#import "FriendsViewController.h"

@implementation AcceptCallViewController

@synthesize lblUserName;
@synthesize lblTime;
@synthesize lblDay;
@synthesize userImageView;
@synthesize sessionId;
@synthesize userId;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)btnBackClicked:(id)sender{
    [self dismissViewControllerAnimated:YES completion:nil];
}
- (IBAction)btnSettingsClicked:(id)sender{
    SettingsViewController *settingsViewController;
    if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone){
        settingsViewController = [[SettingsViewController alloc] initWithNibName:@"SettingsViewController_iPhone" bundle:nil];
    }
    else{
        settingsViewController = [[SettingsViewController alloc] initWithNibName:@"SettingsViewController_iPad" bundle:nil];
    }
    [self presentViewController:settingsViewController animated:YES completion:nil];
}

- (IBAction)btnAcceptClicked:(id)sender{
    
    if([AppDelegate appdelegate].videoChat== nil){
         [AppDelegate appdelegate].videoChat = [[QBChat instance] createAndRegisterVideoChatInstanceWithSessionID:sessionId];
    }

    // setup custom capture
    //
	[AppDelegate appdelegate].videoChat.isUseCustomAudioChatSession = YES;
	[AppDelegate appdelegate].videoChat.isUseCustomVideoChatCaptureSession = YES;

    [[AppDelegate appdelegate].videoChat acceptCallWithOpponentID:userId conferenceType:QBVideoChatConferenceTypeAudio];
}
- (IBAction)btnRejectClicked:(id)sender{
    [AppDelegate appdelegate].videoChat = [[QBChat instance] createAndRegisterVideoChatInstanceWithSessionID:sessionId];
    [[AppDelegate appdelegate].videoChat rejectCallWithOpponentID:userId];
    [[QBChat instance] unregisterVideoChatInstance:[AppDelegate appdelegate].videoChat];
    [AppDelegate appdelegate].videoChat = nil;
    
    LeftMenuViewController *leftMenu;
    FriendsViewController *friendsViewController;
    if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone){
        leftMenu = [[LeftMenuViewController alloc] initWithNibName:@"LeftMenuViewController_iPhone" bundle:nil];
        friendsViewController = [[FriendsViewController alloc] initWithNibName:@"FriendsViewController_iPhone" bundle:nil];
    }
    else{
        leftMenu = [[LeftMenuViewController alloc] initWithNibName:@"LeftMenuViewController_iPad" bundle:nil];
        friendsViewController = [[FriendsViewController alloc] initWithNibName:@"FriendsViewController_iPad" bundle:nil];
    }
    UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:friendsViewController];
    UINavigationController *navController1 = [[UINavigationController alloc] initWithRootViewController:leftMenu];
    [navController1 setNavigationBarHidden:YES animated:YES];
    MFSideMenuContainerViewController *container = [MFSideMenuContainerViewController
                                                    containerWithCenterViewController:navController
                                                    leftMenuViewController:navController1
                                                    rightMenuViewController:nil];
    [AppDelegate appdelegate].window.rootViewController = container;
    [[AppDelegate appdelegate].window makeKeyAndVisible];
    
}

@end
