//
//  PixGoogleAnalyticsManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 17..
//
//

#include "PixGoogleAnalyticsManager.h"
#include "PixServerUrlManager.h"

static PixGoogleAnalyticsManager* _sharedObject = nullptr;

PixGoogleAnalyticsManager* PixGoogleAnalyticsManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixGoogleAnalyticsManager();
    }
    
    return _sharedObject;
}

PixGoogleAnalyticsManager::PixGoogleAnalyticsManager()
{
    
}

PixGoogleAnalyticsManager::~PixGoogleAnalyticsManager()
{
    
}

void PixGoogleAnalyticsManager::init()
{
    sdkbox::PluginGoogleAnalytics::init();
}

void PixGoogleAnalyticsManager::logEvent(const std::string& eventCategory, const std::string& eventAction, const std::string& eventLabel, int value)
{
    std::string strEventLabel;
    
    if(SREVERURL_MANAGER->isDev())
    {
        strEventLabel.append("dev_");
    }
    
    strEventLabel.append(eventAction);
    
    sdkbox::PluginGoogleAnalytics::logEvent(eventCategory, eventAction, strEventLabel, value);
    sdkbox::PluginGoogleAnalytics::dispatchHits();
}