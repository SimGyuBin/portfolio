//
//  UserInfo.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 25..
//
//

#include "UserInfo.h"

void localImageSort(std::vector<LocalImageInfo*> &array, bool (*compare)(const LocalImageInfo*, const LocalImageInfo*))
{
    // 선택정렬 알고리즘
    int k;
    LocalImageInfo *temp;
    
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

bool comparelocalImageDateHigh(const LocalImageInfo* left, const LocalImageInfo* right)
{
    return atoll(left->getFullDate().c_str()) >= atoll(right->getFullDate().c_str());
}

bool comparelocalImageDateLow(const LocalImageInfo* left, const LocalImageInfo* right)
{
    return atoll(left->getFullDate().c_str()) <= atoll(right->getFullDate().c_str());
}

UserInfo::UserInfo()
: mstr_Member_id("")
, strUserID("")
, name("")
, vendor("")
, mstr_smartRenderingID("")
, phoneNumber("")
, email("")
, mstr_Grade("0")
, me_UserType(USER_NONE)
, mbln_Alarm(false)
, mSelectThemeInfo(nullptr)
, mSelectThemeChangeInfo(nullptr)
, mn_CartCount(0)
, mbln_NoticeNew(false)
, mbln_EventNew(false)
, mbln_ShowMembershipEventLayer(false)
, mbln_ShowMyAlbumEvent(false)
{
    phoneNumber = PixUserDefault::GetPhone();
    strUserID = PixUserDefault::GetUserID();
    me_UserType = PIX_USER_TYPE(PixUserDefault::GetUserType());
    
    ValueMap valueMap = FileUtils::getInstance()->getValueMapFromFile("vendor.plist");
    vendor = valueMap.at("vendor").asString().c_str();
    
    imageModels.clear();
    localImageInfoVec.clear();
    smartMaskBookModelVec.clear();
    
    mbln_Alarm = PixUserDefault::GetAlarm();
}

UserInfo::~UserInfo()
{
    removeAllImageModel();
    localImageInfoVecClear();
}

void UserInfo::removeAllImageModel()
{
    for(auto iter = imageModels.begin(); iter != imageModels.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter).second);
    }
    
    imageModels.clear();
}

void UserInfo::localImageInfoVecClear()
{
    for(auto iter = localImageInfoVec.begin(); iter != localImageInfoVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    localImageInfoVec.clear();
}

void UserInfo::smartMaskBookModelVecClear()
{
    for(auto iter = smartMaskBookModelVec.begin(); iter != smartMaskBookModelVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    smartMaskBookModelVec.clear();
}

int UserInfo::getImageModelCount()
{
    return (int)imageModels.size();
}

void UserInfo::addImageModel(PixImageModel *pImageModel)
{
    if(pImageModel)
    {
        auto ret = imageModels.insert(std::make_pair(pImageModel->getSmartIndex(), pImageModel));
        
        if (ret.second == false)
        {
            CC_SAFE_DELETE((ret.first->second));
            imageModels[pImageModel->getSmartIndex()] = pImageModel;
        }
    }
}

PixImageModel* UserInfo::getImageModelBySeq(int nSeqIndex)
{
    if ((imageModels.size() - 1) < nSeqIndex)
        return nullptr;
    
    std::unordered_map<int, PixImageModel*>::iterator it;
    it = imageModels.begin();
    for (int i = 0; i < nSeqIndex; ++i)
    {
        it++;
    }
    return (*it).second;
}

bool UserInfo::removeImageModel(PixImageModel *pImageModel)
{
    if(pImageModel)
    {
        int nKey = pImageModel->getSmartIndex();
        auto it = imageModels.find(nKey);
        
        if (it != imageModels.end())
        {
            CC_SAFE_DELETE((*it).second);
            imageModels.erase(it);
            return true;
        }
    }
    
    return false;
}

PixImageModel* UserInfo::getImageModelBySmartIndex(int nSmartIndex)
{
    int nKey = nSmartIndex;
    auto it = imageModels.find(nKey);
    
    if (it != imageModels.end())
    {
        return (*it).second;
    }
    
    return nullptr;
}

void UserInfo::localImageDataDelete()
{
    for (auto iter = imageModels.begin(); iter != imageModels.end(); ++iter)
    {
        LocalImageInfo *localImageInfo = ((*iter).second)->getLocalImageInfo();
        
        if(localImageInfo)
        {
            localImageInfo->destroyImageData();
        }
    }
}

std::vector<LocalImageInfo*>& UserInfo::getlocalImageInfoVec()
{
    return localImageInfoVec;
}

int UserInfo::getLocalImageCount()
{
    return (int)localImageInfoVec.size();
}

void UserInfo::pushLocalImageInfo(LocalImageInfo *info)
{
    localImageInfoVec.push_back(info);
}

LocalImageInfo* UserInfo::getLocalImageInfo(int nIndex)
{
    if (nIndex < 0 || nIndex >= localImageInfoVec.size())
    {
        return nullptr;
    }
    
    return localImageInfoVec.at(nIndex);
}

LocalImageInfo* UserInfo::getLocalImageInfoByPath(const std::string &path)
{
    for(auto iter = localImageInfoVec.begin(); iter != localImageInfoVec.end(); ++iter)
    {
        if((*iter)->getPath().compare(path) == 0)
        {
            return (*iter);
        }
    }
    
    return nullptr;
}

int UserInfo::getSmartMaskBookModelCount()
{
    return (int)smartMaskBookModelVec.size();
}

void UserInfo::pushSmartMaskBookModel(PixSmartMaskBookModel *model)
{
    smartMaskBookModelVec.push_back(model);
}

PixSmartMaskBookModel* UserInfo::getSmartMaskBookModel(int nIndex)
{
    if (nIndex < 0 || nIndex >= smartMaskBookModelVec.size())
    {
        return nullptr;
    }
    
    return smartMaskBookModelVec.at(nIndex);
}

void UserInfo::destroyLocalImageData()
{
    for(auto iter = localImageInfoVec.begin(); iter != localImageInfoVec.end(); ++iter)
    {
        (*iter)->destroyImageData();
    }
}

void UserInfo::destroyThumbnailImageData()
{
    for(auto iter = localImageInfoVec.begin(); iter != localImageInfoVec.end(); ++iter)
    {
        (*iter)->destroyThumbnailImageData();
    }
}

void UserInfo::sortLocalImageDateHigh()
{
    localImageSort(localImageInfoVec, comparelocalImageDateHigh);
}

void UserInfo::sortLocalImageDatelow()
{
    localImageSort(localImageInfoVec, comparelocalImageDateLow);
}

void UserInfo::localImageVecReverse()
{
    std::reverse(localImageInfoVec.begin(),localImageInfoVec.end());
}

void UserInfo::addThumbnailData(const std::string &strPath, Data *pData, const std::string &strWidth, const std::string &strHeight)
{
    for (auto iter = localImageInfoVec.begin(); iter != localImageInfoVec.end(); ++iter)
    {
        if((*iter)->getPath().compare(strPath) == 0)
        {
            Data *thumbnailData = (*iter)->getThumbnailData();
            
            if(thumbnailData != nullptr)
            {
                CC_SAFE_DELETE(thumbnailData);
            }
                        
            (*iter)->setThumbnailData(pData);
//            (*iter)->setWidth(strWidth);
//            (*iter)->setHeight(strHeight);
            break;
        }
    }
}

void UserInfo::addFullScreenData(const std::string &strPath, Data *pData, const std::string &strWidth, const std::string &strHeight)
{
    for (auto iter = localImageInfoVec.begin(); iter != localImageInfoVec.end(); ++iter)
    {
        if((*iter)->getPath().compare(strPath) == 0)
        {            
            Data *originalData = (*iter)->getFullScreenData();
            
            if(originalData != nullptr)
            {
                CC_SAFE_DELETE(originalData);
            }
            
            (*iter)->setFullScreenData(pData);
//            (*iter)->setWidth(strWidth);
//            (*iter)->setHeight(strHeight);
            return;
        }
    }
}

void UserInfo::setLocalImageDeselect()
{
    for (auto iter = localImageInfoVec.begin(); iter != localImageInfoVec.end(); ++iter)
    {
        (*iter)->setSelected(false);
    }
}

int UserInfo::getLocalImageSelectImageCount()
{
    int nCount = 0;
    
    for (auto iter = localImageInfoVec.begin(); iter != localImageInfoVec.end(); ++iter)
    {
    
        if((*iter)->getSelected() == true)
        {
            ++nCount;
        }
    }
    
    return nCount;
}
