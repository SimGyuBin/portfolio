//
//  PixUserDefault.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 31..
//
//

#ifndef PixUserDefault_h
#define PixUserDefault_h

#include "cocos2d.h"

USING_NS_CC;

class PixUserDefault {
    
public :

    static std::string GetUUID();
    static void SetUUID(const std::string &value);
    
    static std::string GetMemberID();
    static void SetMemberID(const std::string &value);
    
    static std::string GetUserID();
    static void SetUserID(const std::string &value);
    
    static std::string GetName();
    static void SetName(const std::string &value);
    
    static std::string GetGrade();
    static void SetGrade(const std::string &value);
    
    static std::string GetPhone();
    static void SetPhone(const std::string &value);
    
    static std::string GetEmail();
    static void SetEmail(const std::string &value);
    
    static int GetUserType();
    static void SetUserType(int value);
    
    static bool GetSmartAlbumTutorial();
    static void SetSmartAlbumTutorial(bool value);
    
    static bool GetThemeGuide();
    static void SetThemeGuide(bool value);
    
    static bool GetPhotoGuide();
    static void SetPhotoGuide(bool value);
    
    static std::string GetAccountsGoogleToken();
    static void SetAccountsGoogleToken(const std::string &value);
    
    static std::string GetAccountsGoogleUserInfo();
    static void SetAccountsGoogleUserInfo(const std::string &value);
    
    static int GetNoticeID();
    static void setNoticeID(int value);
    
    static int GetEventID();
    static void setEventID(int value);
    
    static bool GetMyAlbumEvent();
    static void SetMyAlbumEvent(int value);
    
    // 옵션 설정 값
    static float GetSoundVolume();
    static void SetSoundVolume(float value);
    
    static float GetEffectVolume();
    static void SetEffectVolume(float value);
    
    static bool GetViberation();
    static void SetViberation(bool value);
    
    static bool GetDisplayHold();
    static void SetDisplayHold(bool value);
    
    static int GetDisplayOrientation(int defValue);
    static void SetDisplayOrientation(int value);
    
    static bool GetDisplayStats();
    static void SetDisplayStats(bool value);
    
    static bool GetAlarm();
    static void SetAlarm(bool value);

    
};

#endif /* PixUserDefault_h */
