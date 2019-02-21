//
//  PixUserDefault.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 31..
//
//

#include "PixUserDefault.h"

std::string PixUserDefault::GetUUID()
{
    return UserDefault::getInstance()->getStringForKey("USER_UUID", "0");
}

void PixUserDefault::SetUUID(const std::string &value)
{
    UserDefault::getInstance()->setStringForKey("USER_UUID", value);
    UserDefault::getInstance()->flush();
}

std::string PixUserDefault::GetMemberID()
{
    return UserDefault::getInstance()->getStringForKey("MEMBER_ID", "0");
}

void PixUserDefault::SetMemberID(const std::string &value)
{
    UserDefault::getInstance()->setStringForKey("MEMBER_ID", value);
    UserDefault::getInstance()->flush();
}

std::string PixUserDefault::GetUserID()
{
    return UserDefault::getInstance()->getStringForKey("USER_ID", "0");
}

void PixUserDefault::SetUserID(const std::string &value)
{
    UserDefault::getInstance()->setStringForKey("USER_ID", value);
    UserDefault::getInstance()->flush();
}

std::string PixUserDefault::GetName()
{
    return UserDefault::getInstance()->getStringForKey("NAME", "");
}

void PixUserDefault::SetName(const std::string &value)
{
    UserDefault::getInstance()->setStringForKey("NAME", value);
    UserDefault::getInstance()->flush();
}

std::string PixUserDefault::GetGrade()
{
    return UserDefault::getInstance()->getStringForKey("GRADE", "0");
}

void PixUserDefault::SetGrade(const std::string &value)
{
    UserDefault::getInstance()->setStringForKey("GRADE", value);
    UserDefault::getInstance()->flush();
}

std::string PixUserDefault::GetPhone()
{
    return UserDefault::getInstance()->getStringForKey("PHONE", "0");
}

void PixUserDefault::SetPhone(const std::string &value)
{
    UserDefault::getInstance()->setStringForKey("PHONE", value);
    UserDefault::getInstance()->flush();
}

std::string PixUserDefault::GetEmail()
{
    return UserDefault::getInstance()->getStringForKey("EMAIL", "");
}

void PixUserDefault::SetEmail(const std::string &value)
{
    UserDefault::getInstance()->setStringForKey("EMAIL", value);
    UserDefault::getInstance()->flush();
}

int PixUserDefault::GetUserType()
{
    return UserDefault::getInstance()->getIntegerForKey("USER_TYPE", 0);
}

void PixUserDefault::SetUserType(int value)
{
    UserDefault::getInstance()->setIntegerForKey("USER_TYPE", value);
    UserDefault::getInstance()->flush();
}

bool PixUserDefault::GetSmartAlbumTutorial()
{
    return UserDefault::getInstance()->getBoolForKey("SMART_ALBUM_TUTROIAL", true);
}

void PixUserDefault::SetSmartAlbumTutorial(bool value)
{
    UserDefault::getInstance()->setBoolForKey("SMART_ALBUM_TUTROIAL", value);
    UserDefault::getInstance()->flush();
}

bool PixUserDefault::GetThemeGuide()
{
    return UserDefault::getInstance()->getBoolForKey("THEME_GUIDE", true);
}

void PixUserDefault::SetThemeGuide(bool value)
{
    UserDefault::getInstance()->setBoolForKey("THEME_GUIDE", value);
    UserDefault::getInstance()->flush();
}

bool PixUserDefault::GetPhotoGuide()
{
    return UserDefault::getInstance()->getBoolForKey("PHOTO_GUIDE", true);
}

void PixUserDefault::SetPhotoGuide(bool value)
{
    UserDefault::getInstance()->setBoolForKey("PHOTO_GUIDE", value);
    UserDefault::getInstance()->flush();
}

std::string PixUserDefault::GetAccountsGoogleToken()
{
    return UserDefault::getInstance()->getStringForKey("ACCOUNTS_GOOGLE_TOKEN", "");
}

void PixUserDefault::SetAccountsGoogleToken(const std::string &value)
{
    UserDefault::getInstance()->setStringForKey("ACCOUNTS_GOOGLE_TOKEN", value);
    UserDefault::getInstance()->flush();
}

std::string PixUserDefault::GetAccountsGoogleUserInfo()
{
    return UserDefault::getInstance()->getStringForKey("ACCOUNTS_GOOGLE_USERINFO", "");
}

void PixUserDefault::SetAccountsGoogleUserInfo(const std::string &value)
{
    UserDefault::getInstance()->setStringForKey("ACCOUNTS_GOOGLE_USERINFO", value);
    UserDefault::getInstance()->flush();
}

int PixUserDefault::GetNoticeID()
{
    return UserDefault::getInstance()->getIntegerForKey("NOTICE_ID", 0);
}

void PixUserDefault::setNoticeID(int value)
{
    UserDefault::getInstance()->setIntegerForKey("NOTICE_ID", value);
    UserDefault::getInstance()->flush();
}

int PixUserDefault::GetEventID()
{
    return UserDefault::getInstance()->getIntegerForKey("EVENT_ID", 0);
}

void PixUserDefault::setEventID(int value)
{
    UserDefault::getInstance()->setIntegerForKey("EVENT_ID", value);
    UserDefault::getInstance()->flush();
}

bool PixUserDefault::GetMyAlbumEvent()
{
    return UserDefault::getInstance()->getBoolForKey("MY_ALBUM_EVENT", false);
}

void PixUserDefault::SetMyAlbumEvent(int value)
{
    UserDefault::getInstance()->setBoolForKey("MY_ALBUM_EVENT", value);
    UserDefault::getInstance()->flush();
}

// 옵션

float PixUserDefault::GetSoundVolume()
{
    return UserDefault::getInstance()->getFloatForKey("SOUND_VOLUME", 0.7f);
}

void PixUserDefault::SetSoundVolume(float value)
{
    UserDefault::getInstance()->setFloatForKey("SOUND_VOLUME", value);
    UserDefault::getInstance()->flush();
}

float PixUserDefault::GetEffectVolume()
{
    return UserDefault::getInstance()->getFloatForKey("EFFECT_VOLUME", 0.7f);
}

void PixUserDefault::SetEffectVolume(float value)
{
    UserDefault::getInstance()->setFloatForKey("EFFECT_VOLUME", value);
    UserDefault::getInstance()->flush();
}

bool PixUserDefault::GetViberation()
{
    return UserDefault::getInstance()->getBoolForKey("VIBRATION_ENABLE", true);
}

void PixUserDefault::SetViberation(bool value)
{
    UserDefault::getInstance()->setBoolForKey("VIBRATION_ENABLE", value);
    UserDefault::getInstance()->flush();
}

bool PixUserDefault::GetDisplayHold()
{
    return UserDefault::getInstance()->getBoolForKey("DISPLAY_HOLD", false);
}

void PixUserDefault::SetDisplayHold(bool value)
{
    UserDefault::getInstance()->setBoolForKey("DISPLAY_HOLD", value);
    UserDefault::getInstance()->flush();
}

int PixUserDefault::GetDisplayOrientation(int defValue)
{
    return UserDefault::getInstance()->getIntegerForKey("DIAPLAY_ORIENTATION", defValue);
}

void PixUserDefault::SetDisplayOrientation(int value)
{
    UserDefault::getInstance()->setIntegerForKey("DIAPLAY_ORIENTATION", value);
    UserDefault::getInstance()->flush();
}

bool PixUserDefault::GetDisplayStats()
{
    return UserDefault::getInstance()->getBoolForKey("DISPLAYSTATS_ENABLE", true);
}

void PixUserDefault::SetDisplayStats(bool value)
{
    UserDefault::getInstance()->setBoolForKey("DISPLAYSTATS_ENABLE", value);
    UserDefault::getInstance()->flush();
}

bool PixUserDefault::GetAlarm()
{
    return UserDefault::getInstance()->getBoolForKey("ALARM", true);
}

void PixUserDefault::SetAlarm(bool value)
{
    UserDefault::getInstance()->setBoolForKey("ALARM", value);
    UserDefault::getInstance()->flush();
}
