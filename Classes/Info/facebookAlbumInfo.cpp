//
//  facebookAlbumInfo.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 2. 23..
//
//

#include "facebookAlbumInfo.h"
#include "facebookImageInfo.h"

facebookAlbumInfo::facebookAlbumInfo()
: mstr_Name("")
, mstr_ID("")
, mn_Count(0)
{
    
}

facebookAlbumInfo::~facebookAlbumInfo()
{
    clear();
}

void facebookAlbumInfo::clear()
{
    for (auto iter = facebookImageVec.begin(); iter != facebookImageVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    facebookImageVec.clear();
}

int facebookAlbumInfo::getFacebookImageCount()
{
    return (int)facebookImageVec.size();
}

void facebookAlbumInfo::pushFacebookImageInfo(facebookImageInfo *pInfo)
{
    facebookImageVec.push_back(pInfo);
}

facebookImageInfo* facebookAlbumInfo::getFacebookImageInfo(int nIndex)
{
    if (nIndex < 0 || nIndex >= facebookImageVec.size())
    {
        return nullptr;
    }
    
    return facebookImageVec.at(nIndex);
}

