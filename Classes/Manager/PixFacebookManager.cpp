//
//  PixFacebookManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 29..
//
//

#include "PixFacebookManager.h"
#include "../Scene/PixApp.h"
#include "../Util/PixNativeUtils.h"
#include "../Network/PixHttpClient.h"

static PixFacebookManager* _sharedObject = nullptr;

static void checkFaceBookStatus()
{
    CCLOG("##FB> permission list: ");
    for (auto& permission : PluginFacebook::getPermissionList())
    {
        CCLOG("##FB>> permission %s", permission.data());
    }
    CCLOG("##FB> access token: %s", PluginFacebook::getAccessToken().c_str());
    CCLOG("##FB> user id: %s", PluginFacebook::getUserID().c_str());
    CCLOG("##FB> FBSDK version: %s", PluginFacebook::getSDKVersion().c_str());
}

PixFacebookManager* PixFacebookManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixFacebookManager();
    }
    
    return _sharedObject;
}

PixFacebookManager::PixFacebookManager()
: pixHttpClient(nullptr)
, mListener(nullptr)
{
//     PluginFacebook::setListener(this);
    pixHttpClient = PixHttpClient::sharedObject();
    
    facebookAlbums.clear();
}

PixFacebookManager::~PixFacebookManager()
{
    pixHttpClient->cancelRequest(this);
    
    for(auto iter = facebookAlbums.begin(); iter != facebookAlbums.end(); ++iter)
    {
        CC_SAFE_DELETE(iter->second);
    }
    
    facebookAlbums.clear();
}

void PixFacebookManager::init()
{
    PluginFacebook::init();
}

void PixFacebookManager::onLogin()
{
    CCLOG("##FB %s", __FUNCTION__);    
    PixNativeUtils::nativeCallFacebookLogin();
}

void PixFacebookManager::onLogout()
{
    CCLOG("##FB %s", __FUNCTION__);
    PixNativeUtils::nativeCallFacebookLogOut();
}

bool PixFacebookManager::onLoginClick()
{
    return PixNativeUtils::nativeCallFacebookLoginCheck();
}

void PixFacebookManager::onCheckStatus()
{
    CCLOG("##FB %s", __FUNCTION__);
    checkFaceBookStatus();
}

void PixFacebookManager::onGetMyInfo()
{
    CCLOG("##FB %s", __FUNCTION__);
    PixNativeUtils::nativeCallFacebookMyInfo();
}

void PixFacebookManager::onGetMyFriends()
{
    CCLOG("##FB %s", __FUNCTION__);
    
    sdkbox::PluginFacebook::fetchFriends();
}

void PixFacebookManager::onCaptureScreen()
{
    CCLOG("##FB %s", __FUNCTION__);
    
    utils::captureScreen(CC_CALLBACK_2(PixFacebookManager::afterCaptureScreen, this), "screen.png");
}
void PixFacebookManager::onShareLink()
{
    CCLOG("##FB %s", __FUNCTION__);
    
    FBShareInfo info;
    info.type  = FB_LINK;
    info.link  = "http://www.cocos2d-x.org";
    info.title = "cocos2d-x";
    info.text  = "Best Game Engine";
    info.image = "http://cocos2d-x.org/images/logo.png";
    PluginFacebook::share(info);

}

void PixFacebookManager::onSharePhoto(const std::string &strCaptureFilename)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    if (!strCaptureFilename.empty() && FileUtils::getInstance()->isFileExist(strCaptureFilename))
    {
        CCLOG("##FB dialog photo: %s", strCaptureFilename.c_str());
        
        FBShareInfo info;
        info.type  = FB_PHOTO;
        info.title = "capture screen";
        info.image = strCaptureFilename;
        PluginFacebook::share(info);
    }
    else
    {
        CCLOG("##FB capture screen first");
    }
}

void PixFacebookManager::onDialogLink()
{
    CCLOG("##FB %s", __FUNCTION__);
    
    FBShareInfo info;
    info.type  = FB_LINK;
    info.link  = "http://www.cocos2d-x.org";
    info.title = "cocos2d-x";
    info.text  = "Best Game Engine";
    info.image = "http://cocos2d-x.org/images/logo.png";
    PluginFacebook::dialog(info);
}

void PixFacebookManager::onDialogPhoto(const std::string &strCaptureFilename)
{
    CCLOG("##FB %s", __FUNCTION__);
    
    if (!strCaptureFilename.empty() && FileUtils::getInstance()->isFileExist(strCaptureFilename))
    {
        CCLOG("dialog photo: %s", strCaptureFilename.c_str());
        FBShareInfo info;
        info.type  = FB_PHOTO;
        info.title = "capture screen";
        info.image = strCaptureFilename;
        PluginFacebook::dialog(info);
    }
    else
    {
        CCLOG("##FB capture screen first");
    }
}

void PixFacebookManager::onRequestReadPermission()
{
    CCLOG("##FB %s", __FUNCTION__);
    
    PluginFacebook::requestReadPermissions({FB_PERM_READ_USER_FRIENDS});
}

void PixFacebookManager::onRequestPublishPermission()
{
    CCLOG("##FB %s", __FUNCTION__);
    
    PluginFacebook::requestPublishPermissions({FB_PERM_PUBLISH_POST});
}

void PixFacebookManager::onInviteFriends()
{
    PluginFacebook::inviteFriends("https://fb.me/402104549959868",
                                  "http://www.cocos2d-x.org/attachments/802/cocos2dx_landscape.png");
}

void PixFacebookManager::onUserAlbums()
{
    if(onLoginClick())
    {
        PixNativeUtils::nativeCallFacebookonUserAlbums();
    }
}

void PixFacebookManager::onUserImages()
{
    if(onLoginClick() && !facebookAlbums.empty())
    {
        for(auto iter = facebookAlbums.begin(); iter != facebookAlbums.end(); ++iter)
        {
            PixNativeUtils::nativeCallFacebookonUserImages(iter->first);
        }
    }
}

#pragma mark - Facebook callback

void PixFacebookManager::onLogin(bool isLogin, const std::string& msg)
{
    CCLOG("##FB isLogin: %d, error: %s", isLogin, msg.c_str());
    
    if(mListener)
    {
        mListener->onLogin(isLogin, msg);
    }
}

void PixFacebookManager::onSharedSuccess(const std::string& message)
{
    CCLOG("##FB onSharedSuccess:%s", message.c_str());
    
//    MessageBox(message.c_str(), "share success");
}

void PixFacebookManager::onSharedFailed(const std::string& message)
{
    CCLOG("##FB onSharedFailed:%s", message.c_str());
    
//    MessageBox(message.c_str(), "share failed");
}

void PixFacebookManager::onSharedCancel()
{
    CCLOG("##FB onSharedCancel");
    
//    MessageBox("", "share cancel");
}

void PixFacebookManager::onAPI(const std::string& tag, const std::string& jsonData)
{
    CCLOG("##FB onAPI: tag -> %s, json -> %s", tag.c_str(), jsonData.c_str());
    if (tag == "__fetch_picture_tag__")
    {
//        picojson::value v;
//        picojson::parse(v, jsonData);
//        std::string url = v.get("data").get("url").to_str();
//        CCLOG("picture's url = %s", url.data());
//        
//        _iconSprite->updateWithUrl(url);
    }
}

void PixFacebookManager::onPermission(bool isLogin, const std::string& msg)
{
    CCLOG("##FB onPermission: %d, error: %s", isLogin, msg.c_str());
    
//    std::string title = "permission ";
//    title.append((isLogin ? "success" : "failed"));
//    MessageBox(msg.c_str(), title.c_str());
}

void PixFacebookManager::onFetchFriends(bool ok, const std::string& msg)
{
    CCLOG("##FB %s: %d = %s", __FUNCTION__, ok, msg.data());
    
//    MenuItemFont::setFontSize(20);
//    static Menu *menu = Menu::create();
//    menu->setPositionY(20);
//    menu->cleanup();
    
    const std::vector<sdkbox::FBGraphUser>& friends = PluginFacebook::getFriends();
    for (int i = 0; i < friends.size(); i++)
    {
        const sdkbox::FBGraphUser& user = friends.at(i);
        CCLOG("##FB> -------------------------------");
        CCLOG("##FB>> %s", user.uid.data());
        CCLOG("##FB>> %s", user.firstName.data());
        CCLOG("##FB>> %s", user.lastName.data());
        CCLOG("##FB>> %s", user.name.data());
        CCLOG("##FB>> %s", user.isInstalled ? "app is installed" : "app is not installed");
        CCLOG("##FB");
        
//        MenuItemFont *item = MenuItemFont::create(user.name, [=](Ref*) {
//            sdkbox::FBAPIParam params;
//            params["redirect"] = "false";
//            params["type"] = "small";
//            std::string url(user.uid + "/picture");
//            PluginFacebook::api(url, "GET", params, "__fetch_picture_tag__");
//        });
//        menu->addChild(item);
    }
    
//    if (!menu->getParent()) {
//        menu->alignItemsHorizontally();
//        addChild(menu);
//    }
//    
//    MessageBox("", "fetch friends");
}

void PixFacebookManager::onRequestInvitableFriends( const std::map<std::string, std::string>& friends )
{
   
}

void PixFacebookManager::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )
{
     CCLOG("on invite friends with invite ids %s= '%s'", result?"ok":"error", msg.c_str());
}

void PixFacebookManager::onInviteFriendsResult( bool result, const std::string& msg )
{
    CCLOG("on invite friends %s= '%s'", result?"ok":"error", msg.c_str());
}

void PixFacebookManager::onGetUserInfo( const std::map<std::string, std::string>& userInfo )
{
    CCLOG("##FB %s", __FUNCTION__);
   
    if(mListener)
    {
        mListener->onGetUserInfo(userInfo);
    }
}

void PixFacebookManager::onGetUserAlbums( bool result, const std::string& msg )
{
    CCLOG("##FB %s", __FUNCTION__);
    
    if(result)
    {
        PixDictionary *root = pixHttpClient->getDictionaryFromElement(msg);
        
        if(root)
        {
            PixArray *data = root->arrayForKey("data");
            
            if(data)
            {
                for (int i = 0; i < data->count(); ++i)
                {
                    PixDictionary *album = data->getDictionaryAtIndex(i);
                    
                    if(album)
                    {
                        facebookAlbumInfo *info = new facebookAlbumInfo();
                        
                        info->setName(album->stringForKey("name"));
                        info->setID(album->stringForKey("id"));
                        info->setCount(album->intForKey("count"));
                        
                        auto ret = facebookAlbums.insert(std::make_pair(info->getID(), info));
                        
                        if(ret.second == false)
                        {
                            CC_SAFE_DELETE(ret.first->second);
                            facebookAlbums[info->getID()] = info;
                        }
                    }
                }
            }
            
            onUserImages();
        }
    }
}

void PixFacebookManager::onGetUserImages( bool result, const std::string& albumID, const std::string& msg )
{
    CCLOG("##FB %s", __FUNCTION__);
    
    if(result)
    {
        PixDictionary *root = pixHttpClient->getDictionaryFromElement(msg);
        
        if(root)
        {
            facebookAlbumInfo *albumInfo = nullptr;
            
            auto it = facebookAlbums.find(albumID);
            
            if (it != facebookAlbums.end())
            {
                albumInfo = it->second;
            }
            
            if(albumInfo)
            {
                PixArray *data = root->arrayForKey("data");
                
                if(data)
                {
                    for (int i = 0; i < data->count(); ++i)
                    {
                        PixDictionary *image = data->getDictionaryAtIndex(i);
                        
                        if(image)
                        {
                            facebookImageInfo *info = new facebookImageInfo();
                            
                            info->setID(image->stringForKey("id"));
                            info->setName(image->stringForKey("name"));
                            info->setUpdated_time(image->stringForKey("updated_time"));
                            info->setSource(image->stringForKey("source"));
                            info->setPicture(image->stringForKey("picture"));
                            info->setWidth(image->intForKey("width"));
                            info->setHeight(image->intForKey("height"));
                               
                            albumInfo->pushFacebookImageInfo(info);
                        }
                    }
                }
            }
        }
    }
}

void PixFacebookManager::afterCaptureScreen(bool yes, const std::string& outputFilename)
{
    CCLOG("##FB afterCaptureScreen: %s", outputFilename.c_str());
    
    if (yes)
    {
        
    }
}
