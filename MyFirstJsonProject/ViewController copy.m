//
//  ViewController.m
//  MyFirstJsonProject
//
//  Created by Victor on 2/24/15.
//  Copyright (c) 2015 Victor. All rights reserved.
//

#import "ViewController.h"
#import "JSON.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.jsonLabel.text = @"My Json Text";
    self.jsonMainDictionary = nil;
    NSString *str=@"http://api.kivaws.org/v1/loans/search.json?status=fundraising";
    NSURL *url=[NSURL URLWithString:str];
    NSData *data=[NSData dataWithContentsOfURL:url];
    NSError *error=nil;
    id response=[NSJSONSerialization JSONObjectWithData:data options:
                 NSJSONReadingMutableContainers error:&error];
    
    NSArray *loansArray;
    if(response){
        loansArray = [response mutableArrayValueForKeyPath:@"loans"];
        NSLog(@"Your idArray count: %d", loansArray.count);
    }

    NSString * strLocation = [[[loansArray lastObject] objectForKey:@"location"] objectForKey:@"country_code"];
    NSLog(@"Country Code: %@",strLocation);
    self.jsonLabel.text = strLocation;
//    for (NSDictionary * dict in loansArray) {
//        NSString * strLocation = [[dict objectForKey:@"location"] objectForKey:@"country_code"];
//        NSLog(@"Country Code: %@",strLocation);
//    }
   

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
