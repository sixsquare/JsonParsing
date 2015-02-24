//
//  WebImageView.m
//  ASIHttpDemo
//
//  Created by Hendy on 13-9-8.
//  Copyright (c) 2013年 Hendy. All rights reserved.
//

#import "WebImageView.h"
#import "ASIHTTPRequest.h"

@implementation WebImageView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

//同步请求
- (void)synchronous:(ASIHTTPRequest *)request
{
    [request startSynchronous];
    if (request.error) {
        NSLog(@"error : %@", request.error);
    } else {
        NSData *data = request.responseData;
        UIImage *image = [UIImage imageWithData:data];
        self.image = image;
    }
}

//异步请求
- (void)aSynchronous:(ASIHTTPRequest *)request
{
//    request.delegate = self;
    
    [request setCompletionBlock:^{
        NSData *data = request.responseData;
        UIImage *image = [UIImage imageWithData:data];
        self.image = image;
    }];
    
    [request setFailedBlock:^{
        NSLog(@"error : %@", request.error);
    }];
    
    [request startAsynchronous];
}

//***如果覆写了此方法, 在requestFinished的responseData会为空, 所以要在此方法处理data***
//- (void)request:(ASIHTTPRequest *)request didReceiveData:(NSData *)data
//{
//    NSLog(@"receive data size: %d", data.length);
//}

- (void)requestFinished:(ASIHTTPRequest *)request
{
    NSData *data = request.responseData;
//    NSLog(@"%d", data.length);
    UIImage *image = [UIImage imageWithData:data];
    self.image = image;
}

- (void)requestFailed:(ASIHTTPRequest *)request
{
    NSLog(@"error : %@", request.error);
}

- (void)setImageURL:(NSString *)url
{
    ASIHTTPRequest *request = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:url]];
    [request setRequestMethod:@"GET"];
    [request setTimeOutSeconds:30];
    
    //请求头
//    [request setRequestHeaders:(NSMutableDictionary *)];
    
    //Cookies
//    [request setRequestCookies:(NSMutableArray *)];
    
    //同步请求
//    [self synchronous:request];
    
    //异步请求
    [self aSynchronous:request];
}

@end
