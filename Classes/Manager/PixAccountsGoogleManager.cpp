//
//  PixAccountsGoogleManager.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 2. 14..
//
//

#include "PixAccountsGoogleManager.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixUserDefault.h"
#include "../Util/PixNativeUtils.h"
#include "../Layer/PixAndroidWebViewLaver.h"
#include "../Network/PixHttpClient.h"
#include "../Info/googlePhotoAlbumInfo.h"
#include "../Info/googlePhotoInfo.h"
#include "../Scene/PixApp.h"
#include "../Common.h"

static std::string OAUTH_URL = "https://accounts.google.com/o/oauth2/auth";
static std::string GOOGLE_PHOTO_SCOPE = "openid+email+profile+https://picasaweb.google.com/data/";
static std::string TOKEN_URL = "https://accounts.google.com/o/oauth2/token";
static std::string OAUTH_USERINFO = "https://www.googleapis.com/oauth2/v3/userinfo";
static std::string PICASAWEB_URL = "https://picasaweb.google.com";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
static std::string CLIENT_ID = "241674176230-l1c2klvqrhchb2sok8i63ba5da6jges0";
static std::string REDIRECT_URL = std::string("com.googleusercontent.apps.").append(CLIENT_ID).append(":/oauthredirect");
#else
static std::string CLIENT_ID = "241674176230-u0p0ip88mq9mum8rkmnisk842jkhqs3a";
static std::string REDIRECT_URL = "http://localhost";
#endif

static PixAccountsGoogleManager* _sharedObject = nullptr;

PixAccountsGoogleManager* PixAccountsGoogleManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixAccountsGoogleManager();
        _sharedObject->init();
    }
    
    return _sharedObject;
}

PixAccountsGoogleManager::PixAccountsGoogleManager()
: _userInfoCallBack(nullptr)
, pixHttpClient(nullptr)
, token(nullptr)
, userinfo(nullptr)
, androidWebViewLaver(nullptr)
{
    pixHttpClient = PixHttpClient::sharedObject();
    
    googlePhotoAlbums.clear();
}

PixAccountsGoogleManager::~PixAccountsGoogleManager()
{
    pixHttpClient->cancelRequest(this);
    
    CC_SAFE_RELEASE_NULL(token);
    CC_SAFE_RELEASE_NULL(userinfo);
    
    for (auto iter = googlePhotoAlbums.begin(); iter != googlePhotoAlbums.end(); ++iter)
    {
        CC_SAFE_DELETE(iter->second);
    }
    
    googlePhotoAlbums.clear();
}

void PixAccountsGoogleManager::accountsGoogle(const std::function<void()> &callback)
{
    this->_userInfoCallBack = callback;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    std::string url = std::string(OAUTH_URL).append("?");
    url.append("response_type=code");
    url.append("&redirect_uri=").append(REDIRECT_URL);
    url.append("&client_id=").append(CLIENT_ID).append(".apps.googleusercontent.com");
    url.append("&scope=").append(GOOGLE_PHOTO_SCOPE);
    url.append("&approval_prompt=force");
    url.append("&access_type=offline");
    
    PixNativeUtils::nativeCallAccountsGoogle(url);
    
#else
    PixNativeUtils::nativeCallGoogleSignIn();
//    androidAccountsGoogle(url);
#endif
}

bool PixAccountsGoogleManager::resumeAuthorizationFlowWithURL(const std::string &url)
{
    if(!url.empty())
    {
        auto error = url.find("error=");
        
        if ((error != std::string::npos) && (0 < error))
        { 
            return true;
        }
        
        std::string code;
        std::string key = "code=";
     
        auto pos = url.find(key);
        
        if ((pos != std::string::npos) && (0 < pos))
        {
            code = url.substr(pos + key.size(), url.size());
            
            auto end = code.find("&");
            
            if((end != std::string::npos) && (0 < end))
            {
                code = code.substr(0, end);
            }
        }

        if(!code.empty())
        {
            ACCOUNTS_GOOGLE_MANAGER->getToken(code);
            return true;
        }
    }
    
    return false;
}

void PixAccountsGoogleManager::clearAuthState()
{
    CC_SAFE_RELEASE_NULL(token);
    PixUserDefault::SetAccountsGoogleToken("");
    
    CC_SAFE_RELEASE_NULL(userinfo);
    PixUserDefault::SetAccountsGoogleUserInfo("");
    
    for (auto iter = googlePhotoAlbums.begin(); iter != googlePhotoAlbums.end(); ++iter)
    {
        CC_SAFE_DELETE(iter->second);
    }
    
    googlePhotoAlbums.clear();
    
//    PixNativeUtils::nativeCallGoogleSignOut();
}

bool PixAccountsGoogleManager::isAuthorize()
{
    if(token)
    {
        return true;
    }
    
    return false;
}

std::string PixAccountsGoogleManager::getAccess_token()
{
    if(token)
    {
        return token->stringForKey("access_token");
    }
    
    return "";
}

std::string PixAccountsGoogleManager::getId_token()
{
    if(token)
    {
        return token->stringForKey("id_token");
    }
    
    return "";
}

int PixAccountsGoogleManager::getExpires_in()
{
    if(token)
    {
        return token->intForKey("expires_in");
    }
    
    return 0;
}

std::string PixAccountsGoogleManager::getRefresh_token()
{
    if(token)
    {
        return token->stringForKey("refresh_token");
    }
    
    return "";
}

std::string PixAccountsGoogleManager::getToken_type()
{
    if(token)
    {
        return token->stringForKey("token_type");
    }
    
    return "";
}

std::string PixAccountsGoogleManager::getUserID()
{
    if(userinfo)
    {
        return userinfo->stringForKey("sub");
    }
    
    return "";
}

std::string PixAccountsGoogleManager::getUserName()
{
    if(userinfo)
    {
        return userinfo->stringForKey("name");
    }
    
    return "";
}

std::string PixAccountsGoogleManager::getUserEmail()
{
    if(userinfo)
    {
        return userinfo->stringForKey("email");
    }
    
    return "";
}

std::string PixAccountsGoogleManager::getUserProfile()
{
    if(userinfo)
    {
        return userinfo->stringForKey("profile");
    }
    
    return "";
}

void PixAccountsGoogleManager::getToken(const std::string &strCode)
{
    std::map<std::string, std::string> postParams;
    
    postParams["client_id"] = std::string(CLIENT_ID).append(".apps.googleusercontent.com");
    postParams["redirect_uri"] = REDIRECT_URL;
    postParams["code"] = strCode;
    postParams["grant_type"] = "authorization_code";
    
    pixHttpClient->getFormData(TOKEN_URL, postParams, "token", this, httpresponse_selector(PixAccountsGoogleManager::getTokenCallback), httpresponse_selector(PixAccountsGoogleManager::accountsGoogleRequestError));
}

void PixAccountsGoogleManager::getPicasawebAlbum(const std::function<void()> &callback)
{
    this->_picasawebPhotoCallBack = callback;
    
    std::string url = PICASAWEB_URL;
    url.append("/data/feed/api/user/");
    url.append(getUserID());
    url.append("?access_token=");
    url.append(getAccess_token());
    url.append("&alt=json");
    
    pixHttpClient->sendGetRequest(url, "PicasawebAlbum", this, httpresponse_selector(PixAccountsGoogleManager::getPicasawebAlbumCallback), httpresponse_selector(PixAccountsGoogleManager::picasawebRequestError));
}

void PixAccountsGoogleManager::removeAndroidWebViewLaver()
{
    if(androidWebViewLaver)
    {
        androidWebViewLaver->removeFromParentAndCleanup(true);
        androidWebViewLaver = nullptr;
    }
}

void PixAccountsGoogleManager::androidGoogleSignInSuccess(const std::string &name,const std::string &getUserID, const std::string &email, const std::string &access_token)
{    
    std::string strResponseData = StringUtils::format("{\"access_token\":\"%s\"}",access_token.c_str());
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(strResponseData);
    
    if(root)
    {
        PixUserDefault::SetAccountsGoogleToken("");
        PixUserDefault::SetAccountsGoogleToken(strResponseData);
        
        CC_SAFE_RELEASE_NULL(token);
        token = root;
        token->retain();
    }
    
    strResponseData = StringUtils::format("{\"email\":\"%s\",\"name\":\"%s\",\"sub\":\"%s\",\"profile\":\"https://plus.google.com/%s\"}",email.c_str(), name.c_str(), getUserID.c_str(), getUserID.c_str());
    
    root = pixHttpClient->getDictionaryFromElement(strResponseData);
    
    if(root)
    {
        PixUserDefault::SetAccountsGoogleUserInfo("");
        PixUserDefault::SetAccountsGoogleUserInfo(strResponseData);
        
        CC_SAFE_RELEASE_NULL(userinfo);
        userinfo = root;
        userinfo->retain();
    }
    
    if(_userInfoCallBack)
    {
        (_userInfoCallBack)();
        _userInfoCallBack = nullptr;
    }
}

#pragma mark - private

void PixAccountsGoogleManager::init()
{
    std::string strResponseData = PixUserDefault::GetAccountsGoogleToken();
    
    PixDictionary* root = pixHttpClient->getDictionaryFromElement(strResponseData);
    
    if(root)
    {
        CC_SAFE_RELEASE_NULL(token);
        token = root;
        token->retain();
    }
    
    strResponseData = PixUserDefault::GetAccountsGoogleUserInfo();
    
    root = pixHttpClient->getDictionaryFromElement(strResponseData);
    
    if(root)
    {
        CC_SAFE_RELEASE_NULL(userinfo);
        userinfo = root;
        userinfo->retain();
    }
}

void PixAccountsGoogleManager::androidAccountsGoogle(const std::string &url)
{
    auto webView = cocos2d::experimental::ui::WebView::create();
    
    webView->setOnDidFailLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
        
    });
    
    webView->setOnJSCallback([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {

    });
    
    webView->setOnDidFinishLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
        
    });
    
    webView->setOnShouldStartLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url)->bool {
        
        if(ACCOUNTS_GOOGLE_MANAGER->resumeAuthorizationFlowWithURL(url))
        {
            if(sender)
            {
                ACCOUNTS_GOOGLE_MANAGER->removeAndroidWebViewLaver();
            }
        }
        
        return true;
    });
    
    webView->setScalesPageToFit(true);
    webView->loadURL(url);
    
    androidWebViewLaver = PixAndroidWebViewLaver::create(webView, true);
    PIXAPP->isScene->addChild(androidWebViewLaver);
}

void PixAccountsGoogleManager::getTokenCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::vector<char>* data = response->getResponseData();
    std::string strResponseData(data->begin(), data->end());
    
    PixDictionary *root = pixHttpClient->getDictionaryFromElement(strResponseData);
    
    if(root)
    {
        CC_SAFE_RELEASE_NULL(token);
        PixUserDefault::SetAccountsGoogleToken("");
        
        PixUserDefault::SetAccountsGoogleToken(strResponseData);
        token = root;
        token->retain();
        
        getUserInfo();
    }
}

void PixAccountsGoogleManager::getUserInfo()
{
    PIXAPP->showLoading();
    
    std::vector<std::string> headers;
    std::string head = std::string("Authorization:").append(getToken_type()).append(" ")
    .append(getAccess_token());
    headers.push_back(head);
    
    pixHttpClient->sendGetRequest(OAUTH_USERINFO, headers, "userinfo", this, httpresponse_selector(PixAccountsGoogleManager::getUserInfoCallback), httpresponse_selector(PixAccountsGoogleManager::accountsGoogleRequestError));
}

void PixAccountsGoogleManager::getUserInfoCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PIXAPP->hideLoading();
    
    std::vector<char>* data = response->getResponseData();
    std::string strResponseData(data->begin(), data->end());
    
    PixDictionary *root = pixHttpClient->getDictionaryFromElement(strResponseData);
    
    if(root)
    {
        CC_SAFE_RELEASE_NULL(userinfo);
        PixUserDefault::SetAccountsGoogleUserInfo("");
        
        PixUserDefault::SetAccountsGoogleUserInfo(strResponseData);
        userinfo = root;
        userinfo->retain();
      
        if(_userInfoCallBack)
        {
            (_userInfoCallBack)();
            _userInfoCallBack = nullptr;
        }
    }
}

void PixAccountsGoogleManager::getPicasawebAlbumCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary *root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        PixDictionary *feed = root->dictionaryForKey("feed");
        
        if(feed)
        {
            PixArray *entry = feed->arrayForKey("entry");
            
            if(entry)
            {
                for (int i = 0; i < entry->count(); ++i)
                {
                    PixDictionary *album = entry->getDictionaryAtIndex(i);
                    
                    if(album)
                    {
                        googlePhotoAlbumInfo *info = new googlePhotoAlbumInfo();
                        
                        PixDictionary *title = album->dictionaryForKey("title");
                        
                        if(title)
                        {
                            info->setTitle(title->stringForKey("$t"));
                        }
                        
                        PixDictionary *id = album->dictionaryForKey("gphoto$id");
                        
                        if(id)
                        {
                            info->setAlbumID(id->stringForKey("$t"));
                        }
                        
                        PixDictionary *numphotos = album->dictionaryForKey("gphoto$numphotos");
                        
                        if(numphotos)
                        {
                            info->setNumphotos(numphotos->intForKey("$t"));
                        }
                                                
                        auto ret = googlePhotoAlbums.insert(std::make_pair(info->getAlbumID(), info));
                        
                        if (ret.second == false)
                        {
                            CC_SAFE_DELETE(ret.first->second);
                            googlePhotoAlbums[info->getAlbumID()] = info;
                        }
                    }
                }
            }
        }
        
        getPicasawebPhoto();
    }
}

void PixAccountsGoogleManager::getPicasawebPhoto(int nStartIndex)
{
    for (auto iter = googlePhotoAlbums.begin(); iter != googlePhotoAlbums.end(); ++iter)
    {
        std::string url = PICASAWEB_URL;
        url.append("/data/feed/api/user/");
        url.append(getUserID());
        url.append("/albumid/");
        url.append((*iter->second).getAlbumID());
        url.append("?access_token=");
        url.append(getAccess_token());
        
        if(nStartIndex > 0)
        {
            url.append("&start-index=").append(StringUtils::format("%d",nStartIndex));
            url.append("&max-results=1000");
        }

        url.append("&alt=json");
        
        pixHttpClient->sendGetRequest(url, "PicasawebPhoto", this, httpresponse_selector(PixAccountsGoogleManager::getPicasawebPhotoCallback), httpresponse_selector(PixAccountsGoogleManager::picasawebRequestError));
    }
}

void PixAccountsGoogleManager::getPicasawebPhotoCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    PixDictionary *root = pixHttpClient->getDictionaryFromElement(response);
    
    if(root)
    {
        if(root)
        {
            PixDictionary *feed = root->dictionaryForKey("feed");
            
            if(feed)
            {
                int nOpenSearchstartIndex = 0;
                
                PixDictionary *openSearchstartIndex = feed->dictionaryForKey("openSearch$startIndex");
                
                if(openSearchstartIndex)
                {
                    nOpenSearchstartIndex = openSearchstartIndex->intForKey("$t");
                }
                
                PixDictionary *id = feed->dictionaryForKey("gphoto$id");
                
                if(id)
                {
                    googlePhotoAlbumInfo *albumInfo = nullptr;
                    
                    auto it = googlePhotoAlbums.find(id->stringForKey("$t"));
                    
                    if (it != googlePhotoAlbums.end())
                    {
                        albumInfo = it->second;
                    }
                    
                    if (albumInfo)
                    {
                        std::string strUpdated = feed->stringForKey("updated");
                        std::string strPublished = feed->stringForKey("published");
                        
                        PixArray *entry = feed->arrayForKey("entry");
                        
                        if(entry)
                        {
                            albumInfo->setIncreasePhotos((int)entry->count());
                            
                            for (int i = 0; i < entry->count(); ++i)
                            {
                                PixDictionary *photo = entry->getDictionaryAtIndex(i);
                                
                                if(photo)
                                {
                                    googlePhotoInfo *info = new googlePhotoInfo();

                                    info->setPublished(strPublished);
                                    info->setUpdateed(strUpdated);
                                    
                                    PixDictionary *id = photo->dictionaryForKey("gphoto$id");
                                    
                                    if(id)
                                    {
                                        info->setPhotoID(id->stringForKey("$t"));
                                    }
                                    
                                    PixDictionary *width = photo->dictionaryForKey("gphoto$width");
                                    
                                    if(width)
                                    {
                                        info->setWidth(width->intForKey("$t"));
                                    }
                                    
                                    PixDictionary *height = photo->dictionaryForKey("gphoto$height");
                                    
                                    if(height)
                                    {
                                        info->setHight(height->intForKey("$t"));
                                    }
                                    
                                    PixDictionary *size = photo->dictionaryForKey("gphoto$size");
                                    
                                    if(size)
                                    {
                                        info->setSize(size->intForKey("$t"));
                                    }
                                    
                                    PixDictionary *content = photo->dictionaryForKey("content");
                                    
                                    if(content)
                                    {
                                        info->setSrc(content->stringForKey("src"));
                                        
                                        std::string type = content->stringForKey("type");
                                        
                                        if(type.compare("image/jpeg") == 0)
                                        {
                                            
                                        }
                                        else if(type.compare("image/png") == 0)
                                        {
                                            
                                        }
                                        else
                                        {
                                            CC_SAFE_DELETE(info);
                                            continue;
                                        }
                                    }
                                    
                                    PixDictionary *mediaGroup = photo->dictionaryForKey("media$group");
                                    
                                    if(mediaGroup)
                                    {
                                        PixArray *mediaThumbnail = mediaGroup->arrayForKey("media$thumbnail");
                                        
                                        if(mediaThumbnail)
                                        {
                                            PixDictionary *thumbnail = mediaThumbnail->getDictionaryAtIndex((int)(mediaThumbnail->count() - 1));
                                            
                                            if(thumbnail)
                                            {
                                                info->setThumbnail(thumbnail->stringForKey("url"));
                                            }
                                        }
                                    }
                                    
                                    albumInfo->pushGooglePhotoInfo(info);
                                }
                            }
                            
                            if(albumInfo->isEnd() == false)
                            {
                                if(albumInfo->getIncreasePhotos() > 0)
                                {
                                    getPicasawebPhoto(albumInfo->getIncreasePhotos() + 1);
                                    return;
                                }
                            }
                            
                            bool isEnd = true;
                            
                            for (auto iter = googlePhotoAlbums.begin(); iter != googlePhotoAlbums.end(); ++iter)
                            {
                                if((*iter->second).isEnd() == false)
                                {
                                    isEnd = false;
                                    break;
                                }
                            }
                            
                            if(isEnd)
                            {
                                if(_picasawebPhotoCallBack)
                                {
                                    (_picasawebPhotoCallBack)();
                                    _picasawebPhotoCallBack = nullptr;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void PixAccountsGoogleManager::accountsGoogleRequestError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    
}
void PixAccountsGoogleManager::picasawebRequestError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    
}
