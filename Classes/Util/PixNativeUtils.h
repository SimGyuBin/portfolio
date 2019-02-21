//
//  PixNativeUtils.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 26..
//
//

// 단말기 정보를 가저오는 브릿지 클래스

#ifndef PixNativeUtils_h
#define PixNativeUtils_h

#include "cocos2d.h"

USING_NS_CC;

static std::string native_root = "org/cocos2dx/cpp/Natives";

typedef const std::function<void()>& pathOfAllImages_CallBack;

class PixNativeUtils
{
public:
    
    static void nativeCallPathOfAllImages(pathOfAllImages_CallBack callback);
    static void nativeCallPathOfAllImagesComplete();
    static void nativeCallGetThumbnailImage(const std::string &strUrl, int width, int height);
    static void nativeCallGetFullScreenImage(const std::string &strUrl, int width, int height);
    static void nativeCallGetUploadImage(const std::string &strUrl);
    static void nativeCallChangeOrientation(int flag);
    static Sprite* nativeCallTransformImage(const unsigned char * data, ssize_t dataLen);
    static std::string nativeCallAES256Encrypt(const std::string &plainText, const std::string &key);
    static std::string nativeCallAES256Decrypt(const std::string &base64Text, const std::string &key);
    static std::string nativeCallAES256Encrypt(const std::string &plainText);
    static std::string nativeCallAES256Decrypt(const std::string &base64Text);
    static void nativeCallClipboard(const std::string &paste, const std::string &msg);
    static void nativeCallBrowser(const std::string &strUrl);
    static void nativeCallSaveImage(cocos2d::Data *pData, const std::string &strFileName, const std::string &msg);
    
    static std::string getDeviceId();
    static std::string getAppVersionString();
    static std::string getOsVersionString();
    static int getDeviceWidth();
    static int getDeviceHeight();
    static std::string getManuFacturer();
    static std::string getModelName();
    static std::string getDeviceBrand();
    static int getFreeDisk();
    static bool isWiFi();
    static void goURL(const char* url);
    static void nativeCallWriteMail(const std::string &strUrl);
    static bool nativeCallPurchase(const std::string &strUrl);
    static bool nativeCallHasPrefix(const std::string &strUrl, const std::string &strKey);
    
    static bool isEmailPatternMatching(const std::string &str);
    static bool isPhoneNumberPatternMatching(const std::string &str);
    static bool isPasswordPatternMatching(const std::string &str);
    static bool checkValidateString(const std::string &str);
    
#pragma mark - Facebook
    
    static void nativeCallFacebookLogin();
    static void nativeCallFacebookLogOut();
    static bool nativeCallFacebookLoginCheck();
    static void nativeCallFacebookMyInfo();
    static void nativeCallFacebookonUserAlbums();
    static void nativeCallFacebookonUserImages(const std::string &strAlbumID);
    static void nativeCallFacebookShare(const std::string &strUrl);
    static void nativeCallFacebookSharePhotoData(cocos2d::Data *pData);
    static void nativeCallFacebookSharePhotoPath(const std::string &strPath);

#pragma mark - Google
    
    static void nativeCallAccountsGoogle(const std::string &strUrl);
 
#pragma mark - kakao
    
    static void nativeCallKakaoLogin();
    static void nativeCallKakaoLogout();
    
#pragma mark - OneSignal
    
    static void nativeCallOneSignalsetSubscription(bool enable);
    
#pragma mark - for Android
    
    static void nativeCallAndroidDestroy();
    static void nativeCallAndroidToast(const std::string &msg);
    static std::string getPhoneNumber();
    static std::string getJavaString(const char *methodName);
    static void nativeCallIntentURIstartActivity(const std::string &url);
    static bool nativeCallstartsWith(const std::string &strUrl, const std::string &strKey);
    static bool isVirtualButton();
    static void nativeCallGoogleSignIn();
    static void nativeCallGoogleSignOut();
    static void nativeCallIntroFinish(bool var);
};

#endif /* PixNativeUtils_h */
