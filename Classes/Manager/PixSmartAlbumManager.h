//
//  PixSmartAlbumManager.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 3..
//
//

#ifndef PixSmartAlbumManager_h
#define PixSmartAlbumManager_h

#include "cocos2d.h"
#include "../Network/PixDownloader.h"
#include "../Info/CardSkinInfo.h"

#define SMARTALBUM_MANAGER      (PixSmartAlbumManager::sharedObject())
#define COVER_SMART_INDEX       10000
#define MAX_REQUEST_COUNT       10
#define MAX_PAGESTRING_COUNT    1000

USING_NS_CC;

class PixDictionary;
class PixImageModel;
class CreateAlbumInfo;
class PixSmartMaskBookModel;
class PixSmartMaskPageModel;


void imageModelSort(std::vector<PixImageModel*> &array, bool (*compare)(const PixImageModel*, const PixImageModel*));
bool compareImageModelDateHigh(const PixImageModel* left, const PixImageModel* right);
bool compareImageModelDateLow(const PixImageModel* left, const PixImageModel* right);

void createAlbumInfoSort(std::deque<CreateAlbumInfo*> &array, bool (*compare)(const CreateAlbumInfo*, const CreateAlbumInfo*));
bool compareCreateAlbumInfoGroupNameHigh(const CreateAlbumInfo* left, const CreateAlbumInfo* right);
bool compareCreateAlbumInfoGroupNameLow(const CreateAlbumInfo* left, const CreateAlbumInfo* right);

class ParamInfo
{

public:

    void clear()
    {
        total.clear();
        img.clear();
    }
    
    std::string total;
    std::string img;
};


class CreateAlbumInfo
{
  
public:
    
    CreateAlbumInfo();
    ~CreateAlbumInfo();

    void pushImageModel(PixImageModel *pModel ,bool isBack = true);
    PixImageModel* getImageQueueFront(bool isPop = false);
    PixImageModel* getImageQueueBack(bool isPop = false);
    PixImageModel* getImageModel(int nIndex);
    int getImageModelQueueCount();
    
private:
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_GroupName, GroupName)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Type, Type)
    CC_SYNTHESIZE(int, mn_GroupIndex, GroupIndex)
    std::deque<PixImageModel*> imageQueue;
    
};

class PixSmartAlbumManager : public Ref, public PixDownloaderDelegate
{
    
public :
    
    static PixSmartAlbumManager* sharedObject();
    
    PixSmartAlbumManager();
    virtual ~PixSmartAlbumManager();
    
    void clear();
    void init();
    void downloadResources(float dt);
    bool isDownloadResources();
    int getGroupCount();
    PixDictionary* getGroupDictionary(int nIndex);
    PixDictionary* getLeafDictionary(PixDictionary* pDic);
    PixDictionary* getChildenDictionary(PixDictionary* pDic);
    PixDictionary* getItemDictionary(PixDictionary* pDic);
    
    ParamInfo* getSmartAlbumImgParam(std::vector<PixImageModel*> &selectImageModelVec, PixDictionary *pDic);
    ParamInfo* getLayoutChangeImgParam(PixSmartMaskPageModel *pModel);
    std::string getSmartAlbumPhotoParam(std::vector<PixImageModel*> &selectImageModelVec, PixDictionary *pDic);
    ParamInfo* getSmartAlbumThemeChangeParam(PixSmartMaskBookModel *pSmartMaskBookModel, PixDictionary *pDic);
    
    void setSmartAlbumDictionary(PixDictionary* pDic);
    PixDictionary* getSmartAlbumDictionary();
    
    int getCoverImageCount();
    PixImageModel* getCoverImageModel(int nIndex);
    void coverImageModelVecClear();
    
    int getCardSkinCount();
    CardSkinInfo* getCardSkinInfo(int nIndex);
    CardSkinInfo* getCardSkinInfoBySkinId(int nId);
    
    void createPageString();
    std::string getPageString(int nIndex);
    std::string getMaxPageString(int num);
    
private:
    
    PixDictionary *themeDictionary;
    PixDictionary *smartAlbumDictionary;
    
    ParamInfo paramInfo;
    std::deque<CreateAlbumInfo*> createAlbumInfoQueue;
    std::vector<PixImageModel*> coverImageModelVec;
    std::vector<CardSkinInfo*> cardSkinInfoVec;
    std::deque<PixDownloader*> downloaderQueue;
    std::vector<std::string> pageStringVec;
    
    int mn_RequestCount;
    
    CC_SYNTHESIZE(const char*, mstr_LanguageCode, LanguageCode)
    CC_SYNTHESIZE(int, mn_DownloadCount, DownloadCount)
    CC_SYNTHESIZE(int, mn_DownloadResourcesCount, DownloadResourcesCount)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Start_date, Start_date)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_End_date, End_date)
    
    CreateAlbumInfo* getCreateAlbumInfo(int nIndex);
    CreateAlbumInfo* getCreateAlbumInfoByGroupName(const std::string &groupName);
    void reomveCreateAlbumInfo(CreateAlbumInfo *pInfo);
    
    void imagesGroupingAlgorithm(std::vector<PixImageModel*> &imageModelVec, int page_max_pic);
    void minPageAlgorithms(int min_page);
    void maxPageAlgorithms(int max_page, int page_max_pic);
    void distributionPageAlgorithm(int distribution, int distribution_page, int page_max_pic);
    void onePicPageAlgorithm(int nPageOnePic);
    void albumCoverCreateAlgorithm(int cover_max_pic);
    
#pragma mark - Downloade
    
    void downloadThemeResources(PixDictionary *dic);
    void downloadCardSkinResources(CardSkinInfo *pCardSkinInfo);
    
#pragma mark - Downloade Delegate
    
    virtual void onError(PixDownloader *downloader, const std::string& identifier, int errorCode);
    virtual void onProgress(PixDownloader *downloader, const std::string& identifier, int percent);
    virtual void onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data);
};

#endif /* PixSmartAlbumManager_h */
