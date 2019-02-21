//
//  PixHttpRequest.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 22..
//
//

#ifndef PixHttpRequest_h
#define PixHttpRequest_h

#include "cocos2d.h"
#include "../cocos2d/extensions/cocos-ext.h"
#include "network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace network;

class CC_DLL PixHttpRequest : public cocos2d::network::HttpRequest
{
public :
    
    PixHttpRequest();
    virtual ~PixHttpRequest();

    inline void setHttpRequestCompletedCallback(Ref* pTarget, SEL_HttpResponse pSelector)
    {
        target = pTarget;
        callback = pSelector;
    }
    
    inline void setHttpRequestError(Ref* pTarget, SEL_HttpResponse pSelector)
    {
        target = pTarget;
        error = pSelector;
    }
    
    inline void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
    {
        if (target != nullptr && callback != nullptr)
        {
            (target->*callback)(sender,response);
        }
    }
    
    inline void onHttpRequestError(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
    {
        if (target != nullptr && error != nullptr)
        {
            (target->*error)(sender,response);
        }
    }
    
    inline bool cancelRequest(Ref* pTarge)
    {
        if(this->target == pTarge)
        {
            setHttpRequestCompletedCallback(nullptr,nullptr);
            setHttpRequestError(nullptr,nullptr);
            return true;
        }
        
        return false;
    }
    
    inline bool isImmediate()
    {
        return mbln_Immediate;
    }
    
    inline void setImmediate(bool var)
    {
        mbln_Immediate = var;
    }
    
private:
    
    bool mbln_Immediate;
    
    Ref *target;
    SEL_HttpResponse callback;
    SEL_HttpResponse error;
};

#endif /* PixHttpRequest_h */
