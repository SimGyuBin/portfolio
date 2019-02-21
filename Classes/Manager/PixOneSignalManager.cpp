//
//  PixOneSignalManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 19..
//
//

#include "PixOneSignalManager.h"
#include "PluginOneSignal/PluginOneSignal.h"
#include "../Util/PixUserDefault.h"
#include "../Util/PixNativeUtils.h"
#include "../UIControl/PixModalLayer.h"
#include "PixServerUrlManager.h"
#include "../Network/PixHttpClient.h"
#include "../Layer/PixMenuEventDetailLayer.h"
#include "../Layer/PixMenuNoticeiDetailLayer.h"
#include "../Layer/PixMenuEventLayer.h"
#include "../Layer/PixMenuNoticeLayer.h"
#include "../Layer/PixMenuCouponLayer.h"
#include "../Layer/PixMenuOrderLayer.h"
#include "../Layer/PixMenuCartLayer.h"
#include "../Layer/PixPushPopupLayer.h"
#include "../Layer/PixPushToast.h"
#include "../Util/PixNativeUtils.h"
#include "../Scene/PixApp.h"
#include "../Scene/PixMyAlbumLevel.h"
#include "../Common.h"

class CustomListener : public sdkbox::OneSignalListener {
public:
    
    void onNotification(bool isActive, const std::string& message, const std::string& additionalData) {
        CCLOG("%s isActive=%s, message=%s, additionalData=%s", __FUNCTION__, isActive ? "yes" : "no", message.c_str(), additionalData.c_str());
    }
    void onSendTag(bool success, const std::string& k, const std::string& message) {
        CCLOG("%s: success=%d, key=%s, message=%s", __FUNCTION__, success, k.c_str(), message.c_str());
    }
    void onGetTags(const std::string& jsonString) {
        CCLOG("%s json=%s", __FUNCTION__, jsonString.c_str());
    }
    void onIdsAvailable(const std::string& userId, const std::string& pushToken) {
        CCLOG("%s: userId='%s', pushToken='%s'", __FUNCTION__, userId.c_str(), pushToken.c_str());
    
//        PixOneSignalManager::sharedObject()->setUserID(userId);
//        PixOneSignalManager::sharedObject()->setPushToken(pushToken);
    }
    void onPostNotification(bool success, const std::string& message) {
        CCLOG("%s: success=%s, message=%s", __FUNCTION__, success ? "yes" : "no", message.c_str());
    }

};

static PixOneSignalManager* _sharedObject = nullptr;

PixOneSignalManager* PixOneSignalManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixOneSignalManager();
    }
    
    return _sharedObject;
}

PixOneSignalManager::PixOneSignalManager()
: mstr_UserID("")
, mstr_PushToken("")
, mbln_IntroFinish(false)
{
    additionalDataQueue.clear();
}

PixOneSignalManager::~PixOneSignalManager()
{
    for (auto iter = additionalDataQueue.begin(); iter != additionalDataQueue.end(); ++iter)
    {
        CC_SAFE_DELETE(*iter);
    }
    
    additionalDataQueue.clear();
}

void PixOneSignalManager::init(const std::string &strUserID, const std::string &strPushToken)
{
    mstr_UserID = strUserID;
    mstr_PushToken = strPushToken;
    
    setSubscription(PixUserDefault::GetAlarm());
    
    CCLOG("OneSignal %s: userId='%s', pushToken='%s'", __FUNCTION__, getUserID().c_str(), getPushToken().c_str());
    
    /*
    // plugin
    sdkbox::PluginOneSignal::setListener(new CustomListener());
    sdkbox::PluginOneSignal::init();
    
//    sdkbox::PluginOneSignal::setLogLevel(sdkbox::OneSignalLogVerbose, sdkbox::OneSignalLogVerbose);
    sdkbox::PluginOneSignal::setSubscription(PixUserDefault::GetAlarm());
//    sdkbox::PluginOneSignal::setEmail(ONESIGNAL_EMAIL);
//    sdkbox::PluginOneSignal::sendTag("key", "value");
//    sdkbox::PluginOneSignal::sendTag("key1", "value1");
//    sdkbox::PluginOneSignal::getTags();
//    sdkbox::PluginOneSignal::deleteTag("key");

    sdkbox::PluginOneSignal::idsAvailable();
    sdkbox::PluginOneSignal::enableInAppAlertNotification(true);
    sdkbox::PluginOneSignal::promptLocation();
     */
}

void PixOneSignalManager::setSubscription(bool enable)
{
    PixNativeUtils::nativeCallOneSignalsetSubscription(enable);
//    sdkbox::PluginOneSignal::setsetSubscription(enable);
}

void PixOneSignalManager::setOneSignalHandleNotification(const std::string &strMessage, const std::string &strTitle, const std::string &strPush_event, bool blnActive)
{
    CCLOG("PixOneSignalManager::setOneSignalHandleNotification strMessage : %s", strMessage.c_str());
    CCLOG("PixOneSignalManager::setOneSignalHandleNotification strTitle : %s", strTitle.c_str());
    CCLOG("PixOneSignalManager::setOneSignalHandleNotification strPush_event : %s", strPush_event.c_str());
    CCLOG("PixOneSignalManager::setOneSignalHandleNotification blnActive : %d", blnActive);
    
    PixOneSignalAdditionalData *additionalData = new PixOneSignalAdditionalData(strMessage, strTitle, strPush_event, blnActive);
    
    additionalDataQueue.push_back(additionalData);
}

void PixOneSignalManager::pushEventExecution()
{
    PixOneSignalAdditionalData *additionalData = nullptr;
    
    if(!additionalDataQueue.empty())
    {
        additionalData = additionalDataQueue.front();
        additionalDataQueue.pop_front();
        
        ONESIGNAL_MANAGER->pushEventExecution(additionalData->getMessage(), additionalData->getTitle(), additionalData->getPush_evevt(), additionalData->getActive());
        
        CC_SAFE_DELETE(additionalData);
    }
}

void PixOneSignalManager::pushEventExecution(const std::string &strMessage, const std::string &strTitle, const std::string &strPush_event, bool blnActive)
{
    CCLOG("PixOneSignalManager::pushEventExecution strMessage : %s", strMessage.c_str());
    CCLOG("PixOneSignalManager::pushEventExecution strTitle : %s", strTitle.c_str());
    CCLOG("PixOneSignalManager::pushEventExecution strPush_event : %s", strPush_event.c_str());
    CCLOG("PixOneSignalManager::pushEventExecution blnActive : %d", blnActive);
   
    /*
     
     {"CART":""}	장바구니
     {"ORDER":""}	주문배송
     {"COUPON_LIST":""}	쿠폰리스트
     {"SET_COUPON":""}	쿠폰등록화면
     {"EVENT":""}	이벤트메인화면
     {"EVENT_DETAIL":"1"} (1=이벤트 ID)	이벤트 상세화면
     {"NOTICE":""}	공지사항
     {"NOTICE_DETAIL":"1"} (1=공지사항 ID)	공지사항 상세화면
     {"ALBUM":""}	앨범리스트(메인화면)
     {"NATIVE_BROWSER":"url정보"}	URL띄우기
     만약 앨범 제작중에는 타이틀만 토스트로 띄우게 처리
     
     */
    
    if(PIXAPP->getBackground())
    {
        setOneSignalHandleNotification(strMessage, strTitle, strPush_event, blnActive);
    }
    else
    {
        PIX_PAGE_TYPE eType = PIXAPP->getPageType();
        
        if(eType == PT_MYALBUM || eType == PT_MYALBUM_THEME)
        {
            if(blnActive)
            {
                if(strPush_event.empty())
                {
                    PixPushToast::showToast(strTitle);
                }
                else
                {
                    Scene *scene = PIXAPP->isScene;
                    
                    if(scene)
                    {
                        Node *beforePopup = dynamic_cast<Node*>(scene->getChildByTag(PUSHPOPUP_TAG));
                        
                        if (beforePopup)
                        {
                            beforePopup->removeFromParentAndCleanup(true);
                        }
                        
                        scene->addChild(PixPushPopupLayer::create(strMessage, strTitle, strPush_event), 100, PUSHPOPUP_TAG);
                    }
                }
            }
            else
            {
                if(strPush_event.empty())
                {
                    PixPushToast::showToast(strTitle);
                }
                else
                {
                    moveEventPage(strPush_event);
                }
            }
        }
        else
        {
            PixPushToast::showToast(strTitle);
        }
    }
}

void PixOneSignalManager::moveEventPage(const std::string &strPush_event)
{
    if(!strPush_event.empty())
    {
        PixDictionary* root = PixHttpClient::sharedObject()->getDictionaryFromElement(strPush_event);
        
        if(root)
        {
            Scene *scene = PIXAPP->isScene;
            
            if(scene)
            {
                if(!scene->getChildren().empty())
                {
                    int nIndex = (int)scene->getChildren().size() - 1;
                    
                    while (true)
                    {
                        GbLevel *level = dynamic_cast<GbLevel*>(scene->getChildren().at(nIndex));
                        
                        if(level)
                        {
                            PixMyAlbumLevel *myAlbumLevel = dynamic_cast<PixMyAlbumLevel*>(level);
                            
                            if(myAlbumLevel)
                            {
                                if(myAlbumLevel->isSlideLeftMenuShow())
                                {
                                    myAlbumLevel->hideSlideLeftMenu();
                                }
                            }
                        }
                        
                        PixModalLayer *layer = dynamic_cast<PixModalLayer*>(scene->getChildren().at(nIndex));
                        
                        if(layer)
                        {
                            layer->hideLayer();
                        }
                        
                        --nIndex;
                        
                        if(nIndex < 0)
                        {
                            break;
                        }
                    }
                }
            }
            
            PixArray *keys = root->allKeys();
            
            if(keys)
            {
                std::string strKey = keys->getStringAtIndex(0);
                
                if(strKey.compare("CART") == 0)
                {
                    PixMenuCartLayer *menuCartLayer = PixMenuCartLayer::create();
                    scene->addChild(menuCartLayer);
                }
                else if(strKey.compare("ORDER") == 0)
                {
                    PixMenuOrderLayer *menuOrderLayer = PixMenuOrderLayer::create();
                    scene->addChild(menuOrderLayer);
                }
                else if(strKey.compare("COUPON_LIST") == 0)
                {
                    PixMenuCouponLayer *menuCouponLayer = PixMenuCouponLayer::create();
                    scene->addChild(menuCouponLayer);
                }
                else if(strKey.compare("SET_COUPON") == 0)
                {
                    PixMenuCouponLayer *menuCouponLayer = PixMenuCouponLayer::create();
                    menuCouponLayer->showRegisterLayer();
                    scene->addChild(menuCouponLayer);
                }
                else if(strKey.compare("EVENT") == 0)
                {
                    PixMenuEventLayer *menuEventLayer = PixMenuEventLayer::create();
                    scene->addChild(menuEventLayer);
                }
                else if(strKey.compare("EVENT_DETAIL") == 0)
                {
                    int nID = root->intForKey("EVENT_DETAIL");
                    
                    std::string strUrl = SREVERURL_MANAGER->getBoardWebViewUrl(BOARD_EVENT,nID);
                    
                    PixMenuEventDetailLayer *menuEventDetailLayer = PixMenuEventDetailLayer::create(strUrl);
                    scene->addChild(menuEventDetailLayer);
                }
                else if(strKey.compare("NOTICE") == 0)
                {
                    PixMenuNoticeLayer *menuNoticeLayer = PixMenuNoticeLayer::create();
                    scene->addChild(menuNoticeLayer);
                }
                else if(strKey.compare("NOTICE_DETAIL") == 0)
                {
                    int nID = root->intForKey("NOTICE_DETAIL");
                    std::string strUrl = SREVERURL_MANAGER->getBoardWebViewUrl(BOARD_NOTICE,nID);
                    
                    PixMenuNoticeiDetailLayer *menuNoticeiDetailLayer = PixMenuNoticeiDetailLayer::create(strUrl);
                    scene->addChild(menuNoticeiDetailLayer);
                }
                else if(strKey.compare("ALBUM") == 0)
                {
                    // 열려 있는 레이어들만 닫는 이벤트
                }
                else if(strKey.compare("NATIVE_BROWSER") == 0)
                {
                    std::string strUrl = root->stringForKey("NATIVE_BROWSER");
                    PixNativeUtils::goURL(strUrl.c_str());
                }
            }
        }
    }
}

std::string PixOneSignalManager::getUserID()
{
    return mstr_UserID;
}

std::string PixOneSignalManager::getPushToken()
{
    return mstr_PushToken;
}

bool PixOneSignalManager::isAdditionalData()
{
    return !additionalDataQueue.empty();
}

void PixOneSignalManager::setIntroFinish(bool var)
{
 
    PixNativeUtils::nativeCallIntroFinish(var);
    mbln_IntroFinish = var;
}

bool PixOneSignalManager::getIntroFinish()
{
    return mbln_IntroFinish;
}
