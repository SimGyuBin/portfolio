//
//  PixMyAlbumManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 29..
//
//

#include "PixMyAlbumManager.h"
#include "../Scene/PixMyAlbumLevel.h"
#include "../Scene/PixApp.h"

static PixMyAlbumManager* _sharedObject = nullptr;

PixMyAlbumManager* PixMyAlbumManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixMyAlbumManager();
    }
    
    return _sharedObject;
}

PixMyAlbumManager::PixMyAlbumManager()
{
    mn_Referrer = 0;
    clear();
}

PixMyAlbumManager::~PixMyAlbumManager()
{
    clear();
}

void PixMyAlbumManager::clear()
{
    for(auto iter = myAlbumQueue.begin(); iter != myAlbumQueue.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    myAlbumQueue.clear();
    myAlbumCacheList.clear();
    
    loadImageQueue.clear();
    
    for(auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    
    mn_CacheCount = 0;
    _domain.clear();
    _page = 0;
    _totalCount = 0;
    _AlbumEnd = false;
    
    mn_MaxCacheCount = MAX_ALBUM_CACHE_COUNT;
    mn_CacheCount = 0;
    
    mn_MaxDownloadCount = MAX_ALBUM_LOADIMAGE_COUNT;
    mn_DownloadCount = 0;
    mbln_LoadImageHold = false;
    
    unscheduleLoadImageUpdate();
}

void PixMyAlbumManager::pushMyAlbumModel(MyAlbumModel *pModel, bool isFront)
{
    if(isFront)
    {
        myAlbumQueue.push_front(pModel);
    }
    else
    {
        myAlbumQueue.push_back(pModel);
    }
}

MyAlbumModel* PixMyAlbumManager::getMyAlbumModelBySeq(int nSeqIndex)
{
    if (nSeqIndex < 0 || nSeqIndex >= myAlbumQueue.size())
    {
        return nullptr;
    }
    return myAlbumQueue.at(nSeqIndex);
}

MyAlbumModel* PixMyAlbumManager::getMyAlbumModelByID(int nAlbum_id)
{
    for(auto iter = myAlbumQueue.begin(); iter != myAlbumQueue.end(); ++iter)
    {
        if((*iter)->getAlbum_id() == nAlbum_id)
        {
            return (*iter);
        }
    }
    
    return nullptr;
}

MyAlbumModel* PixMyAlbumManager::getMyAlbumModelByCartID(int nCart_id)
{
    for(auto iter = myAlbumQueue.begin(); iter != myAlbumQueue.end(); ++iter)
    {
        if((*iter)->getCart_id() == nCart_id)
        {
            return (*iter);
        }
    }
    
    return nullptr;
}

bool PixMyAlbumManager::removeMyAlbumModel(MyAlbumModel *pModel)
{
    for(auto iter = myAlbumQueue.begin(); iter != myAlbumQueue.end(); ++iter)
    {
        if((*iter) == pModel)
        {
            CC_SAFE_DELETE((*iter));
            myAlbumQueue.erase(iter);
            return true;
        }
    }
    
    return false;
}

bool PixMyAlbumManager::removeMyAlbumModelByID(int nAlbum_id)
{
    for(auto iter = myAlbumQueue.begin(); iter != myAlbumQueue.end(); ++iter)
    {
        if((*iter)->getAlbum_id() == nAlbum_id)
        {
            CC_SAFE_DELETE((*iter));
            myAlbumQueue.erase(iter);
            return true;
        }
    }
    
    return false;
}

int PixMyAlbumManager::getCount()
{
    return (int)myAlbumQueue.size();
}

void PixMyAlbumManager::increasePageCount()
{
    ++_page;
}

void PixMyAlbumManager::decreasePageCount()
{
    --_page;
}

void PixMyAlbumManager::myAlbumDataParsing(PixDictionary *pDic)
{
    if(pDic)
    {
        PixArray *albums = pDic->arrayForKey("albums");
        
        _AlbumEnd = (pDic->intForKey("is_end") == 0) ? false : true;
        MYALBUM_MANAGER->setTotalCount(pDic->intForKey("total_page"));
        
        if(albums)
        {
            for (int i = 0; i < albums->count(); ++i)
            {
                PixDictionary *album = albums->getDictionaryAtIndex(i);
                
                if(album)
                {
                    MyAlbumModel *myAlbumModel = new MyAlbumModel();
                    
                    int id = album->intForKey("id");
                    std::string title = album->stringForKey("title");
                    std::string contents = album->stringForKey("contents");
                    std::string created_date = album->stringForKey("created_date");
                    std::string start_date = album->stringForKey("startdate");
                    std::string end_date = album->stringForKey("enddate");
                    int card_skin_id = album->intForKey("card_skin_id");
                    std::string card_skin_url = album->stringForKey("card_skin_url");
                    int cart_id = album->intForKey("cart_id");
                    bool make_flag = (album->intForKey("make_flag") == 0) ? false : true;

                    myAlbumModel->setAlbum_id(id);
                    myAlbumModel->setTitle(title);
                    myAlbumModel->setContents(contents);
                    myAlbumModel->setCreated_date(created_date);
                    myAlbumModel->setStart_date(start_date);
                    myAlbumModel->setEnd_date(end_date);
                    myAlbumModel->setCard_skin_id(card_skin_id);
                    myAlbumModel->setCard_skin_url(card_skin_url);
                    myAlbumModel->setCart_id(cart_id);
                    myAlbumModel->setMake_flag(make_flag);
                    
                    PixArray *photos = album->arrayForKey("photos");
                    
                    if(photos)
                    {
                        for (int i = 0; i < photos->count(); ++i)
                        {
                            PixArray *JArray = photos->getArrayAtIndex(0);
                            
                            if(JArray)
                            {
                                for (int j = 0; j < JArray->count(); ++j)
                                {
                                    PixDictionary *JMap = JArray->getDictionaryAtIndex(j);
                                    
                                    if(JMap)
                                    {
                                        PixMyAlbumImageModel *myAlbumImageModel = new PixMyAlbumImageModel();
                                        
                                        std::string id = JMap->stringForKey("id");
                                        //                                std::string path = photo->stringForKey("path");
                                        std::string mid_file_name = JMap->stringForKey("mid_file_name");
                                        int width = JMap->intForKey("width");
                                        int height = JMap->intForKey("height");
                                        
                                        myAlbumImageModel->setId(id);
                                        myAlbumImageModel->setUrl(mid_file_name);
                                        myAlbumImageModel->setWidth(width);
                                        myAlbumImageModel->setHeight(height);
                                        
                                        myAlbumModel->pushMyAlbumImageModel(myAlbumImageModel);
                                    }
                                }
                            }
                        }
                    }
                    
                    MYALBUM_MANAGER->pushMyAlbumModel(myAlbumModel);
                }
            }
        }
    }
}

void PixMyAlbumManager::removeAlbumListViewItem(MyAlbumModel *pModel, bool isAction)
{
    PixMyAlbumLevel *myAlbumLevel = dynamic_cast<PixMyAlbumLevel*>(PIXAPP->curLevel);
    
    if(myAlbumLevel)
    {
        myAlbumLevel->removeAlbumListViewItem(pModel, isAction);
    }
    
    if(_totalCount > 0)
    {
        --_totalCount;
    }
}

void PixMyAlbumManager::skinChange(int nAlbum_id, int nCard_skin_id)
{
    MyAlbumModel *pModel =  getMyAlbumModelByID(nAlbum_id);
    
    if(pModel)
    {
        pModel->setCard_skin_id(nCard_skin_id);
        
        PixMyAlbumLevel *myAlbumLevel = dynamic_cast<PixMyAlbumLevel*>(PIXAPP->curLevel);
        
        if(myAlbumLevel)
        {
            myAlbumLevel->skinChange(pModel);
        }
    }
}

#pragma mark - Cache

void PixMyAlbumManager::addMyAlbumImageData(MyAlbumModel *pModel)
{
    removeMyAlbumImageData();
    myAlbumCacheList.push_back(pModel);
    ++mn_CacheCount;
}

void PixMyAlbumManager::removeMyAlbumImageData(MyAlbumModel *pModel)
{
    if(getCacheCount() > getMaxCacheCount())
    {
        removeMyAlbumCacheData(pModel);
    }
}

void PixMyAlbumManager::removeMyAlbumCacheData(MyAlbumModel *pModel)
{
    if(pModel == nullptr)
        return;
    
    for (auto iter = myAlbumCacheList.begin(); iter != myAlbumCacheList.end(); ++iter)
    {
        if((*iter) == pModel)
        {
            (*iter)->removeImageData();
            myAlbumCacheList.erase(iter);
            --mn_CacheCount;
            break;
        }
    }
}

void PixMyAlbumManager::removeMyAlbumImageData()
{
    for(auto iter = myAlbumCacheList.begin(); iter != myAlbumCacheList.end(); )
    {
        if(getCacheCount() > getMaxCacheCount())
        {
            if((*iter)->isVisible() == false)
            {
                (*iter)->removeImageData();
                myAlbumCacheList.erase(iter);
                --mn_CacheCount;
                break;
            }
            else
            {
                ++iter;
            }
        }
        else
        {
            break;
        }
    }
}

bool PixMyAlbumManager::isRemoveMyAlbumImageData(MyAlbumModel *pModel)
{
    if(getCacheCount() > getMaxCacheCount())
    {
        removeMyAlbumImageData(pModel);
        return true;
    }
    
    return false;
}

#pragma mark - download image

void PixMyAlbumManager::scheduleLoadImageUpdate()
{
    Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(PixMyAlbumManager::loadImage), this, 0, false);
}

void PixMyAlbumManager::unscheduleLoadImageUpdate()
{
    Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(PixMyAlbumManager::loadImage), this);
}

void PixMyAlbumManager::loadImage(float dt)
{
    if(mbln_LoadImageHold)
        return;
    
    while (true)
    {
        if(!loadImageQueue.empty())
        {
            if(mn_DownloadCount < mn_MaxDownloadCount)
            {
                PixMyAlbumImageModel *pModel = loadImageQueue.front();
                std::string url = pModel->getUrl();
                sendGetImageUrl(url, pModel);
                loadImageQueue.pop_front();
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}

void PixMyAlbumManager::loadMyAlbumImage(PixMyAlbumImageModel *pModel)
{
    if(pModel)
    {
        loadImageQueue.push_back(pModel);
    }
}

void PixMyAlbumManager::sendGetImageUrl(const std::string &url, PixMyAlbumImageModel *pModel)
{
    PixDownloader *pixDownloader = PixDownloader::create();
    pixDownloader->setDelegate(pModel->getAlbumCoverCell());
    pixDownloader->setWrite(false);
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->setDownloadeUrl(url.c_str());
    pixDownloader->setUserData(pModel);
    pixDownloader->createDownloadDataTask("AlbumCoverImage");
    
    ++mn_DownloadCount;
    
    downloaderQueue.push_back(pixDownloader);
}

bool PixMyAlbumManager::loadMyAlbumImageCompleted(PixDownloader *downloader)
{
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        if((*iter) == downloader)
        {
            (*iter)->release();
            downloaderQueue.erase(iter);
            --mn_DownloadCount;
            return true;
        }
    }
    
    return false;
}
