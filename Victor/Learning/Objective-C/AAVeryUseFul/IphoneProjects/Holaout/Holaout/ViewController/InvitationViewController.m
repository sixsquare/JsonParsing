//
//  InvitationViewController.m
//  Holaout
//
//  Created by Amit Parmar on 09/12/13.
//  Copyright (c) 2013 N-Tech. All rights reserved.
//

#import "InvitationViewController.h"
#import "DataManager.h"
#import "MessageViewController.h"

@implementation InvitationViewController

@synthesize lblName;
@synthesize profileImgView;
@synthesize btnsendMessage;
@synthesize btnAddToContact;
@synthesize btnBlock;
@synthesize user;
@synthesize imgData;
@synthesize activityIndicator;
@synthesize contactArr;
@synthesize userId;


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
    }
    return self;
}

- (void)completedWithResult:(Result *)result{
    [activityIndicator stopAnimating];
    if(result.success && [result isKindOfClass:QBCFileDownloadTaskResult.class]){
        QBCFileDownloadTaskResult *res = (QBCFileDownloadTaskResult *)result;
        UIImage *profilePicture = [UIImage imageWithData:res.file];
        profileImgView.image = profilePicture;
        imgData = res.file;
        
        NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:user.fullName,kContactName,user.phone,KContactPhone,user.email,kContactEmail,@"1",kContactIsHolaout,userId,kContactHolaoutId,imgData,kContactImage, nil];
        
        for (int i=0; i<contactArr.count; i++)
        {
            NSString *str = [[contactArr objectAtIndex:i] valueForKey:kContactHolaoutId];
            if([str isEqualToString:userId]){
                [[DataManager sharedDataManager] updateContactsData:dictionary forRowId:[[contactArr objectAtIndex:i] objectForKey:KID]];
                
                break;
            }
        }
        
    }else{
        NSLog(@"Errors=%@", result.errors);
    }
}

- (UIStatusBarStyle)preferredStatusBarStyle
{
    return UIStatusBarStyleLightContent;
}

- (void)viewDidLoad{
    [super viewDidLoad];
    [activityIndicator startAnimating];
    
    [self setNeedsStatusBarAppearanceUpdate];
    
    [DataManager sharedDataManager].contactDelegate = self;
    NSDictionary *dictionary = [[NSUserDefaults standardUserDefaults] objectForKey:kStoredData];
    NSString *usersId = [dictionary objectForKey:kUserId];
    [[DataManager sharedDataManager] getHolaoutContactsData:usersId];
    
    lblName.text = user.fullName;
    profileImgView.layer.borderColor = [UIColor lightGrayColor].CGColor;
    profileImgView.layer.borderWidth = 3.0;
    profileImgView.layer.cornerRadius = 25.0;
    profileImgView.layer.masksToBounds = YES;
}

-(void)returnContactArray:(NSArray *)dataArray{
    NSLog(@"%@",dataArray);
    NSMutableArray *holaoutArray  = [[NSMutableArray alloc] init];
    
    for(int i=0;i<[dataArray count];i++){
        if([[[dataArray objectAtIndex:i] objectForKey:kISBlocked] intValue] != 1){
            if([[[dataArray objectAtIndex:i] objectForKey:kContactIsHolaout] intValue] == 1){
                [holaoutArray addObject:[dataArray objectAtIndex:i]];
            }
        }
        else
        {
            [holaoutArray addObject:[dataArray objectAtIndex:i]];
        }
    }
    contactArr=holaoutArray;
    userId = [NSString stringWithFormat:@"%d",user.ID];
    
    if([contactArr count]==0 )
    {
        [btnAddToContact setTitle:@"Add To Contact" forState:UIControlStateNormal];
        [btnAddToContact setTag:101];
    }
    
    for (int i=0; i<contactArr.count; i++)
    {
        NSString *str = [[contactArr objectAtIndex:i] valueForKey:kContactHolaoutId];
        
        if([str isEqualToString:userId]){
            
            [btnAddToContact setTitle:@"Remove From Contact" forState:UIControlStateNormal];
            [btnAddToContact setTag:102];
            
            if ([[[contactArr objectAtIndex:i] objectForKey:kISBlocked] intValue] != 1)//Blocked or Unblocked
            {
                [btnBlock setTitle:@"Block" forState:UIControlStateNormal];
                [btnBlock setTag:202];
            }
            else
            {
                [btnBlock setTitle:@"UnBlock" forState:UIControlStateNormal];
                [btnBlock setTag:201];
            }
            return;
            
        }
        else{
            [btnAddToContact setTitle:@"Add To Contact" forState:UIControlStateNormal];
            [btnAddToContact setTag:101];
        }
    }
}

- (void) viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
  
}

- (void)didReceiveMemoryWarning{
    [super didReceiveMemoryWarning];
}

- (IBAction)backButtonClicked:(id)sender{
    [self dismissViewControllerAnimated:YES completion:nil];
}
- (IBAction)sendMessageButtonClicked:(id)sender{
    MessageViewController *messageViewController;
    if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone){
        messageViewController = [[MessageViewController alloc] initWithNibName:@"MessageViewController_iPhone" bundle:nil];
    }
    else{
        messageViewController = [[MessageViewController alloc] initWithNibName:@"MessageViewController_iPad" bundle:nil];
    }
    NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:user.email,kContactEmail,user.phone,KContactPhone,user.fullName,kContactName,[NSString stringWithFormat:@"%d",user.ID],kContactHolaoutId, nil];
    messageViewController.opponent = user;
    messageViewController.selectedFriend = dictionary;
    [self presentViewController:messageViewController animated:YES completion:nil];
    
}
- (IBAction)addToContactButtonClicked:(id)sender{
    if([btnAddToContact tag] == 101 ){
        NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:user.fullName,kContactName,user.phone,KContactPhone,user.email,kContactEmail,@"1",kContactIsHolaout,userId,kContactHolaoutId, nil];
        
        [[QBChat instance] addUserToContactListRequest:[userId intValue]];
        NSUInteger userProfilePictureID = user.blobID;
        [QBContent TDownloadFileWithBlobID:userProfilePictureID delegate:self];
        [[DataManager sharedDataManager] insertContactsData:dictionary];
        [btnAddToContact setTitle:@"Remove From Contact" forState:UIControlStateNormal];
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:kAppTitle message:@"Contact added successfully" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [alert show];
    }
    else{
        for (int i=0; i<contactArr.count; i++)
        {
            NSString *str = [[contactArr objectAtIndex:i] valueForKey:kContactHolaoutId];
            if([str isEqualToString:userId]){
                userId = [[contactArr objectAtIndex:i] objectForKey:KID];
                break;
            }
        }
        
        [[DataManager sharedDataManager] deleteContactsDataForId:userId];
        [[QBChat instance] removeUserFromContactList:[userId intValue]];
        [btnAddToContact setTitle:@"Add To Contact" forState:UIControlStateNormal];
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:kAppTitle message:@"Contact removed successfully" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [alert show];
    }
}

- (IBAction)blockUserButtonClicked:(id)sender{
    for(int i=0;i<[contactArr count];i++)
    {
        NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:user.fullName,kContactName,user.phone,KContactPhone,user.email,kContactEmail,@"1",kContactIsHolaout,userId,kContactHolaoutId,@"1",kISBlocked,@"1",kContactId, nil];
        NSString *str = [[contactArr objectAtIndex:i] valueForKey:kContactHolaoutId];
        if ([btnBlock tag]==202) {
            if ([[[contactArr objectAtIndex:i] objectForKey:kISBlocked] intValue] != 1 && [str isEqualToString:userId]) {
                
                [btnBlock setTitle:@"Block" forState:UIControlStateNormal];
                [[QBChat instance] removeUserFromContactList:[userId intValue]];
                
                [[DataManager sharedDataManager] updateContactsData:dictionary forRowId:[[contactArr objectAtIndex:i] objectForKey:KID]];
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:kAppTitle message:@"Contact Blocked successfully" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
                [alert show];
                return;
            }
        }
        else if([btnBlock tag]==201 && [str isEqualToString:userId])
        {
            NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:user.fullName,kContactName,user.phone,KContactPhone,user.email,kContactEmail,@"1",kContactIsHolaout,userId,kContactHolaoutId,@"0",kISBlocked,@"1",kContactId, nil];
            [btnBlock setTitle:@"UnBlock" forState:UIControlStateNormal];
            [[QBChat instance] addUserToContactListRequest:[userId intValue]];
            [[DataManager sharedDataManager] updateContactsData:dictionary forRowId:[[contactArr objectAtIndex:i] objectForKey:KID]];
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:kAppTitle message:@"Contact UnBlocked successfully" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
            [alert show];
            return;
        }
    }
}

@end
