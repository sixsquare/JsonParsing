//
//  ViewController.h
//  MyFirstJsonProject
//
//  Created by Victor on 2/24/15.
//  Copyright (c) 2015 Victor. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
@property (strong, nonatomic) IBOutlet UILabel *jsonLabel;

@property(strong,nonatomic) NSDictionary  *jsonMainDictionary;

- (NSDictionary *) indexKeyedDictionaryFromArray:(NSMutableArray *)array;

@end

