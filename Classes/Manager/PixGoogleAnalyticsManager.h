//
//  PixGoogleAnalyticsManager.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 17..
//
//

#ifndef PixGoogleAnalyticsManager_h
#define PixGoogleAnalyticsManager_h

#include "cocos2d.h"
#include "PluginGoogleAnalytics/PluginGoogleAnalytics.h"

#define GOOGLEANALYTICS_MANAGER     (PixGoogleAnalyticsManager::sharedObject())

class PixGoogleAnalyticsManager
{
public:
    static PixGoogleAnalyticsManager* sharedObject();
    
    PixGoogleAnalyticsManager();
    virtual ~PixGoogleAnalyticsManager();
    
    void init();
    
    void logEvent(const std::string& eventCategory, const std::string& eventAction, const std::string& eventLabel, int value);
    
private:
};


#endif /* PixGoogleAnalyticsManager_h */
