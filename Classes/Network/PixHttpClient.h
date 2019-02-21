//
//  PixHttpClient.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 11..
//
//

// 네트웍 통신 모듈

#ifndef PixHttpClient_h
#define PixHttpClient_h

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "../Util/PixDictionary.h"
#include "../Util/pugixml/pugixml.hpp"
#include "../UIControl/PixAlert.h"
#include "../Model/PixNetworkImageModel.h"
#include "PixHttpRequest.h"
#include "../Manager/PixGoogleAnalyticsManager.h"
#include "../Manager/PixServerUrlManager.h"

USING_NS_CC;

using namespace network;

class PixHttpClient : public Ref
{
    
public :
    
    static PixHttpClient* sharedObject();
    
    PixHttpClient();
    virtual ~PixHttpClient();
    void destroyInstance();
    void setTimeoutForConnect(int value);
    
    void sendGetRequest(std::string url, std::string requestTag, Ref* pTarget, SEL_HttpResponse pSelector, SEL_HttpResponse pError, void* pUserData = nullptr, bool blnSendImmediate = false);
    void sendGetRequest(std::string url, std::vector<std::string> &headers, std::string requestTag, Ref* pTarget, SEL_HttpResponse pSelector, SEL_HttpResponse pError, void* pUserData = nullptr, bool blnSendImmediate = false);
    void sendGetRequest(std::string url, std::map<std::string, std::string> &postParams, std::string requestTag, Ref* pTarget, SEL_HttpResponse pSelector, SEL_HttpResponse pError, bool blnSendImmediate = false);
    void sendPutRequest(std::string url, std::map<std::string, std::string> &postParams, std::string requestTag, Ref* pTarget, SEL_HttpResponse pSelector, SEL_HttpResponse pError, bool blnSendImmediate = false);
    void getFormData(std::string url, std::map<std::string, std::string> &postParams, std::string requestTag, Ref* pTarget, SEL_HttpResponse pSelector, SEL_HttpResponse pError, void* pUserData = nullptr, bool blnSendImmediate = false);
        
    void cancelRequest(Ref* pTarget);
    
    void getAES256EncryptPostParams(std::map<std::string, std::string> &postParams);
    std::string getBase64String(const std::string &data);
    void getPostData(std::ostringstream &postBuf, std::map<std::string, std::string> &postParams);

    PixDictionary* getDictionaryFromElement(cocos2d::network::HttpResponse *pResponse);
    PixDictionary* getDictionaryFromElement(const std::string &strResponseData);
    PixDictionary* getDictionary(const rapidjson::Value& node);
    PixArray* getArray(const rapidjson::Value& root);
    bool isRequest(const std::string &requestTag);
    
private:
    
    std::deque<PixHttpRequest*> _requestQueue;
    std::deque<PixHttpRequest*> _requestImmediateQueue;
    std::deque<PixHttpRequest*> _responseImmediateQueue;
    
    int mn_RequestImmediateCount;
    
    CC_SYNTHESIZE(int, mn_MaxRequestImmediateCount, MaxRequestImmediateCount)
    
    void httpRequestSend(PixHttpRequest* pRequest);
    void httpRequestSendSchedule(float dt);
    void requestQueueErase(PixHttpRequest* pRequest);
    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void writeFileFromRequest(cocos2d::network::HttpResponse *response,std::string filename);
};

#endif /* PixHttpClient_h */


