//
//  PixServerUrlManager.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 25..
//
//

#ifndef PixServerUrlManager_h
#define PixServerUrlManager_h

#include "cocos2d.h"

#define SREVERURL_MANAGER     (PixServerUrlManager::sharedObject())

USING_NS_CC;

#define IOS_APP_URL "itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewSoftware?id=1139711530&mt=8"
#define AOS_APP_URL "market://details?id=com.pixholic.pzle"

typedef enum
{
    BOARD_NOTICE =  1,
    BOARD_EVENT =   2,
    
} BOARD_TYPE;

class PixServerConfig
{
public:
    
    PixServerConfig()
    {
        mn_image_max_upload = 0;
        mf_Resouce_download_timeout = 0.0f;
    }
    
    CC_SYNTHESIZE(int , mn_image_max_upload, Image_max_upload)
    CC_SYNTHESIZE(float, mf_Resouce_download_timeout, Resouce_download_timeout)
};

class PixDictionary;
class PixArray;

class PixServerUrlManager
{
 
public :
    
    static PixServerUrlManager* sharedObject();
    
    PixServerUrlManager();
    virtual ~PixServerUrlManager();
    
    void clear();
    void init();
    std::string getUrl(const std::string &key);
    std::string getPhotoServerUrl();
    std::string getResourceServerUrl();
    std::string getAlbumServerUrl();
    PixArray* getBank();
    PixDictionary* getWebViewDictionary();
    PixDictionary* getUpdateInfo();
    PixDictionary* getServerNoticeInfo();
    std::string getWebViewUrl(const std::string &key);
    std::string getBoardWebViewUrl(BOARD_TYPE type, int nDetail_ID);
    std::string getAppUrl();

    inline bool isDev()
    {
        return mbln_IsDev;
    }
    
    int getImageMaxUploadCount();
    float getResouceDownloadTimeout();
    
private:
    
    PixDictionary *dictionary;
    PixServerConfig *serverConfig;
    bool mbln_IsDev;
};

#endif /* PixServerUrlManager_h */
