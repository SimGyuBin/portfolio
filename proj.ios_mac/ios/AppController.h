#import <UIKit/UIKit.h>
#import <OneSignal/OneSignal.h>

@class RootViewController;
@class FacebookViewController;
@class CustomNavigationController;

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property (nonatomic, retain) CustomNavigationController *navigationController;
@property (nonatomic, retain) RootViewController *rootViewController;
@property (nonatomic, retain) FacebookViewController *facebookViewController;
@property (strong, nonatomic) OneSignal *oneSignal;

+ (AppController *)appDelegate;

@end

