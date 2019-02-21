//
//  PixFacebookManager.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 29..
//
//

#ifndef PixFacebookManager_h
#define PixFacebookManager_h

#include "cocos2d.h"
#include "../Model/MyAlbumModel.h"
#include "../Util/PixUserDefault.h"
#include "PluginFacebook/PluginFacebook.h"
#include "../Info/facebookAlbumInfo.h"
#include "../Info/facebookImageInfo.h"

#define FACEBOOK_MANAGER     (PixFacebookManager::sharedObject())

USING_NS_CC;
using namespace sdkbox;

class PixHttpClient;
class PixFacebookListener;

class CC_DLL PixFacebookManager : public Ref
{
    
public :
    
    static PixFacebookManager* sharedObject();
    
    PixFacebookManager();
    virtual ~PixFacebookManager();
    
    void init();
    void onLogin();
    void onLogout();
    bool onLoginClick();
    void onCheckStatus();
    void onGetMyInfo();
    void onGetMyFriends();
    void onCaptureScreen();
    void onShareLink();
    void onSharePhoto(const std::string &strCaptureFilename);
    void onDialogLink();
    void onDialogPhoto(const std::string &strCaptureFilename);
    void onRequestReadPermission();
    void onRequestPublishPermission();
    void onInviteFriends();
    void onUserAlbums();
    void onUserImages();
    
#pragma mark - Facebook callback
    
    void onLogin(bool isLogin, const std::string& msg);
    void onSharedSuccess(const std::string& message);
    void onSharedFailed(const std::string& message);
    void onSharedCancel();
    void onAPI(const std::string& key, const std::string& jsonData);
    void onPermission(bool isLogin, const std::string& msg);
    void onFetchFriends(bool ok, const std::string& msg);
    void onRequestInvitableFriends( const std::map<std::string, std::string>& friends );
    void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg );
    void onInviteFriendsResult( bool result, const std::string& msg );
    void onGetUserInfo( const std::map<std::string, std::string>& userInfo );
    void onGetUserAlbums( bool result, const std::string& msg );
    void onGetUserImages( bool result, const std::string& albumID, const std::string& msg );
    void afterCaptureScreen(bool yes, const std::string& outputFilename);
    
private:
    
    PixHttpClient *pixHttpClient;
    std::unordered_map<std::string, facebookAlbumInfo*> facebookAlbums;
    
    CC_SYNTHESIZE(PixFacebookListener*, mListener, Listener)
    
};

class PixFacebookListener
{
public:
    virtual void onLogin(bool isLogin, const std::string& msg) = 0;
    virtual void onSharedSuccess(const std::string& message) = 0;
    virtual void onSharedFailed(const std::string& message) = 0;
    virtual void onSharedCancel() = 0;
    virtual void onAPI(const std::string& key, const std::string& jsonData) = 0;
    virtual void onPermission(bool isLogin, const std::string& msg) = 0;
    virtual void onFetchFriends(bool ok, const std::string& msg) = 0;
    virtual void onRequestInvitableFriends( const std::map<std::string, std::string>& friends ) = 0;
    virtual void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )= 0;
    virtual void onInviteFriendsResult( bool result, const std::string& msg )= 0;
    virtual void onGetUserInfo( const std::map<std::string, std::string>& userInfo ) = 0;
};


#endif /* PixFacebookManager_h */

