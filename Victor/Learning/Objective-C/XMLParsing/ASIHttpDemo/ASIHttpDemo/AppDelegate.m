//
//  AppDelegate.m
//  ASIHttpDemo
//
//  Created by Hendy on 13-9-8.
//  Copyright (c) 2013年 Hendy. All rights reserved.
//

#import "AppDelegate.h"
#import "WebImageView.h"

@implementation AppDelegate

- (void)dealloc
{
    [_window release];
    [super dealloc];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    // Override point for customization after application launch.
    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    
    _imageView = [[WebImageView alloc] initWithFrame:CGRectMake(20, 40, 280, 350)];
    _imageView.backgroundColor = [UIColor cyanColor];
    _imageView.clipsToBounds = YES;
    _imageView.contentMode = UIViewContentModeScaleAspectFill;
    [self.window addSubview:_imageView];
    
    UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    button.frame = CGRectMake(30, 410, 100, 40);
    [button setTitle:@"访问网络图片" forState:UIControlStateNormal];
    [button addTarget:self action:@selector(getImageFromNet) forControlEvents:UIControlEventTouchUpInside];
    [self.window addSubview:button];
    
    return YES;
}

- (void)getImageFromNet
{
    //加载网络图片
    NSString *url = @"http://h.hiphotos.baidu.com/album/w%3D2048/sign=979e129794cad1c8d0bbfb274b066609/5366d0160924ab1830fbe38f34fae6cd7a890b9c.jpg";
    
    //多线程方式加载
    [_imageView setImageURL:url];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    [_imageView release];
}

@end
