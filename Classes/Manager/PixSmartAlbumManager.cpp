//
//  PixSmartAlbumManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 3..
//
//

#include "PixSmartAlbumManager.h"
#include "PixMessageManager.h"
#include "../Network/PixHttpClient.h"
#include "../Util/PixNativeUtils.h"
#include "../Util/PixStringUtils.h"
#include "../Model/PixImageModel.h"
#include "../Model/PixSmartMaskPageModel.h"
#include "../Model/PixSmartMaskBookModel.h"
#include "../Info/LocalImageInfo.h"
#include "platform/CCCommon.h"
#include "../Common.h"
#include "../UIControl/PixAlert.h"

#pragma mark - PixImageModel sort

void imageModelSort(std::vector<PixImageModel*> &array, bool (*compare)(const PixImageModel*, const PixImageModel*))
{
    // 선택정렬 알고리즘
    int k;
    PixImageModel *temp;
    
    for (int i = 1; i < array.size(); i++)
    {
        temp = array[i];
        k = i;
        
        while (k > 0 && !((*compare)(array[k - 1], temp)) )
        {
            array[k] = array[k - 1];
            k--;
        }
        array[k] = temp;
    }
}

bool compareImageModelDateHigh(const PixImageModel* left, const PixImageModel* right)
{
    LocalImageInfo* leftInfo = left->getLocalImageInfo();
    LocalImageInfo* rightInfo = right->getLocalImageInfo();
    
    if(leftInfo && rightInfo)
    {
        return atoll(leftInfo->getFullDate().c_str()) >= atoll(rightInfo->getFullDate().c_str());
    }

    return false;
}

bool compareImageModelDateLow(const PixImageModel* left, const PixImageModel* right)
{
    LocalImageInfo* leftInfo = left->getLocalImageInfo();
    LocalImageInfo* rightInfo = right->getLocalImageInfo();
    
    if(leftInfo && rightInfo)
    {
        return atoll(leftInfo->getFullDate().c_str()) <= atoll(rightInfo->getFullDate().c_str());
    }

    return false;
}

#pragma mark - CreateAlbumInfo sort

void createAlbumInfoSort(std::deque<CreateAlbumInfo*> &array, bool (*compare)(const CreateAlbumInfo*, const CreateAlbumInfo*))
{
    // 선택정렬 알고리즘
    int k;
    CreateAlbumInfo *temp;
    
    for (int i = 1; i < array.size(); i++)
    {
        temp = array[i];
        k = i;
        
        while (k > 0 && !((*compare)(array[k - 1], temp)) )
        {
            array[k] = array[k - 1];
            k--;
        }
        array[k] = temp;
    }
}

bool compareCreateAlbumInfoGroupNameHigh(const CreateAlbumInfo* left, const CreateAlbumInfo* right)
{
    return left->getGroupName() >= right->getGroupName();
}

bool compareCreateAlbumInfoGroupNameLow(const CreateAlbumInfo* left, const CreateAlbumInfo* right)
{
    return left->getGroupName() <= right->getGroupName();
}

#pragma mark - createAlbumInfo

CreateAlbumInfo::CreateAlbumInfo()
: mstr_GroupName("")
, mstr_Type("")
, mn_GroupIndex(0)
{
    mstr_Type = "P";
    imageQueue.clear();
}

CreateAlbumInfo::~CreateAlbumInfo()
{
    imageQueue.clear();
}

void CreateAlbumInfo::pushImageModel(PixImageModel *pModel, bool isBack)
{
    if(isBack)
    {
        imageQueue.push_back(pModel);
    }
    else
    {
        imageQueue.push_front(pModel);
    }
}

PixImageModel *CreateAlbumInfo::getImageQueueFront(bool isPop)
{
    if(imageQueue.empty() || imageQueue.size() == 0)
        return nullptr;
    
    PixImageModel *imageModel = imageQueue.front();
    
    if(isPop)
    {
        imageQueue.pop_front();
    }
    
    return imageModel;
}

PixImageModel *CreateAlbumInfo::getImageQueueBack(bool isPop)
{
    if(imageQueue.empty() || imageQueue.size() == 0)
        return nullptr;
    
    PixImageModel *imageModel = imageQueue.back();
    
    if(isPop)
    {
        imageQueue.pop_back();
    }
    
    return imageModel;
}

PixImageModel* CreateAlbumInfo::getImageModel(int nIndex)
{
    if (nIndex < 0 || nIndex >= imageQueue.size())
    {
        return nullptr;
    }
    
    return imageQueue.at(nIndex);
}

int CreateAlbumInfo::getImageModelQueueCount()
{
    return (int)imageQueue.size();
}

#pragma mark - PixSmartAlbumManager

static PixSmartAlbumManager* _sharedObject = nullptr;

PixSmartAlbumManager* PixSmartAlbumManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixSmartAlbumManager();
    }
    
    return _sharedObject;
}

PixSmartAlbumManager::PixSmartAlbumManager()
: themeDictionary(nullptr)
, smartAlbumDictionary(nullptr)
, mstr_LanguageCode(nullptr)
, mstr_Start_date("")
, mstr_End_date("")
, mn_RequestCount(0)
, mn_DownloadCount(0)
, mn_DownloadResourcesCount(0)
{
    createAlbumInfoQueue.clear();
    coverImageModelVec.clear();
    cardSkinInfoVec.clear();
    downloaderQueue.clear();
    
    mstr_LanguageCode = Application::getInstance()->getCurrentLanguageCode();
}

PixSmartAlbumManager::~PixSmartAlbumManager()
{
    clear();
}

void PixSmartAlbumManager::clear()
{
    CC_SAFE_RELEASE_NULL(themeDictionary);
    CC_SAFE_RELEASE_NULL(smartAlbumDictionary);
  
    createAlbumInfoQueue.clear();
    coverImageModelVec.clear();
    
    for (auto iter = cardSkinInfoVec.begin(); iter != cardSkinInfoVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    cardSkinInfoVec.clear();
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        CC_SAFE_RELEASE((*iter));
    }

    downloaderQueue.clear();
    
    Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(PixSmartAlbumManager::downloadResources), this);
    
    mn_RequestCount = 0;
    mn_DownloadCount = 0;
    mn_DownloadResourcesCount = 0;
    
    pageStringVec.clear();
}

void PixSmartAlbumManager::init()
{
    clear();
    
    std::string fullPath = std::string(THEME_PATH).append(THEME_INFO_FILENAME);
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    contentStr = PixStringUtils::replaceAll(contentStr,".","+");
    contentStr = PixStringUtils::replaceAll(contentStr,"-","=");
    contentStr = PixStringUtils::replaceAll(contentStr,"_","/");
    contentStr = PixNativeUtils::nativeCallAES256Decrypt(contentStr);
    
    rapidjson::Document document;
    document.Parse<0>(contentStr.c_str());
    
    if (document.HasParseError() == rapidjson::kParseErrorNone)
    {
        themeDictionary = PixHttpClient::sharedObject()->getDictionary(document);
        themeDictionary->retain();
    }
    
    if(themeDictionary)
    {
        PixArray *rows = themeDictionary->arrayForKey("rows");
        
        if(rows)
        {
            for (int i = 0; i < rows->count(); ++i)
            {
                PixDictionary *root = rows->getDictionaryAtIndex(i);
                
                if(root)
                {
                    downloadThemeResources(root);
                }
            }
        }
        
        PixArray *card_skin = themeDictionary->arrayForKey("card_skin");
        
        if(card_skin)
        {
            for (int i = 0; i < card_skin->count(); ++i)
            {
                PixDictionary *root = card_skin->getDictionaryAtIndex(i);
                
                if(root)
                {
                    CardSkinInfo *cardSkinInfo = new CardSkinInfo();
                    
                    int nCard_skin_id = root->intForKey("card_skin_id");
                    std::string strCard_skin_url = root->stringForKey("card_skin_url");
                    std::string strCard_skin_thumb_url = root->stringForKey("card_skin_thumb_url");
                    
                    cardSkinInfo->setCard_skin_id(nCard_skin_id);
                    cardSkinInfo->setCard_skin_url(strCard_skin_url);
                    cardSkinInfo->setCard_skin_thumb_url(strCard_skin_thumb_url);
                    
                    cardSkinInfoVec.push_back(cardSkinInfo);
                    
                    downloadCardSkinResources(cardSkinInfo);
                }
            }
        }
    }
    
    mn_DownloadResourcesCount = mn_DownloadCount;
    
    if (mn_DownloadResourcesCount > 0)
    {
        Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(PixSmartAlbumManager::downloadResources), this, 0, false);
    }
}
void PixSmartAlbumManager::downloadResources(float dt)
{
    PixDownloader *pixDownloader = nullptr;
    
    if(mn_RequestCount < MAX_REQUEST_COUNT)
    {
        if(downloaderQueue.empty())
        {
            pixDownloader = nullptr;
        }
        else
        {
            pixDownloader = downloaderQueue.front();
            downloaderQueue.pop_front();
        }
        
        if(pixDownloader)
        {
            pixDownloader->createDownloadDataTask(pixDownloader->getDownloadeUrl());
            ++mn_RequestCount;
        }
    }
}

bool PixSmartAlbumManager::isDownloadResources()
{
    if(mn_DownloadCount > 0)
    {
        return true;
    }
    
    return false;
}

int PixSmartAlbumManager::getGroupCount()
{
    if(themeDictionary)
    {
        PixArray *rows = themeDictionary->arrayForKey("rows");
        
        if(rows)
        {
            return (int)rows->count();
        }
    }
    
    return 0;
}

PixDictionary* PixSmartAlbumManager::getGroupDictionary(int nIndex)
{
    if(themeDictionary)
    {
        PixArray *rows = themeDictionary->arrayForKey("rows");
        
        if(rows)
        {
            PixDictionary *group = rows->getDictionaryAtIndex(nIndex);
            
            if(group)
            {
                return group;
            }
        }
    }
    
    return nullptr;
}

PixDictionary* PixSmartAlbumManager::getLeafDictionary(PixDictionary* pDic)
{
    bool isLeaf = false;
    
    if(pDic)
    {
        std::string node = pDic->stringForKey("depth").c_str();

        if(node.compare("leaf") == 0)
        {
            isLeaf = true;
        }
        
        PixArray *children = pDic->arrayForKey("children");
        
        if(children)
        {
            for (int i = 0; i < children->count(); ++i)
            {
                PixDictionary *dic = children->getDictionaryAtIndex(i);
                
                if(dic)
                {
                    if(isLeaf)
                    {
                        return dic;
                    }
                    else
                    {
                        return getLeafDictionary(dic);
                    }
                }
            }
        }
        else
        {
            return nullptr;
        }
    }
    
    return nullptr;
}

PixDictionary* PixSmartAlbumManager::getChildenDictionary(PixDictionary* pDic)
{
    if(pDic)
    {
        PixArray *children = pDic->arrayForKey("children");
        
        if(children)
        {
            for (int i = 0; i < children->count(); ++i)
            {
                PixDictionary *dic = children->getDictionaryAtIndex(i);
                
                if(dic)
                {
                    return dic;
                }
            }
        }
        else
        {
            return nullptr;
        }
    }
    
    return nullptr;
}

PixDictionary* PixSmartAlbumManager::getItemDictionary(PixDictionary* pDic)
{
    if(pDic)
    {
        PixArray *children = pDic->arrayForKey("children");
        
        if(children)
        {
            for (int i = 0; i < children->count(); ++i)
            {
                PixDictionary *dic = children->getDictionaryAtIndex(i);
                
                if(dic)
                {
                    if(dic->arrayForKey("children"))
                    {
                        return getItemDictionary(dic);
                    }
                    else
                    {
                        return dic;
                    }
                }
            }
        }
        else
        {
            return nullptr;
        }
    }
    
    return nullptr;
}

ParamInfo* PixSmartAlbumManager::getSmartAlbumImgParam(std::vector<PixImageModel*> &selectImageModelVec, PixDictionary *pDic)
{
    if(selectImageModelVec.empty())
    {
        return nullptr;
    }
    
    if(pDic == nullptr)
    {
        return nullptr;
    }
    
    paramInfo.clear();
    createAlbumInfoQueue.clear();
    
    /*
     우선은 소수점이 발생할결우 반내림
     앨범 전체 내지 중 사진이 1장인 비율이 최소 20%
     */
    
    int min_page = pDic->intForKey("min_page");             // 최소 페이지 장수(내지)
    int max_page = pDic->intForKey("max_page");             // 최대 페이지 장수(내지)
    int cover_max_pic = pDic->intForKey("cover_max_pic");   // 커버 사진 게수
    int page_max_pic = pDic->intForKey("page_max_pic");     // 내지 사진 최대 개수
    int min_make_page = pDic->intForKey("min_make_page");   // (제작가능한) 앨범 최소 장수 - 표지제외
    int min_make_pic = pDic->intForKey("min_make_pic");     // (제작가능한) 앨범 최소 사진 장수
    
    // 이미지 그룹화 
    imagesGroupingAlgorithm(selectImageModelVec, page_max_pic);

    if(createAlbumInfoQueue.empty())
    {
        return nullptr;
    }
   
//    // 분포도 기준으로 페이지수 셋팅
//    distributionPageAlgorithm(distribution, distribution_page, page_max_pic);
    
//    // 1장인 비율이 최소 20% 셋팅
//    onePicPageAlgorithm(nPageOnePic);
    
    // 최소 페이지 보다 적을 경유
    minPageAlgorithms(min_page);
    
    // 제작 최소 사진수 체크
    if(selectImageModelVec.size() >= min_make_pic)
    {
        // 최소 제작 조건으로 셋팅
        minPageAlgorithms(min_make_page);
    }
   
    // 최대 페이지보다 많을 경우
    maxPageAlgorithms(max_page, page_max_pic);
    
    // 커버 셋팅
    albumCoverCreateAlgorithm(cover_max_pic);
    
    // 토탈 값 대입
    paramInfo.total = StringUtils::format("%d",(int)createAlbumInfoQueue.size());

    for(auto iter = createAlbumInfoQueue.begin(); iter != createAlbumInfoQueue.end(); ++iter)
    {
        int res_count_width = 0;
        int res_count_height = 0;
        
        std::vector<std::string> widthSmartIndexVec;
        std::vector<std::string> heightSmartIndexVec;
        std::vector<std::string> smartIndexVec;
        
        std::string strType = (*iter)->getType();
        
        int nCount = (*iter)->getImageModelQueueCount();
        
        for(int i = 0; i < nCount; ++i)
        {
            PixImageModel *imageModel = (*iter)->getImageModel(i);
            
            if(imageModel)
            {
                LocalImageInfo* localImageInfo = imageModel->getLocalImageInfo();
                
                if(localImageInfo)
                {
                    int nWidth = atoi(localImageInfo->getWidth().c_str());
                    int nHeight = atoi(localImageInfo->getHeight().c_str());
                    
                    int nSmartIndex = imageModel->getSmartIndex();
                    
                    if(strType.compare("C") == 0)
                    {
                        nSmartIndex += COVER_SMART_INDEX;
                        
                        PixImageModel *coverModel = imageModel->clone();
                        coverModel->setSmartIndex(nSmartIndex);
                        coverImageModelVec.push_back(coverModel);
                    }
                    
                    if(nWidth > nHeight)
                    {
                        ++res_count_width;
                        widthSmartIndexVec.push_back(PixStringUtils::int2string(nSmartIndex));
                    }
                    else
                    {
                        ++res_count_height;
                        heightSmartIndexVec.push_back(PixStringUtils::int2string(nSmartIndex));
                    }
                }
            }
        }
        
        paramInfo.img.append(strType).append(",");
        paramInfo.img.append(StringUtils::format("%d",res_count_width)).append(",");
        paramInfo.img.append(StringUtils::format("%d",res_count_height)).append(",");
        
        for(int i = 0; i < widthSmartIndexVec.size(); ++i)
        {
            smartIndexVec.push_back(widthSmartIndexVec.at(i));
        }
        
        for(int i = 0; i < heightSmartIndexVec.size(); ++i)
        {
            smartIndexVec.push_back(heightSmartIndexVec.at(i));
        }
        
        for(int i = 0; i < smartIndexVec.size(); ++i)
        {
            paramInfo.img.append(smartIndexVec.at(i));
            
            if((i + 1) != smartIndexVec.size())
            {
                 paramInfo.img.append("-");
            }
        }
        
        if((iter + 1) != createAlbumInfoQueue.end())
        {
             paramInfo.img.append("^");
        }
        
        CC_SAFE_DELETE((*iter));
    }
    
    createAlbumInfoQueue.clear();

    /*
     이미지 정보 (페이지형태[C:커버/P:내지],가로사진수,세로시진수,사진번호-사진번호[가로사진먼저 나열후 세로사진 나열]^...)
     ex ) C,1,0,1^P,3,2,1-2-4-3-5^P,1,2,6-8-7^....
     */

    return &paramInfo;
}

ParamInfo* PixSmartAlbumManager::getLayoutChangeImgParam(PixSmartMaskPageModel *pModel)
{
    if(pModel == nullptr)
        return nullptr;
    
    paramInfo.clear();
    
    int res_count_width = 0;
    int res_count_height = 0;

    std::vector<std::string> widthSmartIndexVec;
    std::vector<std::string> heightSmartIndexVec;
    std::vector<std::string> smartIndexVec;
    
    int nCount = pModel->getSmartMaskImageModelCount();
    
    for(int i = 0; i < nCount; ++i)
    {
        PixSmartMaskImageModel *smartMaskImageModel = pModel->getSmartMaskImageModel(i);
        
        if(smartMaskImageModel)
        {
            PixSmartImageModel* smartImageModel = smartMaskImageModel->getSmartImageModel();
            
            if(smartImageModel)
            {
                int nWidth = atoi(smartImageModel->getWidth().c_str());
                int nHeight = atoi(smartImageModel->getHeight().c_str());
                
                if(nWidth > nHeight)
                {
                    ++res_count_width;
                    widthSmartIndexVec.push_back(PixStringUtils::int2string(smartImageModel->getSmartIndex()));
                }
                else
                {
                    ++res_count_height;
                    heightSmartIndexVec.push_back(PixStringUtils::int2string(smartImageModel->getSmartIndex()));
                }
            }
        }
    }
    
    paramInfo.img.append(pModel->getlayout_res_code_sub()).append(",");
    paramInfo.img.append(StringUtils::format("%d",res_count_width)).append(",");
    paramInfo.img.append(StringUtils::format("%d",res_count_height)).append(",");

    for(int i = 0; i < widthSmartIndexVec.size(); ++i)
    {
        smartIndexVec.push_back(widthSmartIndexVec.at(i));
    }
    
    for(int i = 0; i < heightSmartIndexVec.size(); ++i)
    {
        smartIndexVec.push_back(heightSmartIndexVec.at(i));
    }
    
    for(int i = 0; i < smartIndexVec.size(); ++i)
    {
        paramInfo.img.append(smartIndexVec.at(i));
        
        if((i + 1) != smartIndexVec.size())
        {
            paramInfo.img.append("-");
        }
    }

    //    이미지 정보 (페이지형태[layout_res_code_sub],가로사진수,세로시진수,사진번호-사진번호[가로사진먼저 나열후 세로사진 나열]^...)
    //
    //    ex ) autolayout_hardcover,1,0,1^autolayout_page,3,2,1-2-4-3-5^autolayout_page,1,2,6-8-7^....
    
    return &paramInfo;
}

std::string PixSmartAlbumManager::getSmartAlbumPhotoParam(std::vector<PixImageModel*> &selectImageModelVec, PixDictionary *pDic)
{
    if(selectImageModelVec.empty())
    {
        return nullptr;
    }
    
    if(pDic == nullptr)
    {
        return nullptr;
    }
    
    PixImageModel *front = selectImageModelVec.front();
    
    if(front)
    {
        LocalImageInfo *info = front->getLocalImageInfo();
        
        if(info)
        {
            mstr_Start_date = info->getFullDate();
        }
    }
    
    PixImageModel *back = selectImageModelVec.back();
    
    if(back)
    {
        LocalImageInfo *info = back->getLocalImageInfo();
        
        if(info)
        {
            mstr_End_date = info->getFullDate();
        }
    }
    
    std::string strPhoto;
    strPhoto.clear();
    
    imageModelSort(selectImageModelVec, compareImageModelDateLow);
    
    //    photo	"사진 정보 (사진번호, 가로세로정보[w/h], 생성일시[20170102134209]^.....) - 내림차순으로전달 (가장 오래된 사진이 1번)
    //
    //    ex ) 1,w,20170102134209^2,h,20170103020413^...."
    
    int nIndex = 0;
    
    for (auto iter = selectImageModelVec.begin(); iter != selectImageModelVec.end(); ++iter)
    {
        (*iter)->setSmartIndex(nIndex);
        
        std::string strNum;
        std::string strSize;
        std::string strDate;
        
        LocalImageInfo* localImageInfo = (*iter)->getLocalImageInfo();
        
        if(localImageInfo)
        {
            int nWidth = atoi(localImageInfo->getWidth().c_str());
            int nHeight = atoi(localImageInfo->getHeight().c_str());
            
            int nSmartIndex = (*iter)->getSmartIndex();
            
            strNum = StringUtils::format("%d",nSmartIndex);
            
            if(nWidth > nHeight)
            {
                strSize = "w";
            }
            else
            {
                strSize = "h";
            }
            
            strDate = localImageInfo->getFullDate();
        }
        
        strPhoto.append(strNum);
        strPhoto.append(",");
        strPhoto.append(strSize);
        strPhoto.append(",");
        strPhoto.append(strDate);
        strPhoto.append("^");
        
        ++nIndex;
    }
    
    if(!strPhoto.empty())
    {
        strPhoto = strPhoto.substr(0, strPhoto.find_last_of("^"));
    }
    
    return strPhoto;
}

ParamInfo* PixSmartAlbumManager::getSmartAlbumThemeChangeParam(PixSmartMaskBookModel *pSmartMaskBookModel, PixDictionary *pDic)
{
    if(pSmartMaskBookModel == nullptr)
        return nullptr;
    
    if(pDic == nullptr)
        return nullptr;
    
    paramInfo.clear();
    
    int nCount = pSmartMaskBookModel->getSmartMaskPageModeCount();
    
    int nTotal = 0;
    
    for (int i = 0; i < nCount; ++i)
    {
        PixSmartMaskPageModel *pModel = pSmartMaskBookModel->getSmartMaskPageModel(i);
        
        if(pModel)
        {
            if(pModel->getType() == ITEM_COVER)
            {
                paramInfo.img.append("C");
                paramInfo.img.append(",");
            }
            else
            {
                paramInfo.img.append("P");
                paramInfo.img.append(",");
            }
            
            int res_count_width = 0;
            int res_count_height = 0;
            
            std::vector<std::string> widthSmartIndexVec;
            std::vector<std::string> heightSmartIndexVec;
            std::vector<std::string> smartIndexVec;
            
            int nImageModelCount = pModel->getSmartMaskImageModelCount();
            
            for(int j = 0; j < nImageModelCount; ++j)
            {
                PixSmartMaskImageModel *pMaskImageModel = pModel->getSmartMaskImageModel(j);
                
                if(pMaskImageModel)
                {
                    PixSmartImageModel* smartImageModel = pMaskImageModel->getSmartImageModel();
                    
                    if(smartImageModel)
                    {
                        int nWidth = atoi(smartImageModel->getWidth().c_str());
                        int nHeight = atoi(smartImageModel->getHeight().c_str());
                        
                        if(nWidth > nHeight)
                        {
                            ++res_count_width;
                            widthSmartIndexVec.push_back(PixStringUtils::int2string(smartImageModel->getSmartIndex()));
                        }
                        else
                        {
                            ++res_count_height;
                            heightSmartIndexVec.push_back(PixStringUtils::int2string(smartImageModel->getSmartIndex()));
                        }
                    }
                }
                
                ++nTotal;
            }
            
            paramInfo.img.append(StringUtils::format("%d",res_count_width)).append(",");
            paramInfo.img.append(StringUtils::format("%d",res_count_height)).append(",");
            
            for(int i = 0; i < widthSmartIndexVec.size(); ++i)
            {
                smartIndexVec.push_back(widthSmartIndexVec.at(i));
            }
            
            for(int i = 0; i < heightSmartIndexVec.size(); ++i)
            {
                smartIndexVec.push_back(heightSmartIndexVec.at(i));
            }
            
            for(int i = 0; i < smartIndexVec.size(); ++i)
            {
                paramInfo.img.append(smartIndexVec.at(i));
                
                if((i + 1) != smartIndexVec.size())
                {
                    paramInfo.img.append("-");
                }
            }

        }
        
        paramInfo.img.append("^");
    }
   
    if(!paramInfo.img.empty())
    {
        paramInfo.img = paramInfo.img.substr(0, paramInfo.img.find_last_of("^"));
    }

    paramInfo.total = StringUtils::format("%d",nTotal);
    
    /*
     이미지 정보 (페이지형태[C:커버/P:내지],가로사진수,세로시진수,사진번호-사진번호[가로사진먼저 나열후 세로사진 나열]^...)
     ex ) C,1,0,1^P,3,2,1-2-4-3-5^P,1,2,6-8-7^....
     */
    
    return &paramInfo;
    
}

void PixSmartAlbumManager::setSmartAlbumDictionary(PixDictionary* pDic)
{
    CC_SAFE_RELEASE(smartAlbumDictionary);
    
    if(pDic)
    {
        smartAlbumDictionary = pDic;
        smartAlbumDictionary->retain();
    }
}

PixDictionary* PixSmartAlbumManager::getSmartAlbumDictionary()
{
    return smartAlbumDictionary;
}

int PixSmartAlbumManager::getCoverImageCount()
{
    return (int)coverImageModelVec.size();
}

PixImageModel* PixSmartAlbumManager::getCoverImageModel(int nIndex)
{
    if (nIndex < 0 || nIndex >= coverImageModelVec.size())
    {
        return nullptr;
    }
    
    return coverImageModelVec.at(nIndex);
}

void PixSmartAlbumManager::coverImageModelVecClear()
{
    coverImageModelVec.clear();
}

int PixSmartAlbumManager::getCardSkinCount()
{
    return (int)cardSkinInfoVec.size();
}

CardSkinInfo* PixSmartAlbumManager::getCardSkinInfo(int nIndex)
{
    if (nIndex < 0 || nIndex >= cardSkinInfoVec.size())
    {
        return nullptr;
    }

    return cardSkinInfoVec.at(nIndex);
}

CardSkinInfo* PixSmartAlbumManager::getCardSkinInfoBySkinId(int nId)
{
    if(!cardSkinInfoVec.empty())
    {
        for (auto iter = cardSkinInfoVec.begin(); iter != cardSkinInfoVec.end(); ++iter)
        {
            if((*iter)->getCard_skin_id() == nId)
            {
                return (*iter);
            }
        }
    }
    
    return nullptr;
}

void PixSmartAlbumManager::createPageString()
{
    std::string strPageString = "";
    
    for (int i = 1; i < (MAX_PAGESTRING_COUNT + 1); ++i)
    {
        strPageString.append(StringUtils::format("%d", i));
        
        if((i % 2) == 0)
        {
            strPageString.append(",");
        }
        else
        {
            strPageString.append(" - ");
        }
    }
    
    if(!strPageString.empty())
    {
        pageStringVec.push_back(CODE_NAME("Edit09"));
        PixStringUtils::getTokens(pageStringVec, strPageString, ",");
    }
}

std::string PixSmartAlbumManager::getPageString(int nIndex)
{
    if(pageStringVec.empty())
    {
        createPageString();
    }
    
    if (nIndex < 0 || nIndex >= pageStringVec.size())
    {
        return "";
    }
    
    return pageStringVec.at(nIndex);
}

std::string PixSmartAlbumManager::getMaxPageString(int num)
{
    int nMaxPage = (num - 1) * 2;
    return StringUtils::format("%d", nMaxPage);
}

#pragma mark - private

CreateAlbumInfo* PixSmartAlbumManager::getCreateAlbumInfo(int nIndex)
{
    if (nIndex < 0 || nIndex >= createAlbumInfoQueue.size())
    {
        return nullptr;
    }
    
    return createAlbumInfoQueue.at(nIndex);
}

CreateAlbumInfo* PixSmartAlbumManager::getCreateAlbumInfoByGroupName(const std::string &groupName)
{
    for (auto iter = createAlbumInfoQueue.begin(); iter != createAlbumInfoQueue.end(); ++iter)
    {
        if((*iter)->getGroupName().compare(groupName) == 0)
        {
            return (*iter);
        }
    }
    
    return nullptr;
}

void PixSmartAlbumManager::reomveCreateAlbumInfo(CreateAlbumInfo *pInfo)
{
    if(pInfo == nullptr)
        return;
 
    for (auto iter = createAlbumInfoQueue.begin(); iter != createAlbumInfoQueue.end(); ++iter)
    {
        if((*iter) == pInfo)
        {
            CC_SAFE_DELETE((*iter));
            createAlbumInfoQueue.erase(iter);
            return;
        }
    }
}

void PixSmartAlbumManager::imagesGroupingAlgorithm(std::vector<PixImageModel*> &imageModelVec, int page_max_pic)
{
    imageModelSort(imageModelVec, compareImageModelDateLow);
    
    PixImageModel *front = imageModelVec.front();
    
    if(front)
    {
        LocalImageInfo *info = front->getLocalImageInfo();
        
        if(info)
        {
            mstr_Start_date = info->getFullDate();
        }
    }
    
    PixImageModel *back = imageModelVec.back();
    
    if(back)
    {
        LocalImageInfo *info = back->getLocalImageInfo();
        
        if(info)
        {
            mstr_End_date = info->getFullDate();
        }
    }
    
    int nCount = (int)imageModelVec.size();
    
    for(int i = 0; i < nCount; ++i)
    {
        PixImageModel *imageModel = imageModelVec.at(i);
        
        if(imageModel)
        {
            imageModel->setSmartIndex(i);
            
            LocalImageInfo *localImageInfo = imageModel->getLocalImageInfo();
            
            if(localImageInfo)
            {
                std::string strGroupName = localImageInfo->getFullDate().substr(0,10);
                
                int nGroupIndex = 0;
                strGroupName.append(StringUtils::format("-%04d",nGroupIndex));
                
                CreateAlbumInfo *createAlbumInfo = getCreateAlbumInfoByGroupName(strGroupName);
                 
                if(createAlbumInfo == nullptr)
                {
                    createAlbumInfo = new CreateAlbumInfo();
                    createAlbumInfo->setGroupName(strGroupName);
                    createAlbumInfo->setGroupIndex(nGroupIndex);
                    createAlbumInfoQueue.push_back(createAlbumInfo);
                }
                else
                {
                    // 내지 사진 최대 개수 초과시 예외 처리
                    int nImageModelQueueCount = createAlbumInfo->getImageModelQueueCount() + 1;
                    
                    if(nImageModelQueueCount > page_max_pic)
                    {
                        int nNewGroupIndex = createAlbumInfo->getGroupIndex() + 1;
                        
                        while (true)
                        {
                            std::string strNewGroupName = localImageInfo->getFullDate().substr(0,10);
                            strNewGroupName.append(StringUtils::format("-%04d",nNewGroupIndex));
                            
                            CreateAlbumInfo *newCreateAlbumInfo = getCreateAlbumInfoByGroupName(strNewGroupName);
                            
                            if(newCreateAlbumInfo)
                            {
                                int nNewImageModelQueueCount = newCreateAlbumInfo->getImageModelQueueCount() + 1;
                                
                                if(nNewImageModelQueueCount > page_max_pic)
                                {
                                    ++nNewGroupIndex;
                                }
                                else
                                {
                                    createAlbumInfo = newCreateAlbumInfo;
                                    break;
                                }
                            }
                            else
                            {
                                newCreateAlbumInfo = new CreateAlbumInfo();
                                newCreateAlbumInfo->setGroupName(strNewGroupName);
                                newCreateAlbumInfo->setGroupIndex(nNewGroupIndex);
                                createAlbumInfoQueue.push_back(newCreateAlbumInfo);
                                createAlbumInfo = newCreateAlbumInfo;
                                break;
                            }
                        }
                    }
                }
                
                createAlbumInfo->pushImageModel(imageModel);
            }
        }
    }
}

void PixSmartAlbumManager::minPageAlgorithms(int min_page)
{
    // 최소 페이지 보다 적을 경유
    if(createAlbumInfoQueue.size() < min_page)
    {
        int nRandom = -1;
        
        while (true)
        {
            int nIndex = RandomHelper::random_int(0, (int)(createAlbumInfoQueue.size() - 1));
            
            if(nIndex == nRandom)
            {
                continue;
            }
            
            nRandom = nIndex;
            
            CreateAlbumInfo *createAlbumInfo = getCreateAlbumInfo(nIndex);
            
            if(createAlbumInfo)
            {
                int nCount = createAlbumInfo->getImageModelQueueCount() / 2;
                
                if(nCount > 0)
                {
                    std::deque<CreateAlbumInfo*>::iterator iter;
                    
                    iter = std::find(createAlbumInfoQueue.begin(), createAlbumInfoQueue.end(), createAlbumInfo);
                    
                    if(iter != createAlbumInfoQueue.end())
                    {
                        int nNewGroupIndex = createAlbumInfo->getGroupIndex() + 1;
                        std::string strNewGroupName = createAlbumInfo->getGroupName().substr(0,10);
                        strNewGroupName.append(StringUtils::format("-%04d",nNewGroupIndex));
                        
                        CreateAlbumInfo *newCreateAlbumInfo = new CreateAlbumInfo();
                        newCreateAlbumInfo->setGroupName(strNewGroupName);
                        newCreateAlbumInfo->setGroupIndex(nNewGroupIndex);
                        createAlbumInfoQueue.insert(iter + 1, newCreateAlbumInfo);
                        
                        for(int i = 0; i < nCount; ++i)
                        {
                            PixImageModel *imageMode = createAlbumInfo->getImageQueueBack(true);
                            newCreateAlbumInfo->pushImageModel(imageMode,false);
                        }
                    }
                }
                else
                {
                    continue;
                }
                
                if(createAlbumInfoQueue.size() >= min_page)
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
}

void PixSmartAlbumManager::maxPageAlgorithms(int max_page, int page_max_pic)
{
    // 최대 페이지보다 많을 경우
    if(createAlbumInfoQueue.size() > max_page)
    {
        int nCurIndex = 0;
        int nNextIndex = 1;
        
        while (true)
        {
            CreateAlbumInfo *pCurInfo = getCreateAlbumInfo(nCurIndex);
            CreateAlbumInfo *pNextInfo = getCreateAlbumInfo(nNextIndex);
        
            if(pCurInfo)
            {
                if(pCurInfo->getImageModelQueueCount() == page_max_pic)
                {
                    ++nCurIndex;
                    ++nNextIndex;
                    continue;
                }
            }
            
            if(pCurInfo && pNextInfo)
            {
                PixImageModel *pImageModel = pNextInfo->getImageQueueFront(true);
                
                if(pImageModel)
                {
                    pCurInfo->pushImageModel(pImageModel);
                }
                else
                {
                    reomveCreateAlbumInfo(pNextInfo);
                }
                
                if(pCurInfo->getImageModelQueueCount() == page_max_pic)
                {
                    ++nCurIndex;
                    ++nNextIndex;
                    continue;
                }
                
                if(createAlbumInfoQueue.size() <= max_page)
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
}

void PixSmartAlbumManager::distributionPageAlgorithm(int distribution, int distribution_page, int page_max_pic)
{
    // 분포도 기준으로 페이지수 셋팅
    if(createAlbumInfoQueue.size() != distribution_page)
    {
        // 분포도 보다 시진 개수 많은 페이지 분류
        // 분포도 보다 사진 개수 적은 페이지 분류
        // 적은 페이지 합침
        // 많은 페이지 나눔
        // 나누고 합칠때 날짜 정렬조건에 맞춰서 나눈다
        
        std::deque<CreateAlbumInfo*> distributionBelowQueue;
        std::deque<CreateAlbumInfo*> distributionOverQueue;
        
        for(auto iter = createAlbumInfoQueue.begin(); iter != createAlbumInfoQueue.end(); ++iter)
        {
            if((*iter)->getImageModelQueueCount() < distribution)
            {
                distributionBelowQueue.push_back((*iter));
            }
            else
            {
                distributionOverQueue.push_back((*iter));
            }
        }
        
        createAlbumInfoQueue.clear();
        
        int i = 0;
        
        while(true)
        {
            if((i + 1) == distributionBelowQueue.size())
            {
                break;
            }
            
            CreateAlbumInfo *curInfo = distributionBelowQueue.at(i);
            
            int j = i;
            while (true)
            {
                if((j + 1) == distributionBelowQueue.size())
                {
                    break;
                }
                
                CreateAlbumInfo *nextInfo = distributionBelowQueue.at(j + i);
                
                PixImageModel *imageModel = nextInfo->getImageQueueFront(true);
                
                if(imageModel)
                {
                    curInfo->pushImageModel(imageModel);
                }
                else
                {
                    std::deque<CreateAlbumInfo*>::iterator iter;
                    iter = std::find(distributionBelowQueue.begin(), distributionBelowQueue.end(), nextInfo);
                    
                    if(iter != distributionBelowQueue.end())
                    {
                        CC_SAFE_DELETE((*iter));
                        distributionBelowQueue.erase(iter);
                    }
                    
                    ++j;
                }
                
                if(curInfo->getImageModelQueueCount() == distribution)
                {
                    break;
                }
            }
            
            ++i;
        }
        
//        int nCouut = (int)distributionBelowQueue.size();
//        
//        for(int i = 0; i < nCouut; ++i)
//        {
//            if((i + 1) == nCouut)
//            {
//                break;
//            }
//            
//            CreateAlbumInfo *curInfo = distributionBelowQueue.at(i);
//            
//            while (distributionBelowQueue.size() ==  distribution)
//            {
//            
//            }
//            
//            
//            
//            
//            
//           
//            CreateAlbumInfo *nextInfo = distributionBelowQueue.at(i + 1);
//
//            if(curInfo->getImageModelQueueCount() == 0)
//            {
//                continue;
//            }
//            
//            int nSum = curInfo->getImageModelQueueCount() + nextInfo->getImageModelQueueCount();
//            
//            if(nSum < distribution)
//            {
//                while (true)
//                {
//                    PixImageModel *imageModel = nextInfo->getImageQueueFront(true);
//                    
//                    if(imageModel)
//                    {
//                        nextInfo->pushImageModel(imageModel,false);
//                    }
//                    else
//                    {
//                        
//                        
//                        
//                        
//                        
//                        
//                        break;
//                    }
//                }
//                
//                nextInfo->setGroupName(curInfo->getGroupName());
//                nextInfo->setGroupIndex(curInfo->getGroupIndex());
//            }
//            else
//            {
//                int nOverCount = nSum - page_max_pic;
//                int nLoop = nextInfo->getImageModelQueueCount() - nOverCount;
//                int j = 0;
//                while (j < nLoop)
//                {
//                    PixImageModel *imageModel = nextInfo->getImageQueueFront(true);
//                    
//                    if(imageModel)
//                    {
//                        nextInfo->pushImageModel(imageModel);
//                    }
//                    else
//                    {
//                        break;
//                    }
//                    
//                    ++j;
//                }
//            }
//        }
        
        for(auto iter = distributionOverQueue.begin(); iter != distributionOverQueue.end(); )
        {
            ++iter;
        }
        
        for(auto iter = distributionBelowQueue.begin(); iter != distributionBelowQueue.end();)
        {
            if((*iter)->getImageModelQueueCount() ==  0)
            {
                CC_SAFE_DELETE((*iter));
                iter = distributionBelowQueue.erase(iter);
            }
            else
            {
                createAlbumInfoQueue.push_back((*iter));
                ++iter;
            }
        }
        
        for(auto iter = distributionOverQueue.begin(); iter != distributionOverQueue.end(); ++iter)
        {
            if((*iter)->getImageModelQueueCount() ==  0)
            {
                CC_SAFE_DELETE((*iter));
                iter = distributionOverQueue.erase(iter);
            }
            else
            {
                createAlbumInfoQueue.push_back((*iter));
                ++iter;
            }
        }
        
        
        createAlbumInfoSort(createAlbumInfoQueue, compareCreateAlbumInfoGroupNameLow);
    }
}

void PixSmartAlbumManager::onePicPageAlgorithm(int nPageOnePic)
{
    // 1장인 비율이 최소 20% 셋팅
    int nOnePicCount = 0;
    
    for (auto iter = createAlbumInfoQueue.begin(); iter != createAlbumInfoQueue.end(); ++iter)
    {
        if((*iter)->getImageModelQueueCount() == 1)
        {
            ++nOnePicCount;
        }
    }
    
    if(nOnePicCount < nPageOnePic)
    {
        
    }
}

void PixSmartAlbumManager::albumCoverCreateAlgorithm(int cover_max_pic)
{
    // 커버 셋팅
    CreateAlbumInfo *coverInfo = new CreateAlbumInfo();
    coverInfo->setType("C");
    
    int nLoop = 0;
    int nRandom = -1;
    
    while (true)
    {
        int nIndex = RandomHelper::random_int(0, (int)(createAlbumInfoQueue.size() - 1));
        
        CreateAlbumInfo *createAlbumInfo = createAlbumInfoQueue.at(nIndex);
        
        if(createAlbumInfo)
        {
            int nImageModelQueueCount = createAlbumInfo->getImageModelQueueCount();
            nIndex = RandomHelper::random_int(0, (int)(nImageModelQueueCount - 1));
            
            if(nIndex == nRandom)
            {
                continue;
            }
            
            nRandom = nIndex;
            
            if(coverInfo->getImageModelQueueCount() > 0)
            {
                PixImageModel *preImageModel = createAlbumInfo->getImageQueueBack();
                PixImageModel *imageModel = createAlbumInfo->getImageModel(nIndex);
                
                if(preImageModel && imageModel)
                {
                    if(preImageModel == imageModel)
                    {
                        continue;
                    }
                    else
                    {
                        coverInfo->pushImageModel(imageModel);
                    }
                }
            }
            else
            {
                PixImageModel *imageModel = createAlbumInfo->getImageModel(nIndex);
                
                if(imageModel)
                {
                    coverInfo->pushImageModel(imageModel);
                }
            }
        }
        
        ++nLoop;
        
        if(nLoop == cover_max_pic)
        {
            break;
        }
    }
    
    createAlbumInfoQueue.push_front(coverInfo);

}

#pragma mark - Downloade

void PixSmartAlbumManager::downloadThemeResources(PixDictionary *dic)
{
    if(dic)
    {
        PixDictionary *imageDic =dic->dictionaryForKey("image_path");
        
        if(imageDic)
        {
            std::string image_path = imageDic->stringForKey(mstr_LanguageCode);
            
            if(image_path.empty())
            {
                image_path = imageDic->stringForKey(DEFAULT_LANGUAGE);
            }
            
            if(!image_path.empty())
            {
                std::vector<std::string> tokens;
                PixStringUtils::getTokens(tokens, image_path, "/");
                
                std::string filename;
                filename.clear();
                
                if(!tokens.empty())
                {
                    filename = tokens[tokens.size() - 1];
                }
                
                std::string fileExtension = FileUtils::getInstance()->getFileExtension(filename);
                
                bool ret = false;
                
                if(fileExtension.find(".png") != std::string::npos)
                {
                    ret = true;
                }
                
                if(fileExtension.find(".jpg") != std::string::npos)
                {
                    ret = true;
                }
                
                if(ret)
                {
                    std::string fullPath = std::string(THEME_PATH).append(filename);
                    bool isFileExist = FileUtils::getInstance()->isFileExist(fullPath);
                    
                    if(!isFileExist)
                    {
                        ++mn_DownloadCount;
                        PixDownloader *pixDownloader = PixDownloader::create(image_path.c_str(), THEME_PATH, this);
                        pixDownloader->setConnectionTimeout(3);
                        
                        downloaderQueue.push_back(pixDownloader);
                    }
                }
            }
            
            PixArray *children = dic->arrayForKey("children");
            
            if(children)
            {
                for (int i = 0; i < children->count(); ++i)
                {
                    PixDictionary *dic = children->getDictionaryAtIndex(i);
                    
                    if(dic)
                    {
                        downloadThemeResources(dic);
                    }
                }
            }
        }    
    }
}

void PixSmartAlbumManager::downloadCardSkinResources(CardSkinInfo *pCardSkinInfo)
{
    if(!pCardSkinInfo->getCard_skin_url().empty())
    {
        std::vector<std::string> tokens;
        PixStringUtils::getTokens(tokens, pCardSkinInfo->getCard_skin_url(), "/");
        
        std::string filename;
        filename.clear();
        
        if(!tokens.empty())
        {
            filename = tokens[tokens.size() - 1];
        }
        
        std::string fileExtension = FileUtils::getInstance()->getFileExtension(filename);
        
        bool ret = false;
        
        if(fileExtension.find(".png") != std::string::npos)
        {
            ret = true;
        }
        
        if(fileExtension.find(".jpg") != std::string::npos)
        {
            ret = true;
        }
        
        if(ret)
        {
            std::string fullPath = std::string(CARDSKIN_PATH).append(filename);
            bool isFileExist = FileUtils::getInstance()->isFileExist(fullPath);
            
            if(!isFileExist)
            {
                ++mn_DownloadCount;
                PixDownloader *pixDownloader = PixDownloader::create(pCardSkinInfo->getCard_skin_url().c_str(), CARDSKIN_PATH, this);
                pixDownloader->setConnectionTimeout(3);
                
                downloaderQueue.push_back(pixDownloader);
            }
        }
    }
    
    if(!pCardSkinInfo->getCard_skin_thumb_url().empty())
    {
        std::vector<std::string> tokens;
        PixStringUtils::getTokens(tokens, pCardSkinInfo->getCard_skin_thumb_url(), "/");
        
        std::string filename;
        filename.clear();
        
        if(!tokens.empty())
        {
            filename = tokens[tokens.size() - 1];
        }
        
        std::string fileExtension = FileUtils::getInstance()->getFileExtension(filename);
        
        bool ret = false;
        
        if(fileExtension.find(".png") != std::string::npos)
        {
            ret = true;
        }
        
        if(fileExtension.find(".jpg") != std::string::npos)
        {
            ret = true;
        }
        
        if(ret)
        {
            std::string fullPath = std::string(CARDSKIN_PATH).append(filename);
            bool isFileExist = FileUtils::getInstance()->isFileExist(fullPath);
            
            if(!isFileExist)
            {
                ++mn_DownloadCount;
                PixDownloader *pixDownloader = PixDownloader::create(pCardSkinInfo->getCard_skin_thumb_url().c_str(), CARDSKIN_PATH, this);
                pixDownloader->setConnectionTimeout(3);
                
                downloaderQueue.push_back(pixDownloader);
            }
        }
    }
}

#pragma mark - Downloade Delegate

void PixSmartAlbumManager::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
{
    downloader->release();
    
    --mn_DownloadCount;
    --mn_RequestCount;
    
    if(downloaderQueue.empty())
    {
        Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(PixSmartAlbumManager::downloadResources), this);
    }
}

void PixSmartAlbumManager::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    
}

void PixSmartAlbumManager::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    downloader->release();
    
    --mn_DownloadCount;
    --mn_RequestCount;
    
    if(downloaderQueue.empty())
    {
        Director::getInstance()->getScheduler()->unschedule(CC_SCHEDULE_SELECTOR(PixSmartAlbumManager::downloadResources), this);
    }
}
