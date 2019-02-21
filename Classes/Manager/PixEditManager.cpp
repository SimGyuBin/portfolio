//
//  PixEditManager.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 26..
//
//

#include "PixEditManager.h"
#include "../UIControl/PixEditSprite.h"
#include "../Model/PixSmartMaskImageModel.h"
#include "../Scene/PixApp.h"

void smartImageSort(std::vector<PixSmartImageModel*> &array, bool (*compare)(const PixSmartImageModel*, const PixSmartImageModel*))
{
    // 선택정렬 알고리즘
    int k;
    PixSmartImageModel *temp;
    
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

bool compareSmartImageDateHigh(const PixSmartImageModel* left, const PixSmartImageModel* right)
{
    return atoll(left->getDate().c_str()) >= atoll(right->getDate().c_str());
}

bool compareSmartImageDateLow(const PixSmartImageModel* left, const PixSmartImageModel* right)
{
    return atoll(left->getDate().c_str()) <= atoll(right->getDate().c_str());;
}

static PixEditManager* _sharedObject = nullptr;

PixEditManager* PixEditManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixEditManager();
        _sharedObject->clear();
    }
    
    return _sharedObject;
}

PixEditManager::PixEditManager()
{
    photoUnusedVec.clear();
    photoUsedVec.clear();
}

PixEditManager::~PixEditManager()
{
    clear();
}

void PixEditManager::clear()
{
    photoUsedVecClear();
    photoUnusedVecClear();
    smartImageModelTempClear();
}

void PixEditManager::photoUsedVecClear()
{
    for(auto iter = photoUsedVec.begin(); iter != photoUsedVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    photoUsedVec.clear();
}

void PixEditManager::photoUnusedVecClear()
{
    for(auto iter = photoUnusedVec.begin(); iter != photoUnusedVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    photoUnusedVec.clear();
}

void PixEditManager::pushPhotoUnused(PhotoEditInfo *pInfo)
{
    photoUnusedVec.push_back(pInfo);
}

PhotoEditInfo* PixEditManager::getPhotoUnused(int nIndex)
{
    if (nIndex < 0 || nIndex >= photoUnusedVec.size())
    {
        return nullptr;
    }
    
    return photoUnusedVec.at(nIndex);
}

bool PixEditManager::reomvePhotoUnused(PhotoEditInfo *pInfo)
{
    for(auto iter = photoUnusedVec.begin(); iter != photoUnusedVec.end(); ++iter)
    {
        if((*iter) == pInfo)
        {
            CC_SAFE_DELETE((*iter));
            photoUnusedVec.erase(iter);
            return true;
        }
    }
    
    return false;
}

int PixEditManager::getPhotoUnusedCount()
{
    return (int)photoUnusedVec.size();
}

void PixEditManager::pushPhotoUsed(PhotoEditInfo *pInfo)
{
    photoUsedVec.push_back(pInfo);
}

PhotoEditInfo* PixEditManager::getPhotoUsed(int nIndex)
{
    if (nIndex < 0 || nIndex >= photoUsedVec.size())
    {
        return nullptr;
    }
    
    return photoUsedVec.at(nIndex);
}

bool PixEditManager::reomvePhotoUsed(PhotoEditInfo *pInfo)
{
    for(auto iter = photoUsedVec.begin(); iter != photoUsedVec.end(); ++iter)
    {
        if((*iter) == pInfo)
        {
            CC_SAFE_DELETE((*iter));
            photoUsedVec.erase(iter);
            return true;
        }
    }
    
    return false;
}

int PixEditManager::getPhotoUsedCount()
{
    return (int)photoUsedVec.size();
}


void PixEditManager::pushSmartImageModelTemp(PixSmartImageModel *pImageModel)
{
    smartImageModelTempVec.push_back(pImageModel);
}

PixSmartImageModel* PixEditManager::getSmartImageModelTemp(bool isWidth)
{
    for (auto iter = smartImageModelTempVec.begin(); iter != smartImageModelTempVec.end(); ++iter)
    {
        int nLocalImageWidth = atoi((*iter)->getWidth().c_str());
        int nLocalImageHeight = atoi((*iter)->getHeight().c_str());
        
        // 가로 레이아웃
        if(nLocalImageWidth > nLocalImageHeight)
        {
            if(isWidth)
            {
                PixSmartImageModel *pModel = (*iter);
                smartImageModelTempVec.erase(iter);
                return pModel;
            }
        }
        // 세로 레리아웃
        else
        {
            if(!isWidth)
            {
                PixSmartImageModel *pModel = (*iter);
                smartImageModelTempVec.erase(iter);
                return pModel;
            }
        }        
    }
    
    return nullptr;
}

void PixEditManager::smartImageModelTempClear()
{
    
    for (auto iter = smartImageModelTempVec.begin(); iter != smartImageModelTempVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    smartImageModelTempVec.clear();

}

void PixEditManager::sortSmartImageModelTempDateHigh()
{
    smartImageSort(smartImageModelTempVec, compareSmartImageDateHigh);
}

void PixEditManager::sortSmartImageModelTempLow()
{
    smartImageSort(smartImageModelTempVec, compareSmartImageDateLow);
}