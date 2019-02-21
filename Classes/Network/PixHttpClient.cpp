//
//  PixHttpClient.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 11..
//
//

#include "PixHttpClient.h"
#include "../Scene/PixApp.h"
#include "base/base64.h"
#include "../Util/PixStringUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../Manager/PixMessageManager.h"

static PixHttpClient* _sharedObject = nullptr;

PixHttpClient* PixHttpClient::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixHttpClient();
    }
    
    return _sharedObject;
}

PixHttpClient::PixHttpClient()
: mn_RequestImmediateCount(0)
, mn_MaxRequestImmediateCount(5)
{
    _requestQueue.clear();
    _requestImmediateQueue.clear();
    _responseImmediateQueue.clear();
}

PixHttpClient::~PixHttpClient()
{
    _requestQueue.clear();
    _requestImmediateQueue.clear();
    _responseImmediateQueue.clear();
    
    Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(PixHttpClient::httpRequestSendSchedule), this);
}

void PixHttpClient::destroyInstance()
{
    network::HttpClient::getInstance()->destroyInstance();
}

void PixHttpClient::setTimeoutForConnect(int value)
{
    network::HttpClient::getInstance()->setTimeoutForConnect(value);
}

void PixHttpClient::sendGetRequest(std::string url, std::string requestTag, Ref* pTarget, SEL_HttpResponse pSelector, SEL_HttpResponse pError, void* pUserData, bool blnSendImmediate)
{
    PixHttpRequest* request = new PixHttpRequest();
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setUrl(url.c_str());
    request->setHttpRequestCompletedCallback(pTarget, pSelector);
    request->setHttpRequestError(pTarget, pError);
    request->setResponseCallback(CC_CALLBACK_2(PixHttpClient::onHttpRequestCompleted, this));
    request->setTag(requestTag.c_str());
    request->setUserData(pUserData);
    request->setImmediate(blnSendImmediate);
    
    httpRequestSend(request);
}

void PixHttpClient::sendGetRequest(std::string url, std::vector<std::string> &headers, std::string requestTag, Ref* pTarget, SEL_HttpResponse pSelector, SEL_HttpResponse pError, void* pUserData, bool blnSendImmediate)
{
    
    PixHttpRequest* request = new PixHttpRequest();
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setUrl(url.c_str());
    request->setHeaders(headers);
    request->setHttpRequestCompletedCallback(pTarget, pSelector);
    request->setHttpRequestError(pTarget, pError);
    request->setResponseCallback(CC_CALLBACK_2(PixHttpClient::onHttpRequestCompleted, this));
    request->setTag(requestTag.c_str());
    request->setUserData(pUserData);
    request->setImmediate(blnSendImmediate);
    
    httpRequestSend(request);
}

void PixHttpClient::sendGetRequest(std::string url, std::map<std::string, std::string> &postParams, std::string requestTag, Ref* pTarget, SEL_HttpResponse pSelector, SEL_HttpResponse pError, bool blnSendImmediate)
{
    PixHttpRequest* request = new PixHttpRequest();
    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
    request->setUrl(url.c_str());
    request->setHttpRequestCompletedCallback(pTarget, pSelector);
    request->setHttpRequestError(pTarget, pError);
    request->setResponseCallback(CC_CALLBACK_2(PixHttpClient::onHttpRequestCompleted, this));
    
    std::ostringstream postBuf;
    getPostData(postBuf, postParams);
    std::string postData = postBuf.str();
    
    request->setRequestData(postData.c_str(), postData.size());
    request->setTag(requestTag.c_str());
    CCLOG("url = %s",request->getUrl());
    
    request->setTag(requestTag.c_str());
    request->setImmediate(blnSendImmediate);
    
    httpRequestSend(request);
  
}

void PixHttpClient::sendPutRequest(std::string url, std::map<std::string, std::string> &postParams, std::string requestTag, Ref* pTarget, SEL_HttpResponse pSelector,  SEL_HttpResponse pError, bool blnSendImmediate)
{
    PixHttpRequest* request = new PixHttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::PUT);
    request->setHttpRequestCompletedCallback(pTarget, pSelector);
    request->setHttpRequestError(pTarget, pError);
    request->setResponseCallback(CC_CALLBACK_2(PixHttpClient::onHttpRequestCompleted, this));
    
    std::ostringstream postBuf;
    getPostData(postBuf, postParams);
    
    std::string postData = postBuf.str();

    request->setRequestData(postData.c_str(), postData.size());
    request->setTag(requestTag.c_str());
    request->setImmediate(blnSendImmediate);
    CCLOG("url = %s",request->getUrl());
    
    httpRequestSend(request);
}

void PixHttpClient::getFormData(std::string url, std::map<std::string, std::string> &postParams, std::string requestTag, Ref* pTarget, SEL_HttpResponse pSelector, SEL_HttpResponse pError, void* pUserData, bool blnSendImmediate)
{
    PixHttpRequest* request = new PixHttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setHttpRequestCompletedCallback(pTarget, pSelector);
    request->setHttpRequestError(pTarget, pError);
    request->setResponseCallback(CC_CALLBACK_2(PixHttpClient::onHttpRequestCompleted, this));
    
    std::ostringstream postBuf;
    getPostData(postBuf, postParams);
    
    std::string postData = postBuf.str();
    
    request->setRequestData(postData.c_str(), postData.size());
    request->setTag(requestTag.c_str());
    request->setUserData(pUserData);
    request->setImmediate(blnSendImmediate);
    CCLOG("url = %s",request->getUrl());
    
    httpRequestSend(request);
}

void PixHttpClient::cancelRequest(Ref* pTarget)
{
    if(pTarget)
    {
        if(!_requestQueue.empty())
        {
            for (auto iter = _requestQueue.begin(); iter != _requestQueue.end(); )
            {
                if((*iter)->cancelRequest(pTarget))
                {
                    iter = _requestQueue.erase(iter);
                }
                else
                {
                    ++iter;
                }
            }
        }
        
        if(!_requestImmediateQueue.empty())
        {
            for (auto iter = _requestImmediateQueue.begin(); iter != _requestImmediateQueue.end(); )
            {
                if((*iter)->cancelRequest(pTarget))
                {
                    iter = _requestImmediateQueue.erase(iter);
                }
                else
                {
                    ++iter;
                }
            }
        }
        
        if(!_responseImmediateQueue.empty())
        {
            for (auto iter = _responseImmediateQueue.begin(); iter != _responseImmediateQueue.end(); )
            {
                if((*iter)->cancelRequest(pTarget))
                {
                    iter = _responseImmediateQueue.erase(iter);
                }
                else
                {
                    ++iter;
                }
            }
        }
        
        _requestImmediateQueue.clear();
        _responseImmediateQueue.clear();

        mn_RequestImmediateCount = 0;
    }
    
    Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(PixHttpClient::httpRequestSendSchedule), this);
}

#pragma mark - Util

void PixHttpClient::getAES256EncryptPostParams(std::map<std::string, std::string> &postParams)
{
    std::ostringstream postBuf;
    getPostData(postBuf, postParams);
    
    std::string postData = postBuf.str();
    
    postData = PixNativeUtils::nativeCallAES256Encrypt(postData);
    postData = PixStringUtils::replaceAll(postData,"+",".");
    postData = PixStringUtils::replaceAll(postData,"=","-");
    postData = PixStringUtils::replaceAll(postData,"/","_");
    
    postParams.clear();
    postParams["u"] = postData;
}

std::string PixHttpClient::getBase64String(const std::string &data)
{
    std::string strBase64PostData = data;
    std::string strBase64Encode;
    
    char *encodedData = nullptr;
    base64Encode((unsigned char*)strBase64PostData.c_str(), (unsigned int)strBase64PostData.length(), &encodedData);
    
    strBase64Encode = std::string(encodedData);
    CC_SAFE_FREE(encodedData);
    
    return strBase64Encode;
}

void PixHttpClient::getPostData(std::ostringstream &postBuf, std::map<std::string, std::string> &postParams)
{
    std::map<std::string, std::string>::iterator it;
    
    for (it = postParams.begin(); it != postParams.end(); ++it)
    {
        postBuf << it->first << "=" << it->second << "&" ;
    }
    
    postBuf << std::flush ;
}

PixDictionary* PixHttpClient::getDictionaryFromElement(cocos2d::network::HttpResponse *pResponse)
{
    if(pResponse)
    {
        std::vector<char>* data = pResponse->getResponseData();
        std::string strResponseData(data->begin(), data->end());
       
        CCLOG("getDictionaryFromElement %s", strResponseData.c_str());
        
        rapidjson::Document document;
        document.Parse<0>(strResponseData.c_str());
        
        if (document.HasParseError() == rapidjson::kParseErrorNone)
        {
            return getDictionary(document);
        }
    }
    
    return nullptr;
}

PixDictionary* PixHttpClient::getDictionaryFromElement(const std::string &strResponseData)
{
    if(!strResponseData.empty())
    {
//        CCLOG("getDictionaryFromElement %s", strResponseData.c_str());
        
        rapidjson::Document document;
        document.Parse<0>(strResponseData.c_str());
        
        if (document.HasParseError() == rapidjson::kParseErrorNone)
        {
            return getDictionary(document);
        }
    }
    
    return nullptr;
}

PixDictionary* PixHttpClient::getDictionary(const rapidjson::Value& node)
{
    if (node.IsNull())
    {
        return nullptr;
    }
    
    PixDictionary *dic = PixDictionary::create();
    
    rapidjson::Value::ConstMemberIterator it = node.MemberBegin();
    
    while (it != node.MemberEnd())
    {
        const char *key = it->name.GetString();
        
        if(it->value.GetType() == rapidjson::kNullType)
        {
            // 데이터가 널일때 예외처리
            __String *str = new __String("");
            dic->setObject(str, key);
            str->release();
            //            return nullptr;
        }
        else if (it->value.GetType() == rapidjson::Type::kArrayType) // 배열인 경우
        {
            dic->setObject(getArray(it->value), key);
        }
        
        else if (it->value.GetType() == rapidjson::Type::kObjectType) // 딕셔너리인 경우
        {
            dic->setObject(getDictionary(it->value), key);
        }
        else   // 값일 경우 무조건 CCString 형태로 저장
        {
            const char *node_value = nullptr;
            
            rapidjson::Type type = it->value.GetType();
            
            if(type == rapidjson::Type::kStringType)
            {
                node_value = it->value.GetString();
            }
            else if(type == rapidjson::Type::kNumberType)
            {
                if(it->value.IsDouble())
                {
                    node_value = PixStringUtils::valueToString(it->value.GetDouble()).c_str();
                }
                else
                {
                    node_value = PixStringUtils::valueToString(it->value.GetInt()).c_str();
                }
            }
            else if(type == rapidjson::Type::kTrueType)
            {
                node_value = "Y";
            }
            else if(type == rapidjson::Type::kFalseType)
            {
                node_value = "N";
            }

            if(node_value != nullptr)
            {
                __String *str = new __String(node_value);
                
                dic->setObject(str, key);
                str->release();
            }
        }
        
        ++it;
    }
    
    return dic;
}

PixArray* PixHttpClient::getArray(const rapidjson::Value& node)
{
    if (node.IsNull())
    {
        return nullptr;
    }
    
    rapidjson::Value::ConstValueIterator it = node.Begin();
    
    PixArray *array = PixArray::create();
    
    while (it != node.End())
    {
        if(it->GetType() == rapidjson::Type::kNullType)
        {
            // 데이터가 널일때 예외처리
            __String *str = new __String("");
            array->addObject(str);
            str->release();
//            return nullptr;
        }
        else if (it->GetType() == rapidjson::Type::kArrayType) // 배열인 경우
        {
            array->addObject(getArray(*it));
        }
        
        else if (it->GetType() == rapidjson::kObjectType)  // 딕셔너리인 경우
        {
            array->addObject(getDictionary(*it));
        }
        
        else   // 값일 경우 무조건 CCString 형태로 저장
        {
            const char *node_value = nullptr;
            
            rapidjson::Type type = it->GetType();
            
            if(type == rapidjson::Type::kStringType)
            {
                node_value = it->GetString();
            }
            else if(type == rapidjson::Type::kNumberType)
            {
                if(it->IsDouble())
                {
                    node_value = PixStringUtils::valueToString(it->GetDouble()).c_str();
                }
                else
                {
                    node_value = PixStringUtils::valueToString(it->GetInt()).c_str();
                }
            }
            else if(type == rapidjson::Type::kTrueType)
            {
                node_value = "Y";
            }
            else if(type == rapidjson::Type::kFalseType)
            {
                node_value = "N";
            }
            
            if(node_value != nullptr)
            {
                __String *str = new __String(node_value);
                
                array->addObject(str);
                str->release();
            }
        }

        ++it;
    }
    
    return array;
}

bool PixHttpClient::isRequest(const std::string &requestTag)
{
    if(!_requestQueue.empty())
    {
        for (auto iter = _requestQueue.begin(); iter != _requestQueue.end(); ++iter)
        {
            PixHttpRequest *request = (*iter);
            
            if(request)
            {
                if(requestTag.compare(request->getTag()) == 0)
                {
                    return true;
                }
            }
        }
    }
    
    if(!_responseImmediateQueue.empty())
    {
        for (auto iter = _responseImmediateQueue.begin(); iter != _responseImmediateQueue.end(); ++iter)
        {
            PixHttpRequest *request = (*iter);
            
            if(request)
            {
                if(requestTag.compare(request->getTag()) == 0)
                {
                    return true;
                }
            }
        }
    }
    
    return false;
}

#pragma mark - private

void PixHttpClient::httpRequestSend(PixHttpRequest* pRequest)
{
    if(pRequest == nullptr)
        return;
    
    if(pRequest->isImmediate())
    {
        _requestImmediateQueue.push_back(pRequest);
        
        if(mn_RequestImmediateCount == 0)
        {
            Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(PixHttpClient::httpRequestSendSchedule), this, 0, false);
        }
    }
    else
    {
        network::HttpClient::getInstance()->send(pRequest);
        pRequest->release();
        _requestQueue.push_back(pRequest);
    }
}

void PixHttpClient::httpRequestSendSchedule(float dt)
{    
    while (true)
    {
        if(mn_RequestImmediateCount < mn_MaxRequestImmediateCount)
        {
            if(!_requestImmediateQueue.empty())
            {
                PixHttpRequest *pixHttpRequest = _requestImmediateQueue.front();
                _requestImmediateQueue.pop_front();
                
                network::HttpClient::getInstance()->sendImmediate(pixHttpRequest);
                pixHttpRequest->release();
                ++mn_RequestImmediateCount;
                
                _responseImmediateQueue.push_back(pixHttpRequest);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}

void PixHttpClient::requestQueueErase(PixHttpRequest* pRequest)
{
    if(pRequest)
    {
        if(!_requestQueue.empty())
        {
            for (auto iter = _requestQueue.begin(); iter != _requestQueue.end(); ++iter)
            {
                if((*iter))
                {
                    if((*iter) == pRequest)
                    {
                        _requestQueue.erase(iter);
                        break;
                    }
                }
            }
        }
        
        if(!_responseImmediateQueue.empty())
        {
            for (auto iter = _responseImmediateQueue.begin(); iter != _responseImmediateQueue.end(); ++iter)
            {
                if((*iter))
                {
                    if((*iter) == pRequest)
                    {
                        _responseImmediateQueue.erase(iter);
                        --mn_RequestImmediateCount;
                        break;
                    }
                }
            }   
        }
    }
}

void PixHttpClient::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (!response)
    {
        if(Director::getInstance()->getRunningScene()->getChildByTag(ALERT_TAG) == nullptr)
        {
            PixAlert::show("Error", "Error", nullptr, ALERT_TYPE_OK);
        }
        
        PIXAPP->hideLoading();
        return;
    }
    
    if (0 != strlen(response->getHttpRequest()->getTag())) {
        CCLOG("%s compeled",response->getHttpRequest()->getTag());
    }
    
    long statusCode = response->getResponseCode();
    std::string statusString = StringUtils::format("HTTP Status Code:%ld , tag = %s", statusCode,response->getHttpRequest()->getTag());
    
    CCLOG("response code:%s",statusString.c_str());
    
    PixHttpRequest *request = dynamic_cast<PixHttpRequest*>(response->getHttpRequest());
    
    requestQueueErase(request);
    
    if (!response->isSucceed())
    {
        CCLOG("ErrorBuffer : %s",response->getErrorBuffer());
        
        if(Director::getInstance()->getRunningScene()->getChildByTag(ALERT_TAG) == nullptr)
        {
            if(MASSAGE_MANAGER->managerMapEmpty())
            {
                std::string strText = MASSAGE_MANAGER->getNetworkErrorMessage(false);
                PixAlert::show("Error", strText.c_str(), nullptr, ALERT_TYPE_OK);
            }
            else
            {
                PixAlert::show("Error", response->getErrorBuffer(), nullptr, ALERT_TYPE_OK);
            }
        }
        
        PIXAPP->hideLoading();
        
        if(request)
        {
            request->onHttpRequestError(sender,response);
        }
        
        return;
    }
    
    if(((int)statusCode) == 200)
    {
        if(request)
        {
            request->onHttpRequestCompleted(sender,response);
        }
    }
    else
    {
        PixDictionary *root= getDictionaryFromElement(response);
        
        if(root)
        {
            if(Director::getInstance()->getRunningScene()->getChildByTag(ALERT_TAG) == nullptr)
            {
                std::string errorMsg = root->stringForKey("error");
                CCLOG("PixHttpClient errorMsg : %s",errorMsg.c_str());
                
                PixAlert::show("Error", errorMsg.c_str(), nullptr, ALERT_TYPE_OK);
            }
            
            PIXAPP->hideLoading();
        }
        else
        {
            if(Director::getInstance()->getRunningScene()->getChildByTag(ALERT_TAG) == nullptr)
            {
                std::string msg = StringUtils::format("statusCode %d Error", (int)statusCode);
                PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
            }
            
            PIXAPP->hideLoading();
        }
        
        if(request)
        {
            request->onHttpRequestError(sender,response);
        }
    }
}

#pragma mark - Private 

void PixHttpClient::writeFileFromRequest(cocos2d::network::HttpResponse *response,std::string filename)
{
    std::vector<char>* buffer = response->getResponseData();
    //    printf("Http Test,dump data:");
    std::string path = FileUtils::getInstance()->getWritablePath();
    std::string fullPath =  path + response->getHttpRequest()->getTag() + "/" + filename;
    FILE* fp = fopen(fullPath.c_str(), "wb");
    
    CCLOG("PixHttpClient::writeFileFromRequest : %s , %d",fullPath.c_str(), (int)buffer->size());
    
    unsigned char bf;
    
    for (unsigned int i  = 0; i < buffer->size(); i++)
    {
        bf = buffer->at(i);
        fwrite(&bf, 1, 1, fp);
    }
    
    fclose(fp);
}
