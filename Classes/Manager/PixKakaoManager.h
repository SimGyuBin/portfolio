//
//  PixKakaoManager.h
//  Pzle
//
//  Created by 심규빈 on 2017. 6. 16..
//
//

#ifndef PixKakaoManager_h
#define PixKakaoManager_h

#include "cocos2d.h"

#define KAKAO_MANAGER     (PixKakaoManager::sharedObject())

USING_NS_CC;

class PixKakaoManager
{
public:
    static PixKakaoManager* sharedObject();
    
    PixKakaoManager();
    virtual ~PixKakaoManager();
    
    void init();
    void kakaoLogin(const std::function<void()> &callback);
    void kakaoLoginSuccess();
    void kakaoLogout();
    
private:
    
    std::function<void()> _userInfoCallBack;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_UserID, UserID)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_UserName, UserName)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_UserEmail, UserEmail)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_UserProfile, UserProfile)
};


#endif /* PixKakaoManager_h */
