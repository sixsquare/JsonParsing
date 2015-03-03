//
//  IAPHelper.h
//  InAppPurchaseDemo
//
//  Created by Jennis on 07/09/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "StoreKit/StoreKit.h"

@protocol IAPTransactionDelegate;

@interface IAPHelper : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
//	NSSet * _productIdentifiers;    
//    NSArray * _products;
//    NSMutableSet * _purchasedProducts;
//    SKProductsRequest * _request;	
//	
//	id<IAPTransactionDelegate> _delegate;
}

@property (nonatomic,strong) NSSet *productIdentifiers;
@property (nonatomic,strong) NSArray * products;
@property (nonatomic,strong) NSMutableSet *purchasedProducts;
@property (nonatomic,strong) SKProductsRequest *request;
@property (nonatomic, assign) id<IAPTransactionDelegate> delegate;

//Singleton Instance
+ (IAPHelper *) sharedHelper;

//Initialization with product identifirers
- (id)initWithProductIdentifiers:(NSSet *)productIdentifiers;

//Request products from Apple Store
- (void)requestProducts;

//Buy Product
- (void)buyProduct:(SKProduct *)product;

//Load Purchased Products
-(void)loadPurchasedProducts;

//Transaction status
- (void)completeTransaction:(SKPaymentTransaction *)transaction; 
- (void)restoreTransaction:(SKPaymentTransaction *)transaction;
- (void)failedTransaction:(SKPaymentTransaction *)transaction;
- (void)provideContent:(NSString *)productIdentifier;

@end

@protocol IAPTransactionDelegate <NSObject>

@optional

//Products loaded
- (void)finishedProductLoading;

//Product purchase status delegates
-(void)finishdProductPurchased:(NSString*)transaction;
-(void)failedProductPurchased:(SKPaymentTransaction*)transaction;

@end