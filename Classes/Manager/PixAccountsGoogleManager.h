//
//  PixAccountsGoogleManager.h
//  Pzle
//
//  Created by 심규빈 on 2017. 2. 14..
//
//

#ifndef PixAccountsGoogleManager_h
#define PixAccountsGoogleManager_h

#include "cocos2d.h"
#include "network/HttpClient.h"

#define ACCOUNTS_GOOGLE_MANAGER         (PixAccountsGoogleManager::sharedObject())

USING_NS_CC;

class PixHttpClient;
class PixDictionary;
class googlePhotoAlbumInfo;
class PixAndroidWebViewLaver;

class PixAccountsGoogleManager : public Ref
{
    
public :
    
    static PixAccountsGoogleManager* sharedObject();
    
    PixAccountsGoogleManager();
    virtual ~PixAccountsGoogleManager();
    
    void accountsGoogle(const std::function<void()> &callback);
    bool resumeAuthorizationFlowWithURL(const std::string &url);
    
    void clearAuthState();
    bool isAuthorize();
    std::string getAccess_token();
    std::string getId_token();
    int getExpires_in();
    std::string getRefresh_token();
    std::string getToken_type();
    std::string getUserID();
    std::string getUserName();
    std::string getUserEmail();
    std::string getUserProfile();
    
    void getToken(const std::string &strCode);
    void getPicasawebAlbum(const std::function<void()> &callback);
    void removeAndroidWebViewLaver();
    void androidGoogleSignInSuccess(const std::string &name,const std::string &getUserID, const std::string &email, const std::string &access_token);
    
private:
    
    std::function<void()> _userInfoCallBack;
    std::function<void()> _picasawebPhotoCallBack;
    
    PixHttpClient *pixHttpClient;
    PixDictionary *token;
    PixDictionary *userinfo;
    PixAndroidWebViewLaver *androidWebViewLaver;
    
    std::unordered_map<std::string, googlePhotoAlbumInfo*> googlePhotoAlbums;
    
    void init();
    void androidAccountsGoogle(const std::string &url);
    void getTokenCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void getUserInfo();
    void getUserInfoCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getPicasawebAlbumCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getPicasawebPhoto(int nStartIndex = 0);
    void getPicasawebPhotoCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void accountsGoogleRequestError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void picasawebRequestError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};

#endif /* PixAccountsGoogleManager_h */
