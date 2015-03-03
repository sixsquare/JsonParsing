//
//  IAPHelper.m
//  InAppPurchaseDemo
//
//  Created by Jennis on 07/09/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "IAPHelper.h"
#define kInAppProductIdentifierRemoveAd @"com.r3app.tracelearning.remove.ads"
@implementation IAPHelper

static IAPHelper *_sharedHelper;

#pragma mark -
#pragma mark Singleton instance

+ (IAPHelper *) sharedHelper
{    
    if (_sharedHelper != nil)
        return _sharedHelper;
	
    _sharedHelper = [[IAPHelper alloc] init];
    return _sharedHelper;
}

#pragma mark -
#pragma mark Initialization and dealloc

- (id)init {    
	if ((self = [super init])) {
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
		NSSet *productIdentifiers = [NSSet setWithObjects:kInAppProductIdentifierRemoveAd,nil];
		return [self initWithProductIdentifiers:productIdentifiers];
	}
    return self;
}

-(id)initWithProductIdentifiers:(NSSet *)productIdentifiers {
    if ((self = [super init]))
        _productIdentifiers = productIdentifiers; // Store product identifiers
	
    return self;
}

#pragma mark -
#pragma mark Load purchased products

-(void)loadPurchasedProducts {
	// Check for previously purchased products
	NSMutableSet * purchasedProducts = [NSMutableSet set];
	for (NSString * productIdentifier in _productIdentifiers) {
		BOOL productPurchased = [[NSUserDefaults standardUserDefaults] boolForKey:productIdentifier];
		if (productPurchased) 
			[purchasedProducts addObject:productIdentifier];
	}
	self.purchasedProducts = purchasedProducts;	
}

#pragma mark -
#pragma mark Product request and response

- (void)requestProducts {    
    _request = [[SKProductsRequest alloc] initWithProductIdentifiers:_productIdentifiers];
    _request.delegate = self;
    [_request start];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    _request = nil;
    _products = response.products;
    
    for(SKProduct *product in _products)
        NSLog(@"Product details %@,%@,%@,%f...", product.productIdentifier,product.localizedDescription,product.localizedTitle,[product.price floatValue]);
    
	if ([self.delegate respondsToSelector:@selector(finishedProductLoading)])
		[self.delegate finishedProductLoading];
}

- (void)recordTransaction:(SKPaymentTransaction *)transaction {
    
}

#pragma mark -
#pragma mark Payment transaction

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    for (SKPaymentTransaction *transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
            default:
                break;
        }
    }
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction  {
    NSLog(@"completeTransaction...");
    [self recordTransaction: transaction];
    [self provideContent: transaction.payment.productIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction  {
    NSLog(@"restoreTransaction...");    
    [self recordTransaction: transaction];
    [self provideContent: transaction.originalTransaction.payment.productIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];    
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction 
{
    if (transaction.error.code != SKErrorPaymentCancelled)
        NSLog(@"Transaction error: %@", transaction.error.localizedDescription);

	if ([self.delegate respondsToSelector:@selector(failedProductPurchased:)])
		[self.delegate failedProductPurchased:transaction];

    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)provideContent:(NSString *)productIdentifier
{
    [[NSUserDefaults standardUserDefaults] setBool:TRUE forKey:productIdentifier];
    [[NSUserDefaults standardUserDefaults] synchronize];
    [_purchasedProducts addObject:productIdentifier];

	NSLog(@"Toggled flag for: %@ and count->%d",productIdentifier,_purchasedProducts.count);

	if ([self.delegate respondsToSelector:@selector(finishdProductPurchased:)])
		[self.delegate finishdProductPurchased:productIdentifier];
}

#pragma mark -
#pragma mark Buy Product

- (void)buyProduct:(SKProduct *)product  {
    NSLog(@"Buying %@,%@,%@,%f...", product.productIdentifier,product.localizedDescription,product.localizedTitle,[product.price floatValue]);
    SKPayment *payment = [SKPayment paymentWithProduct:product];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

@end
