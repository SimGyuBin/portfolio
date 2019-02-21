/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/


//#define FINGER_TIP              true

#import "AppController.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "CustomNavigationController.h"
#import "FacebookViewController.h"
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <KakaoOpenSDK/KakaoOpenSDK.h>
#import <UserNotifications/UserNotifications.h>
#import "PixApp.h"
#import "PixSdkboxConfigManager.h"
#import "PixAccountsGoogleManager.h"
#import "PixOneSignalManager.h"

#define MY_APP_URL_KEY  @"pzle_purchase"

#if (FINGER_TIP == true)
#import "MBFingerTipWindow.h"
#endif

@interface AppController ()

@end

@implementation AppController

@synthesize window;

#pragma mark -
#pragma mark Application lifecycle



// cocos2d application instance
static AppDelegate s_sharedApplication;

@synthesize navigationController;
@synthesize rootViewController;
@synthesize facebookViewController;

+ (AppController*)appDelegate
{
    return (AppController*)[[UIApplication sharedApplication] delegate];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    
    //iOS6에서 세션끊어지는 상황 방지하기 위해 쿠키 설정. (iOS설정에서 사파리 쿠키 사용 설정도 필요)
    [[NSHTTPCookieStorage sharedHTTPCookieStorage] setCookieAcceptPolicy:NSHTTPCookieAcceptPolicyAlways];
    
#if (FINGER_TIP == true)
    MBFingerTipWindow *fingerTipWindow = [[MBFingerTipWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    fingerTipWindow.alwaysShowTouches = YES;
    window = fingerTipWindow;
#else
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
#endif
    
    // Use RootViewController to manage CCEAGLView
   
    facebookViewController = [[FacebookViewController alloc] init];
    
    // Use RootViewController manage CCEAGLView
    rootViewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
//    rootViewController.wantsFullScreenLayout = YES;
    
    navigationController = [[CustomNavigationController alloc] initWithRootViewController: rootViewController];
    navigationController.navigationBarHidden = TRUE;


    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: navigationController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:navigationController];
    }

    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden:true];
    
    // 원시스널 셋팅
    id notificationReceiverBlock = ^(OSNotification *notification) {
        NSLog(@"Received Notification - %@", notification.payload.notificationID);
        
        // 앱활성화 일때만 실행 됨
        OSNotificationPayload* payload = notification.payload;
        
        std::string strMessage = "";
        std::string strTitle = "";
        std::string strPush_event = "";
        bool blnActive = false;
        
        if(payload != nil)
        {
            NSString *body = payload.body;
            
            if(body != nil)
            {
                strMessage = [[body copy] UTF8String];
            }
            
            NSString *title = payload.title;
            
            if(title != nil)
            {
                strTitle = [title UTF8String];
            }
            
            blnActive = ONESIGNAL_MANAGER->getIntroFinish();
            
            if (payload.additionalData)
            {
                NSString *nsPush_event = [payload.additionalData objectForKey:@"push_event"];
                
                if (nsPush_event != nil)
                {
                    strPush_event = [nsPush_event UTF8String];
                }
            }
        }
        
        if(blnActive)
        {
            ONESIGNAL_MANAGER->pushEventExecution(strMessage, strTitle, strPush_event, blnActive);
        }
        else
        {
            ONESIGNAL_MANAGER->setOneSignalHandleNotification(strMessage, strTitle, strPush_event, blnActive);
        }
    };
    
    id notificationOpenedBlock = ^(OSNotificationOpenedResult *result) {
        // This block gets called when the user reacts to a notification received
        
        // 앱 비활성화 일때만 실행 됨
        OSNotificationPayload* payload = result.notification.payload;
        
        std::string strMessage = "";
        std::string strTitle = "";
        std::string strPush_event = "";
        bool blnActive = false;
        
        if(payload != nil)
        {
            NSString *body = payload.body;
            
            if(body != nil)
            {
                strMessage = [[body copy] UTF8String];
            }
            
            NSString *title = payload.title;
            
            if(title != nil)
            {
                strTitle = [title UTF8String];
            }
            
            blnActive = ONESIGNAL_MANAGER->getIntroFinish();
            
            if (payload.additionalData)
            {
                NSString *nsPush_event = [payload.additionalData objectForKey:@"push_event"];
                
                if (nsPush_event != nil)
                {
                    strPush_event = [nsPush_event UTF8String];
                }
            }
        }
        
        if(blnActive)
        {
            ONESIGNAL_MANAGER->pushEventExecution(strMessage, strTitle, strPush_event, blnActive);
        }
        else
        {
            ONESIGNAL_MANAGER->setOneSignalHandleNotification(strMessage, strTitle, strPush_event, blnActive);
        }
    };

    std::string strID = SDKBOXCONFING_MANAGER->getOneSignalID();
    
    [OneSignal initWithLaunchOptions:launchOptions
                               appId:[NSString stringWithUTF8String:strID.c_str()]
          handleNotificationReceived:notificationReceiverBlock
            handleNotificationAction:notificationOpenedBlock
                            settings:@{kOSSettingsKeyInAppAlerts:@NO}];
    
    [OneSignal IdsAvailable:^(NSString *userId, NSString *pushToken) {
        
        std::string strUserId = "";
        std::string strPushToken = "";
       
        if (userId != nil)
        {
            strUserId = [userId UTF8String];
        }
        
        if (pushToken != nil)
        {
            strPushToken = [pushToken UTF8String];
        }
    
        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([=](void) -> void {
            
            ONESIGNAL_MANAGER->init(strUserId,strPushToken);

        });
       
    }];
    
    [OneSignal promptLocation];
    
    // 배지 카운트 초기화
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
    
    // 카카오 셋팅
    KOSession *session = [KOSession sharedSession];
    session.presentedViewBarTintColor = [UIColor colorWithRed:0x2a / 255.0 green:0x2a / 255.0 blue:0x2a / 255.0 alpha:1.0];
    session.presentedViewBarButtonTintColor = [UIColor colorWithRed:0xe5 / 255.0 green:0xe5 / 255.0 blue:0xe5 / 255.0 alpha:1.0];
    
    
    // 페이스북 셋팅
    bool ret = [[FBSDKApplicationDelegate sharedInstance] application:application
                                        didFinishLaunchingWithOptions:launchOptions];
    
    return ret;
}


- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    NSString *token = [[deviceToken description] stringByTrimmingCharactersInSet: [NSCharacterSet characterSetWithCharactersInString:@"<>"]];
    token = [token stringByReplacingOccurrencesOfString:@" " withString:@""];
    
    NSLog(@"-->> TOKEN:%@",token);
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    // Respond to any push notification registration errors here.
    NSLog(@"Failed to get token, error: %@", error);
}

#pragma mark - Notify

- (void) application:(NSString*)message  additionalData:(NSDictionary*)additionalData isActive:(BOOL)isActive
{
    
}

- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window{
    
    if(PIXAPP->frameType == FRAME_HORIZONTAL)
    {
        return UIInterfaceOrientationMaskLandscapeRight;
    }
    else if(PIXAPP->frameType == FRAME_VERTICALSIZE)
    {
        return UIInterfaceOrientationMaskPortrait;
    }
    
    return UIInterfaceOrientationMaskPortrait;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
     //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    
    [KOSession handleDidBecomeActive];
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    
    if([rootViewController safariViewController])
    {
        // 구글 로그인
        [rootViewController dismissAuthorizationAnimated];
        ACCOUNTS_GOOGLE_MANAGER->resumeAuthorizationFlowWithURL([[url absoluteString] UTF8String]);
        return YES;
    }
    else
    {
        // 카카오 로그인
        if ([KOSession handleOpenURL:url]) {
            return YES;
        }
        
        // 결재
        NSString* scheme = [url scheme];
        NSString* query = [url query];
        
        if( scheme != nil && [scheme hasPrefix:MY_APP_URL_KEY] )
        {
            //imp_uid를 추출
            NSDictionary* query_map = [self parseQueryString:query];
            NSString* imp_uid = query_map[@"imp_uid"];
            NSString* m_redirect_url = query_map[@"m_redirect_url"];
            
            NSLog(@"imp_uid is %@", imp_uid);
            NSLog(@"m_redirect_url is %@", m_redirect_url);
            return YES;
        }
        else
        {
            return [[FBSDKApplicationDelegate sharedInstance] application:application
                                                              openURL:url
                                                    sourceApplication:sourceApplication
                                                           annotation:annotation];
        }
    }
}

- (NSDictionary *)parseQueryString:(NSString *)query
{
    NSMutableDictionary *dict = [[NSMutableDictionary alloc] initWithCapacity:6];
    NSArray *pairs = [query componentsSeparatedByString:@"&"];
    
    for (NSString *pair in pairs)
    {
        NSArray *elements = [pair componentsSeparatedByString:@"="];
        NSString *key = [[elements objectAtIndex:0] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        NSString *val = [[elements objectAtIndex:1] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        
        [dict setObject:val forKey:key];
    }
    
    return dict;
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
     NSLog(@"applicationDidReceiveMemoryWarning");
}


#if __has_feature(objc_arc)
#else
- (void)dealloc {
    [window release];
    [navigationController release];
    [rootViewController release];
    [facebookViewController release];
    [super dealloc];
}
#endif


@end
