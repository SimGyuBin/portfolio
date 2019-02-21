//
//  PixDownloader.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 25..
//
//

#include "PixDownloader.h"
#include "../Util/PixStringUtils.h"
#include "../UIControl/PixAlert.h"

PixDownloader* PixDownloader::create()
{
    PixDownloader *downloader = new PixDownloader();
    downloader->init();
    return downloader;
}

PixDownloader* PixDownloader::create(const char* downloadeUrl, const char* storagePath, PixDownloaderDelegate *pDelegate)
{
    PixDownloader *downloader = new PixDownloader();
    downloader->setDownloadeUrl(downloadeUrl);
    downloader->setStoragePath(storagePath);
    downloader->setDelegate(pDelegate);
    downloader->init();
    return downloader;
}

PixDownloader::PixDownloader()
: _storagePath("")
, _downloadeUrl("")
, _downloader(new Downloader())
, _connectionTimeout(0)
, _retryCount(3)
, _isDownloading(false)
, _write(true)
, _delegate(nullptr)
, _pUserData(nullptr)
{
    
}

PixDownloader::~PixDownloader()
{
    CC_SAFE_DELETE(_downloader);
}

const char* PixDownloader::getDownloadeUrl() const
{
    return _downloadeUrl.c_str();
}

void PixDownloader::setDownloadeUrl(const char* downloadeUrl)
{
    _downloadeUrl = downloadeUrl;
}

const char* PixDownloader::getStoragePath() const
{
    return _storagePath.c_str();
}

void PixDownloader::setStoragePath(const char* storagePath)
{
    _storagePath = storagePath;
    checkStoragePath();
}

void PixDownloader::setConnectionTimeout(unsigned int timeout)
{
    _connectionTimeout = timeout;
}

unsigned int PixDownloader::getConnectionTimeout()
{
    return _connectionTimeout;
}

void PixDownloader::createDownloadDataTask(const std::string& identifier)
{
    _downloader->createDownloadDataTask(_downloadeUrl, identifier);
}

#pragma mark - Private

void PixDownloader::init()
{
    checkStoragePath();
    
    _downloader->onTaskError = DOWNLOADER_CALLBACK(PixDownloader::onTaskError, this);
    _downloader->onTaskProgress = DOWNLOADER_CALLBACK(PixDownloader::onTaskProgress, this);
    _downloader->onDataTaskSuccess = CC_CALLBACK_2(PixDownloader::onDataTaskSuccess, this);
}

void PixDownloader::checkStoragePath()
{
    if (_storagePath.size() > 0 && _storagePath[_storagePath.size() - 1] != '/')
    {
        _storagePath.append("/");
    }    
}

void PixDownloader::onTaskError(const DownloadTask& task, int errorCode, int errorCodeInternal, const std::string& errorStr)
{
    _isDownloading = false;
    
    if(_retryCount == 0)
    {
        if (_delegate)
        {
            _delegate->onError(this, task.identifier, errorCode);
        }
        else
        {
            PixAlert::show("Error", StringUtils::format("Error : %s",task.requestURL.c_str()).c_str(), nullptr, ALERT_TYPE_OK);
        }
    }
    else
    {
        --_retryCount;
        createDownloadDataTask(task.identifier.c_str());
    }
    
    CCLOG("PixDownloader onTaskError _retryCount  %d : ",_retryCount);
    CCLOG("PixDownloader onTaskError : %s , %s", task.identifier.c_str(), task.requestURL.c_str());
}

void PixDownloader::onTaskProgress(const DownloadTask& task, int64_t bytesReceived, int64_t totalBytesReceived, int64_t totalBytesExpected)
{
    if (nullptr == _delegate)
    {
        return;
    }
    
    int percent = totalBytesExpected ? int(totalBytesReceived * 100 / totalBytesExpected) : 0;
    _delegate->onProgress(this, task.identifier, percent);
    CCLOG("downloading... %s : %d%%", task.identifier.c_str(), percent);
}

void PixDownloader::onDataTaskSuccess(const DownloadTask& task, std::vector<unsigned char>& data)
{
    if(_write)
    {
        std::vector<std::string> tokens;
        PixStringUtils::getTokens(tokens, task.requestURL, "/");
        
        std::string filename;
        filename.clear();
        
        if(!tokens.empty())
        {
            filename = tokens[tokens.size() - 1];
        }
     
        std::string fullPath =  _storagePath +  filename;
        FILE* fp = fopen(FileUtils::getInstance()->getSuitableFOpen(fullPath).c_str(), "wb");
        
        if (nullptr == fp)
        {
            if (_delegate)
            {
                _delegate->onError(this, task.identifier, 0);
            }
            else
            {
                PixAlert::show("Error", StringUtils::format("Error fp NULL: %s",task.requestURL.c_str()).c_str(), nullptr, ALERT_TYPE_OK);
            }
            
            return;
        }
        
        CCLOG("PixDownloader::onDataTaskSuccess : %s , %d",fullPath.c_str(), (int)data.size());
        
        unsigned char bf;
        
        for (unsigned int i  = 0; i < data.size(); i++)
        {
            bf = data.at(i);
            fwrite(&bf, 1, 1, fp);
        }
        
        fclose(fp);
    }
    
    if(_delegate)
    {
        _delegate->onSuccess(this, task.identifier, data);
    }
}


