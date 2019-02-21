//
//  PixNativeUtils_ios.m
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 26..
//
//

#import <Foundation/Foundation.h>
#import <SystemConfiguration/SCNetworkReachability.h>
#include <sys/sysctl.h>
#include <netinet/in.h>
#import <Social/Social.h>
#import "NSData+AES.h"
#import "NSData+Base64.h"
#import "NSString+Base64.h"
#include "PixNativeUtils.h"
#include "AppController.h"
#include "RootViewController.h"
#include "FacebookViewController.h"
#include "CustomNavigationController.h"
#include "../Scene/PixApp.h"
#include "../Common.h"
#include "../UIControl/PixToast.h"
#include "../Manager/PixFacebookManager.h"
#include "../Manager/PixKakaoManager.h"
#include <regex>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <KakaoOpenSDK/KakaoOpenSDK.h>
#import <OneSignal/OneSignal.h>

static std::function<void()> _callBack = nullptr;
static bool isPathOfAllImages = false;

void PixNativeUtils::nativeCallPathOfAllImages(pathOfAllImages_CallBack callback)
{
    if(isPathOfAllImages)
        return;
    
    _callBack = callback;
    isPathOfAllImages = true;
    
    [[AppController appDelegate].rootViewController pathOfAllImages];
 
}

void PixNativeUtils::nativeCallPathOfAllImagesComplete()
{    
    if(_callBack)
        (_callBack)();

    _callBack = nullptr;
    isPathOfAllImages = false;
}

void PixNativeUtils::nativeCallGetThumbnailImage(const std::string &strUrl, int width, int height)
{
    [[AppController appDelegate].rootViewController getThumbnailImage:strUrl width:width height:height];
}

void PixNativeUtils::nativeCallGetFullScreenImage(const std::string &strUrl, int width, int height)
{
    [[AppController appDelegate].rootViewController getFullScreenImage:strUrl width:width height:height];
}

void PixNativeUtils::nativeCallGetUploadImage(const std::string &strUrl)
{
    [[AppController appDelegate].rootViewController getUploadImage:strUrl];
}

void PixNativeUtils::nativeCallChangeOrientation(int flag)
{
    [[AppController appDelegate].navigationController changeOrientation:flag];
}

Sprite* PixNativeUtils::nativeCallTransformImage(const unsigned char * data, ssize_t dataLen)
{
    Sprite* sprite = nullptr;
    
    NSData *nsData = [NSData dataWithBytes:data length:dataLen];
    UIImage *pImage = [UIImage imageWithData:nsData];
    
    CGFloat fWidth = pImage.size.width;
    CGFloat fHeight = pImage.size.height;
    
    UIImage *transformImage = nil;
    
    if(fWidth > kMaxTextureSize || fHeight > kMaxTextureSize)
    {
        float tw = ((float) kMaxTextureSize / ((float) fWidth));
        float th = ((float) kMaxTextureSize / ((float) fHeight));
        
        float fScale = 1.0f;
        
        if (tw > th)
        {
            fScale = tw;
        }
        else
        {
            fScale = th;
        }
        
        float resizeWidth = pImage.size.width * fScale;
        float resizeHeight = pImage.size.height * fScale;
        
        UIGraphicsBeginImageContext(CGSizeMake(resizeWidth, resizeHeight));
        CGContextRef context = UIGraphicsGetCurrentContext();
        
        CGContextTranslateCTM(context, 0.0, resizeHeight);
        CGContextScaleCTM(context, 1.0, -1.0);
        CGContextDrawImage(context, CGRectMake(0.0, 0.0, resizeWidth, resizeHeight), [pImage CGImage]);
        
        transformImage = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
    }
    else
    {
        transformImage = pImage;
    }
    
    if(transformImage)
    {
        NSData *imageData = UIImageJPEGRepresentation(transformImage, 1.0f);
        NSUInteger size = [imageData length];
        uint8_t *buff = (uint8_t *)malloc(sizeof(uint8_t)*size);
        memcpy(buff, [imageData bytes], size);
        
        Image *image = new Image();
        bool imageResult = image->initWithImageData(buff, size);
        CCASSERT(imageResult, "Failed to create image from nativeCallTransformImage!");
        free(buff);
        
        Texture2D *texture = new Texture2D();
        texture->initWithImage(image);
        texture->setAliasTexParameters();
        image->release();
        
        sprite = Sprite::createWithTexture(texture);
        texture->release();
    }
    
    return sprite;
}

std::string PixNativeUtils::nativeCallAES256Encrypt(const std::string &plainText, const std::string &key)
{
    NSString *strKey = [NSString stringWithUTF8String:key.c_str()];
    NSString *strPlainText = [NSString stringWithUTF8String:plainText.c_str()];
    NSString *base64Text;
    NSData *cipherData;
    
    cipherData = [[strPlainText dataUsingEncoding:NSUTF8StringEncoding] AES256EncryptWithKey:strKey];
    base64Text = [cipherData base64EncodedString];

    return [base64Text UTF8String];
}

std::string PixNativeUtils::nativeCallAES256Decrypt(const std::string &base64Text, const std::string &key)
{
    NSString *strKey = [NSString stringWithUTF8String:key.c_str()];
    NSString *plainText;
    NSString *strBase64Text = [NSString stringWithUTF8String:base64Text.c_str()];
    NSData *cipherData;
    
    cipherData = [strBase64Text base64DecodedData];
    plainText  = [[NSString alloc] initWithData:[cipherData AES256DecryptWithKey:strKey] encoding:NSUTF8StringEncoding];

    return [plainText UTF8String];
}

std::string PixNativeUtils::nativeCallAES256Encrypt(const std::string &plainText)
{
    return PixNativeUtils::nativeCallAES256Encrypt(plainText,ENCRYPT_KEY);
}

std::string PixNativeUtils::nativeCallAES256Decrypt(const std::string &base64Text)
{
    return PixNativeUtils::nativeCallAES256Decrypt(base64Text,ENCRYPT_KEY);
}

void PixNativeUtils::nativeCallClipboard(const std::string &paste, const std::string &msg)
{
    NSString *nsStr = [NSString stringWithCString:paste.c_str()
                                         encoding:NSUTF8StringEncoding];
    
    [[UIPasteboard generalPasteboard] setString:nsStr];
    
    PixToast::showToast(msg.c_str());
}

void PixNativeUtils::nativeCallBrowser(const std::string &strUrl)
{
     NSString *url = [NSString stringWithUTF8String:strUrl.c_str()];
     [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
}

void PixNativeUtils::nativeCallSaveImage(cocos2d::Data *pData, const std::string &strFileName, const std::string &msg)
{
    NSData *nsData = [NSData dataWithBytes:pData->getBytes() length:pData->getSize()];
    UIImage *image = [UIImage imageWithData:nsData];
    UIImageWriteToSavedPhotosAlbum(image, nil, nil, nil);
    
    if(!msg.empty())
    {
        PixToast::showToast(msg.c_str());
    }
}

std::string PixNativeUtils::getDeviceId()
{
    CFUUIDRef uuid = CFUUIDCreate(kCFAllocatorDefault);
    NSString *UUIDValue = (NSString*)CFUUIDCreateString(kCFAllocatorDefault, uuid);
    
    std::string strRet = [UUIDValue UTF8String];
    
    return strRet;
}

std::string PixNativeUtils::getAppVersionString()
{
     return [[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"] UTF8String];
}

std::string PixNativeUtils::getOsVersionString()
{
    return  [[[UIDevice currentDevice] systemVersion] UTF8String];
}

int PixNativeUtils::getDeviceWidth()
{
    return [[UIScreen mainScreen] bounds].size.width;
}

int PixNativeUtils::getDeviceHeight()
{
    return [[UIScreen mainScreen] bounds].size.height;
}

std::string PixNativeUtils::getManuFacturer()
{
    return "Apple";
}

std::string PixNativeUtils::getModelName()
{
    return [[[UIDevice currentDevice] model] UTF8String];
}

std::string PixNativeUtils::getDeviceBrand()
{
    return "Apple";
}

// 아이폰 남은용량 체크(MB)
int PixNativeUtils::getFreeDisk()
{
    NSDictionary* FileAttrs = [[NSFileManager defaultManager] attributesOfFileSystemForPath:NSHomeDirectory() error:nil];
    
    NSNumber* freeSizeByte = [FileAttrs objectForKey:NSFileSystemFreeSize];
    int freeSize = [freeSizeByte doubleValue] / 1024 / 1024;
    
    return freeSize;
}

bool PixNativeUtils::isWiFi()
{
    struct sockaddr_in zeroAddr;
    bzero(&zeroAddr, sizeof(zeroAddr));
    zeroAddr.sin_len = sizeof(zeroAddr);
    zeroAddr.sin_family = AF_INET;
    
    SCNetworkReachabilityRef target = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddr);
    
    SCNetworkReachabilityFlags flag;
    SCNetworkReachabilityGetFlags(target, &flag);
    
    if(flag & kSCNetworkReachabilityFlagsIsWWAN){
        return false;
    }else {
        return true;
    }
}

void PixNativeUtils::goURL(const char* url)
{
    NSString *urlBase = [NSString stringWithUTF8String:url];
    
    NSURL *appStoreUrl = [NSURL URLWithString:urlBase];
    [[UIApplication sharedApplication] openURL:appStoreUrl];
}

void PixNativeUtils::nativeCallWriteMail(const std::string &strUrl)
{
    std::string strMail = "mailto://";
    strMail.append(strUrl);
    
    PixNativeUtils::goURL(strMail.c_str());
}

bool PixNativeUtils::nativeCallPurchase(const std::string &strUrl)
{
    //ISP호출인지부터 체크
    NSString *URLString = [NSString stringWithCString:strUrl.c_str()
                                             encoding:NSUTF8StringEncoding];
    
    //APP STORE URL 경우 openURL 함수를 통해 앱스토어 어플을 활성화 한다.
    BOOL bAppStoreURL = ([URLString rangeOfString:@"phobos.apple.com" options:NSCaseInsensitiveSearch].location != NSNotFound);
    BOOL bAppStoreURL2 = ([URLString rangeOfString:@"itunes.apple.com" options:NSCaseInsensitiveSearch].location != NSNotFound);
    if(bAppStoreURL || bAppStoreURL2)
    {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:URLString]];
        return true;
    }
    
    //ISP 호출하는 경우
    if([URLString hasPrefix:@"ispmobile://"])
    {
        NSURL *appURL = [NSURL URLWithString:URLString];
        
        if([[UIApplication sharedApplication] canOpenURL:appURL])
        {
            [[UIApplication sharedApplication] openURL:appURL];
        }
        else
        {
            [[AppController appDelegate].rootViewController showAlertViewWithEvent:@"모바일 ISP가 설치되어 있지 않아\nApp Store로 이동합니다." tagNum: 99];
            return true;
        }
    }
    
    //기타(금결원 실시간계좌이체 등)
    NSString *strHttp = @"http://";
    NSString *strHttps = @"https://";
    NSString *strAboutBlank = @"about:blank";
    NSString *reqUrl = [[NSURL URLWithString:URLString] absoluteString];
    NSLog(@"nativeCallPurchase url : %@",reqUrl);
    
    if (!([reqUrl hasPrefix:strHttp]) && !([reqUrl hasPrefix:strHttps]) && !([reqUrl hasPrefix:strAboutBlank]))
    {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:URLString]];
        return true;
    }
    
    return false;
}

bool PixNativeUtils::nativeCallHasPrefix(const std::string &strUrl, const std::string &strKey)
{
    NSString *URLString = [NSString stringWithCString:strUrl.c_str()
                                             encoding:NSUTF8StringEncoding];
    
    NSString *KEYString = [NSString stringWithCString:strKey.c_str()
                                             encoding:NSUTF8StringEncoding];
    
    if([URLString hasPrefix:KEYString])
    {
        return true;
    }
    
    return false;
}

bool PixNativeUtils::isEmailPatternMatching(const std::string &str)
{
    NSString *nsStr = [NSString stringWithCString:str.c_str()
                                         encoding:NSUTF8StringEncoding];
    
    const char *tmp = [nsStr cStringUsingEncoding:NSUTF8StringEncoding];
    if (nsStr.length != strlen(tmp))
    {
        return false;
    }
    
//    NSString *check = @"([0-9a-zA-Z_-]+)@([0-9a-zA-Z_-]+)(\\.[0-9a-zA-Z_-]+){1,2}";
    NSString *check = @"^[_a-zA-Z0-9-\\.]+@[\\.a-zA-Z0-9-]+\\.[a-zA-Z]+$";
    NSRange match = [nsStr rangeOfString:check options:NSRegularExpressionSearch];
   
    if (NSNotFound == match.location)
    {
        return false;
    }
    
    return true;
}

bool PixNativeUtils::isPhoneNumberPatternMatching(const std::string &str)
{
    NSString *nsStr = [NSString stringWithCString:str.c_str()
                                         encoding:NSUTF8StringEncoding];
    
    const char *tmp = [nsStr cStringUsingEncoding:NSUTF8StringEncoding];
    if (nsStr.length != strlen(tmp))
    {
        return false;
    }
    
    NSString *check = @"^[0-9-]{10,14}$";
    NSRange match = [nsStr rangeOfString:check options:NSRegularExpressionSearch];
    
    if (NSNotFound == match.location)
    {
        return false;
    }
    
    return true;
}

bool PixNativeUtils::isPasswordPatternMatching(const std::string &str)
{
    NSString *nsStr = [NSString stringWithCString:str.c_str()
                                         encoding:NSUTF8StringEncoding];
    
    const char *tmp = [nsStr cStringUsingEncoding:NSUTF8StringEncoding];
    if (nsStr.length != strlen(tmp))
    {
        return false;
    }
    
    NSString *check = @"^[A-Za-z0-9]{5,15}";
    NSRange match = [nsStr rangeOfString:check options:NSRegularExpressionSearch];
    
    if (NSNotFound == match.location)
    {
        return false;
    }
    
    return true;
}

bool PixNativeUtils::checkValidateString(const std::string &str)
{
    NSString *nsStr = [NSString stringWithCString:str.c_str()
                                         encoding:NSUTF8StringEncoding];
    
    if (!nsStr)
    {
        return true;
    }
    
    NSString *ptn = @"^[\\.,<>?/'\"~*&(){}|_`:;!@#$%^*+=₩\\-\\[\\]\\\\ㆍ]*$";
    NSRange checkRange = [nsStr rangeOfString:ptn options:NSRegularExpressionSearch];
    
    if (checkRange.length == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

#pragma mark - Facebook

void PixNativeUtils::nativeCallFacebookLogin()
{
    FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
    [login  logInWithReadPermissions: @[@"public_profile",@"email"]     fromViewController:[[AppController appDelegate] rootViewController]     handler:^(FBSDKLoginManagerLoginResult *result, NSError *error)
     {
         if (error)
         {
             FACEBOOK_MANAGER->onLogin(false, "Process error");
         }
         else if (result.isCancelled)
         {
             FACEBOOK_MANAGER->onLogin(false, "Cancelled");
         }
         else
         {
             FACEBOOK_MANAGER->onLogin(true, "connection result");
         }
     }];
}

void PixNativeUtils::nativeCallFacebookLogOut()
{
    FBSDKLoginManager *manager = [[FBSDKLoginManager alloc] init];
    [manager logOut];
}

bool PixNativeUtils::nativeCallFacebookLoginCheck()
{
    if ([FBSDKAccessToken currentAccessToken])
    {
        // User is logged in
        return true;
    }
    else
    {
        return false;
    }
}

void PixNativeUtils::nativeCallFacebookMyInfo()
{
    FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc] initWithGraphPath:@"/me"
                                                                   parameters:@{@"fields":@"id, email, picture, birthday, gender, link, first_name, last_name, name"}
                                                                   HTTPMethod:@"GET"];
    
    [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
        
        NSLog(@"connection result - %@",result);

        std::map<std::string, std::string> userInfo;
        
        std::string strId =  ([result objectForKey:@"id"] != nil) ? [[result objectForKey:@"id"] UTF8String] : "";
        std::string strEmail = ([result objectForKey:@"email"] != nil) ? [[result objectForKey:@"email"] UTF8String] : "";
        std::string strBirthday = ([result objectForKey:@"birthday"] != nil) ? [[result objectForKey:@"birthday"] UTF8String] : "";
        std::string strGender = ([result objectForKey:@"gender"] != nil) ? [[result objectForKey:@"gender"] UTF8String] : "";
        std::string strLink = ([result objectForKey:@"link"] != nil) ? [[result objectForKey:@"link"] UTF8String] : "";
        std::string strLast_name = ([result objectForKey:@"last_name"] != nil) ? [[result objectForKey:@"last_name"] UTF8String] : "";
        std::string strFirst_name = ([result objectForKey:@"first_name"] != nil) ? [[result objectForKey:@"first_name"] UTF8String] : "";
        std::string strName = ([result objectForKey:@"name"] != nil) ? [[result objectForKey:@"name"] UTF8String] : "";
        
        userInfo["id"] = strId;
        userInfo["email"] = strEmail;
        userInfo["birthday"] = strBirthday;
        userInfo["gender"] = strGender;
        userInfo["link"] = strLink;
        userInfo["name"] = strName;
        
        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([=](void) -> void {
            
            FACEBOOK_MANAGER->onGetUserInfo(userInfo);
            
        });

    }];
}

void PixNativeUtils::nativeCallFacebookonUserAlbums()
{
    std::string url = "me/albums";
    url.append("?fields=count,name");
    
    NSString *path = [NSString stringWithCString:url.c_str()
                                        encoding:[NSString defaultCStringEncoding]];
    
    FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc]
                                   initWithGraphPath:path
                                   parameters:nil
                                   HTTPMethod:@"GET"];
   
    [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection,
                                           id result,
                                           NSError *error) {
        
        NSError *jsonError;
        NSData *jsonData = [NSJSONSerialization  dataWithJSONObject:result options:0 error:&jsonError];
        std::string strData = "";
     
        if(jsonData)
        {
            NSString *jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
            strData = [jsonString UTF8String];

        }
        
        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([=](void) -> void {
            
            if(strData.empty())
            {
                FACEBOOK_MANAGER->onGetUserAlbums(false, "");
            }
            else
            {
                FACEBOOK_MANAGER->onGetUserAlbums(true, strData);
            }

        });
        
    }];
}

void PixNativeUtils::nativeCallFacebookonUserImages(const std::string &strAlbumID)
{
    std::string url = std::string("/").append(strAlbumID);
    url.append("/photos");
    url.append("?fields=name,source,id,updated_time,width,height,picture");
    
    NSString *path = [NSString stringWithCString:url.c_str()
                                       encoding:[NSString defaultCStringEncoding]];
    
    FBSDKGraphRequest *request = [[FBSDKGraphRequest alloc]
                                  initWithGraphPath:path
                                  parameters:nil
                                  HTTPMethod:@"GET"];
    
    [request startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
        
        NSError *jsonError;
        NSData *jsonData = [NSJSONSerialization  dataWithJSONObject:result options:0 error:&jsonError];
        std::string strData = "";
        
        if(jsonData)
        {
            NSString *jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
            strData = [jsonString UTF8String];
        }
       
        auto scheduler = cocos2d::Director::getInstance()->getScheduler();
        scheduler->performFunctionInCocosThread([=](void) -> void {
            
            if(strData.empty())
            {
                FACEBOOK_MANAGER->onGetUserImages(false, "", "");

            }
            else
            {
                FACEBOOK_MANAGER->onGetUserImages(true, strAlbumID.c_str(), strData);
            }
            
        });
        
    }];
}

void PixNativeUtils::nativeCallFacebookShare(const std::string &strUrl)
{
    [[AppController appDelegate].facebookViewController facebookShare:strUrl];
}

void PixNativeUtils::nativeCallFacebookSharePhotoData(cocos2d::Data *pData)
{
    [[AppController appDelegate].facebookViewController facebookSharePhotoData:pData];
}

void PixNativeUtils::nativeCallFacebookSharePhotoPath(const std::string &strPath)
{
    [[AppController appDelegate].facebookViewController facebookSharePhotoPath:strPath];
}

#pragma mark - Google

void PixNativeUtils::nativeCallAccountsGoogle(const std::string &strUrl)
{
    [[AppController appDelegate].rootViewController accountsGoogle:strUrl];
}

#pragma mark - kakao

void PixNativeUtils::nativeCallKakaoLogin()
{
    KOSession *session = [KOSession sharedSession];
    
    if (session.isOpen) {
        [session close];
    }
    
    PIXAPP->showLoading();
    
    session.presentingViewController = [AppController appDelegate].navigationController;
    [session openWithCompletionHandler:^(NSError *error) {
        session.presentingViewController = nil;
        
        if (!session.isOpen) {
            
            PIXAPP->hideLoading();
            
            // 로그인 실패
            
            switch (error.code) {
                case KOErrorCancelled:
                    break;
                default:
                    [[[UIAlertView alloc] initWithTitle:@"에러" message:error.description delegate:nil cancelButtonTitle:@"확인" otherButtonTitles:nil, nil] show];
                    break;
            }
        }
        else
        {
            // 로그인 성공
            
            PIXAPP->showLoading();
            
            [KOSessionTask meTaskWithCompletionHandler:^(KOUser* result, NSError *error) {
                if (result) {
                    // success
                    
                    NSLog(@"userId=%@", result.ID);
                    NSLog(@"nickName=%@", [result propertyForKey:KOUserNicknamePropertyKey]);
                    std::string strUserId = "";
//                    std::string strUserId = [[NSString stringWithFormat:@"%d", [result.ID intValue]] UTF8String];
                    std::string strNickName = [[result propertyForKey:KOUserNicknamePropertyKey] UTF8String];
                    std::string strEmail = "";
                    
                    if (result.email) {
                        NSLog(@"email=%@", result.email);
                        strEmail = [result.email UTF8String];
                        strUserId = strEmail;
                        // 이메일을 아이디로 처리함
                    } else {
                        // disagreed
                    }
                    
                    KAKAO_MANAGER->setUserID(strUserId);
                    KAKAO_MANAGER->setUserName(strNickName);
                    KAKAO_MANAGER->setUserEmail(strEmail);
                    
                    KAKAO_MANAGER->kakaoLoginSuccess();
                    
                } else {
                    // failed
                    
                    PIXAPP->hideLoading();
                }
            }];
        }
    }];
}

void PixNativeUtils::nativeCallKakaoLogout()
{
    [KOSessionTask unlinkTaskWithCompletionHandler:^(BOOL success, NSError *error) {
        if (success) {
            // success
            NSLog(@"unlinkTaskWithCompletionHandler success");
        } else {
            // failed
            NSLog(@"unlinkTaskWithCompletionHandler failed");
        }
    }];
}

#pragma mark - OneSignal

void PixNativeUtils::nativeCallOneSignalsetSubscription(bool enable)
{
    [OneSignal setSubscription:(enable == true) ? YES : NO];
}

#pragma mark - for Android
void PixNativeUtils::nativeCallAndroidDestroy() {};
void PixNativeUtils::nativeCallAndroidToast(const std::string &msg) {};
std::string PixNativeUtils::getPhoneNumber() { return ""; };
std::string PixNativeUtils::getJavaString(const char *methodName) { return ""; };
void PixNativeUtils::nativeCallIntentURIstartActivity(const std::string &url) {};
bool PixNativeUtils::nativeCallstartsWith(const std::string &strUrl, const std::string &strKey) { return false; };
bool PixNativeUtils::isVirtualButton() { return false; }
void PixNativeUtils::nativeCallGoogleSignIn() {};
void PixNativeUtils::nativeCallGoogleSignOut() {};
void PixNativeUtils::nativeCallIntroFinish(bool var) {};
