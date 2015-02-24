//
//  WebImageView.h
//  ASIHttpDemo
//
//  Created by Hendy on 13-9-8.
//  Copyright (c) 2013年 Hendy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ASIHTTPRequestDelegate.h"

@interface WebImageView : UIImageView <ASIHTTPRequestDelegate>

- (void)setImageURL:(NSString *)url;

@end
