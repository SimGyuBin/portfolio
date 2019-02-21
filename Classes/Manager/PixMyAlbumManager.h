//
//  PixMyAlbumManager.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 29..
//
//

#ifndef PixMyAlbumManager_h
#define PixMyAlbumManager_h

#include "cocos2d.h"
#include "../Model/MyAlbumModel.h"

#define MYALBUM_MANAGER     (PixMyAlbumManager::sharedObject())

USING_NS_CC;

class PixDictionary;

class CC_DLL PixMyAlbumManager : public Ref
{
public :
    
    static PixMyAlbumManager* sharedObject();
    
    PixMyAlbumManager();
    virtual ~PixMyAlbumManager();
    
    void clear();
    
    void pushMyAlbumModel(MyAlbumModel *pModel, bool isFront = false);
    MyAlbumModel* getMyAlbumModelBySeq(int nSeqIndex);
    MyAlbumModel* getMyAlbumModelByID(int nAlbum_id);
    MyAlbumModel* getMyAlbumModelByCartID(int nCart_id);
    bool removeMyAlbumModel(MyAlbumModel *pModel);
    bool removeMyAlbumModelByID(int nAlbum_id);
    int getCount();
    
    void increasePageCount();
    void decreasePageCount();
    void myAlbumDataParsing(PixDictionary *pDic);
    void removeAlbumListViewItem(MyAlbumModel *pModelm, bool isAction = true);
    void skinChange(int nAlbum_id, int nCard_skin_id);
    
#pragma mark - Cache
    
    void addMyAlbumImageData(MyAlbumModel *pModel);
    void removeMyAlbumImageData(MyAlbumModel *pModel);
    void removeMyAlbumCacheData(MyAlbumModel *pModel);
    void removeMyAlbumImageData();
    bool isRemoveMyAlbumImageData(MyAlbumModel *pModel);
    
#pragma mark - download image
    
    void scheduleLoadImageUpdate();
    void unscheduleLoadImageUpdate();
    void loadImage(float dt);
    
    void loadMyAlbumImage(PixMyAlbumImageModel *pModel);
    void sendGetImageUrl(const std::string &url, PixMyAlbumImageModel *pModel);
    bool loadMyAlbumImageCompleted(PixDownloader *downloader);

private:
    
    int mn_MaxDownloadCount;
    int mn_DownloadCount;
    
    std::deque<MyAlbumModel*> myAlbumQueue;
    std::list<MyAlbumModel*> myAlbumCacheList;
    
    std::deque<PixMyAlbumImageModel*> loadImageQueue;
    std::deque<PixDownloader*> downloaderQueue;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _domain , Domain)
    CC_SYNTHESIZE(int, _page, Page)
    CC_SYNTHESIZE(int, _totalCount, TotalCount)
    CC_SYNTHESIZE(bool, _AlbumEnd, AlbumEnd)
    CC_SYNTHESIZE(int, mn_MaxCacheCount, MaxCacheCount)
    CC_SYNTHESIZE(int, mn_CacheCount, CacheCount)
    CC_SYNTHESIZE(int, mn_Referrer, Referrer)
    CC_SYNTHESIZE(bool, mbln_LoadImageHold, LoadImageHold)
    
};


#endif /* PixMyAlbumManager_h */
