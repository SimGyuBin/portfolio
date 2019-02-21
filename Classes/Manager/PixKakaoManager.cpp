//
//  PixKakaoManager.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 6. 16..
//
//

#include "PixKakaoManager.h"
#include "../Util/PixNativeUtils.h"

static PixKakaoManager* _sharedObject = nullptr;

PixKakaoManager* PixKakaoManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixKakaoManager();
    }
    
    return _sharedObject;
}

PixKakaoManager::PixKakaoManager()
: _userInfoCallBack(nullptr)
, mstr_UserID("")
, mstr_UserName("")
, mstr_UserEmail("")
, mstr_UserProfile("")
{
    
}

PixKakaoManager::~PixKakaoManager()
{
    
}

void PixKakaoManager::init()
{
    
}

void PixKakaoManager::kakaoLogin(const std::function<void()> &callback)
{
    this->_userInfoCallBack = callback;
    
    PixNativeUtils::nativeCallKakaoLogin();
}

void PixKakaoManager::kakaoLoginSuccess()
{
    if(_userInfoCallBack)
    {
        (_userInfoCallBack)();
        _userInfoCallBack = nullptr;
    }
}

void PixKakaoManager::kakaoLogout()
{
    mstr_UserID.clear();
    mstr_UserName.clear();
    mstr_UserEmail.clear();
    mstr_UserProfile.clear();
    
    PixNativeUtils::nativeCallKakaoLogout();
}
