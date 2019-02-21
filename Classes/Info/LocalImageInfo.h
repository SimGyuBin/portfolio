//
//  LocalImageInfo.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 21..
//
//

#ifndef LocalImageInfo_h
#define LocalImageInfo_h

#include "cocos2d.h"
#include "CategoryInfo.h"

USING_NS_CC;

class LocalImageInfo
{
    
public :
    
    LocalImageInfo()
    {
        clear();
    }
    
    LocalImageInfo(LocalImageInfo& info)
    {
        clear();
        
        mstr_Path = info.getPath();
        mstr_DateModified = info.getDateModified();
        mstr_DateAdded = info.getDateAdded();
        mstr_FullDate = info.getFullDate();
        mstr_FileSize = info.getFileSize();
        mstr_Width = info.getWidth();
        mstr_Height = info.getHeight();
        mbln_Selected = info.getSelected();
        mn_orientation = info.getOrientation();
        _thumbnailData = info.getThumbnailData();
        _fullScreenData = info.getFullScreenData();
        
        int nCount = info.getCategorCount();
        
        for(int i = 0; i < nCount; ++i)
        {
            CategoryInfo *pInfo = info.getCategoryInfo(i);
            
            if(pInfo)
            {
                setCategoryInfo(pInfo->getCategoryPath(), pInfo->getCategoryName());
            }
        }
    }
    
    ~LocalImageInfo()
    {
        destroyImageData();
        clear();
    }
    
    void clear()
    {
        mstr_Path.clear();
        mstr_DateModified.clear();
        mstr_DateAdded.clear();
        mstr_FullDate.clear();
        mstr_FileSize.clear();
        mstr_Width.clear();
        mstr_Height.clear();
        mbln_Selected = false;
        mn_orientation = 0;
        _thumbnailData = nullptr;
        _fullScreenData = nullptr;
        
        categoryInfoVec.clear();
    }
    
    void destroyImageData()
    {
        CC_SAFE_DELETE(_thumbnailData);
        CC_SAFE_DELETE(_fullScreenData);
    }
    
    void destroyThumbnailImageData()
    {
        CC_SAFE_DELETE(_thumbnailData);
    }
    
    void destroyFullScreenImageData()
    {
        CC_SAFE_DELETE(_fullScreenData);
    }
    
    LocalImageInfo* clone()
    {
        LocalImageInfo *clone = new LocalImageInfo();
        
        clone->mstr_Path = mstr_Path;
        clone->mstr_DateModified = mstr_DateModified;
        clone->mstr_DateAdded = mstr_DateAdded;
        clone->mstr_FullDate = mstr_FullDate;
        clone->mstr_FileSize = mstr_FileSize;
        clone->mstr_Width = mstr_FileSize;
        clone->mstr_Height = mstr_Height;
        clone->mbln_Selected = mbln_Selected;
        clone->mn_orientation = mn_orientation;
        
        if(_thumbnailData && !_thumbnailData->isNull())
        {
            Data *cloneData = new Data();
            cloneData->copy(_thumbnailData->getBytes(), _thumbnailData->getSize());
            clone->_thumbnailData = cloneData;
        }
        
        if(_fullScreenData && !_fullScreenData->isNull())
        {
            Data *cloneData = new Data();
            cloneData->copy(_fullScreenData->getBytes(), _fullScreenData->getSize());
            clone->_fullScreenData = cloneData;
        }
        
        for(auto iter = categoryInfoVec.begin(); iter != categoryInfoVec.end(); ++iter)
        {
            clone->setCategoryInfo((*iter)->getCategoryPath(), (*iter)->getCategoryName());
        }
        
        return clone;
    }

    void setCategoryInfo(const std::string &strCategoryPath, const std::string &strCategoryName)
    {
        CategoryInfo *categoryInfo = new CategoryInfo();
        categoryInfo->setCategoryPath(strCategoryPath);
        categoryInfo->setCategoryName(strCategoryName);
        
        categoryInfoVec.push_back(categoryInfo);
    }
    
    int getCategorCount()
    {
        return (int)categoryInfoVec.size();
    }
    
    CategoryInfo* getCategoryInfo(int nIndex)
    {
        if (nIndex < 0 || nIndex >= categoryInfoVec.size())
        {
            return nullptr;
        }
        
        return categoryInfoVec.at(nIndex);
    }
    
private:
    
    std::vector<CategoryInfo*> categoryInfoVec;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Path, Path)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_DateModified, DateModified)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_DateAdded, DateAdded)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_FullDate, FullDate)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_FileSize, FileSize)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Width, Width)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Height, Height)
    CC_SYNTHESIZE(int, mn_orientation, Orientation);
    CC_SYNTHESIZE(Data*, _thumbnailData, ThumbnailData)
    CC_SYNTHESIZE(Data*, _fullScreenData, FullScreenData)
    CC_SYNTHESIZE(bool, mbln_Selected, Selected)
};

#endif /* LocalImageInfo_h */
