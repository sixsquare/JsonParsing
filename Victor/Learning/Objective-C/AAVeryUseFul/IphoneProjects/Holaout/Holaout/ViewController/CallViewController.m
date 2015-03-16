//
//  CallViewController.m
//  Holaout
//
//  Created by Amit Parmar on 28/12/13.
//  Copyright (c) 2013 N-Tech. All rights reserved.
//

#import "CallViewController.h"
#import "SettingsViewController.h"
#import "AppDelegate.h"
#import "MessageViewController.h"
#import "AcceptCallViewController.h"

@implementation CallViewController

@synthesize lblUserName;
@synthesize lblTime;
@synthesize lblDay;
@synthesize userImageView;
@synthesize selectedFriend;

// test 34 user id
// 756167
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
    }
    return self;
}

- (void)viewDidLoad{
    [super viewDidLoad];
}

- (void) viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    
    NSString *stringPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)objectAtIndex:0];
    NSError *error = nil;
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:stringPath])
        [[NSFileManager defaultManager] createDirectoryAtPath:stringPath withIntermediateDirectories:NO attributes:nil error:&error];
    if(selectedFriend){
        lblUserName.text = [selectedFriend objectForKey:kContactName];
        NSString *fileName = [selectedFriend objectForKey:kContactImage];
        NSData *data = [NSData dataWithContentsOfFile:fileName];
        userImageView.image = [UIImage imageWithData:data];
    }
    else{
        NSDictionary *dictionary = [[NSUserDefaults standardUserDefaults] objectForKey:kStoredData];
        lblUserName.text = [dictionary objectForKey:kUserName];
        NSString *fileName = [stringPath stringByAppendingFormat:@"/profile.png"];
        NSData *data = [NSData dataWithContentsOfFile:fileName];
        userImageView.image = [UIImage imageWithData:data];
    }
}

- (IBAction)btnSpeakerClicked:(id)sender{
    if([sender tag] == 101){
       [AppDelegate appdelegate].videoChat.useHeadphone = NO;
        [sender setTag:102];
    }
    else{
        [AppDelegate appdelegate].videoChat.useHeadphone = YES;
        [sender setTag:101];
    }
}
- (IBAction)btnMuteClicked:(id)sender{
    if([sender tag] == 201){
        [AppDelegate appdelegate].videoChat.microphoneEnabled = NO;
        [sender setTag:202];
    }
    else{
        [AppDelegate appdelegate].videoChat.microphoneEnabled = YES;
        [sender setTag:201];
    }
    
}
- (IBAction)btnChatClicked:(id)sender{
    QBUUser *user = [QBUUser user];
    user.email = [selectedFriend objectForKey:kContactEmail];
    user.ID = [[selectedFriend objectForKey:kContactHolaoutId] intValue];
    user.phone = [selectedFriend objectForKey:KContactPhone];
    user.fullName = [selectedFriend objectForKey:kContactName];
    
    MessageViewController *messageViewController;
    if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone){
        messageViewController = [[MessageViewController alloc] initWithNibName:@"MessageViewController_iPhone" bundle:nil];
    }
    else{
        messageViewController = [[MessageViewController alloc] initWithNibName:@"MessageViewController_iPad" bundle:nil];
    }
    messageViewController.opponent = user;
    messageViewController.selectedFriend = selectedFriend;
    [self presentViewController:messageViewController animated:YES completion:nil];
}
- (IBAction)btnCallEndClicked:(id)sender{
    [[AppDelegate appdelegate].videoChat finishCall];
    [[QBChat instance] unregisterVideoChatInstance:[AppDelegate appdelegate].videoChat];
    [AppDelegate appdelegate].videoChat = nil;
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
@end
