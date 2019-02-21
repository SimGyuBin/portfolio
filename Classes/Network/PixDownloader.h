//
//  PixDownloader.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 25..
//
//

#ifndef PixDownloader_h
#define PixDownloader_h

#include "cocos2d.h"
#include "../cocos2d/extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define DOWNLOADER_CALLBACK(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)

using namespace network;
using namespace extension;

class PixDownloaderDelegate;

class PixDownloader : public Ref
{
public:

    static PixDownloader* create();
    static PixDownloader* create(const char* downloadeUrl, const char*storagePath, PixDownloaderDelegate *pDelegate);
    
    PixDownloader();
    virtual ~PixDownloader();

    const char* getDownloadeUrl() const;
    void setDownloadeUrl(const char* downloadeUrl);
    const char* getStoragePath() const;
    void setStoragePath(const char* storagePath);
    void setConnectionTimeout(unsigned int timeout);
    unsigned int getConnectionTimeout();
    
    void createDownloadDataTask(const std::string& identifier);
    
    inline void setUserData(void* pUserData)
    {
        _pUserData = pUserData;
    }
    
    inline void* getUserData() const
    {
        return _pUserData;
    };
    
private:
    
    void init();
    void checkStoragePath();
    void onTaskError(const DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr);
    void onTaskProgress(const DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected);
    void onDataTaskSuccess(const DownloadTask& task, std::vector<unsigned char>& data);
    
    std::string _downloadeUrl;
    std::string _storagePath;
    cocos2d::network::Downloader* _downloader;
    bool _isDownloading;
    unsigned int _connectionTimeout;
    int _retryCount;
    void *  _pUserData;
   
    CC_SYNTHESIZE(bool, _write, Write)
    CC_SYNTHESIZE(PixDownloaderDelegate*, _delegate, Delegate)
    
};

class PixDownloaderDelegate
{
public:
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode) {};
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent) {};
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data) {};
};


#endif /* PixDownloader_h */
