//
//  LocalImageDateInfo.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 21..
//
//

#ifndef LocalImageDateInfo_h
#define LocalImageDateInfo_h

#include "cocos2d.h"
#include "LocalImageInfo.h"

USING_NS_CC;

class LocalImageDateInfo
{
    
public:
    
    LocalImageDateInfo()
    {
        mstr_Date.clear();
        localImageInfoVec.clear();
    }
    
    ~LocalImageDateInfo()
    {
        mstr_Date.clear();
        localImageInfoVec.clear();
    }
    
    void pushLocalImageInfo(LocalImageInfo* info)
    {
        localImageInfoVec.push_back(info);
    }
    
    LocalImageInfo* getLocalImageInfo(int nIndex)
    {
        if (nIndex < 0 || nIndex >= localImageInfoVec.size())
        {
            return nullptr;
        }
        
        return localImageInfoVec.at(nIndex);
    }
    
    LocalImageInfo* getLocalImageInfoByPath(const std::string &strPath)
    {
        for (auto iter = localImageInfoVec.begin(); iter != localImageInfoVec.end(); ++iter)
        {
            if((*iter)->getPath().compare(strPath) == 0)
            {
                return (*iter);
            }
        }
        
        return nullptr;
    }
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Date, Date)
    
    std::vector<LocalImageInfo*> localImageInfoVec;
};

#endif /* LocalImageDateInfo_h */
