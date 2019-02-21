//
//  googlePhotoAlbumInfo.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 2. 17..
//
//

#include "googlePhotoAlbumInfo.h"
#include "googlePhotoInfo.h"

googlePhotoAlbumInfo::googlePhotoAlbumInfo()
: mstr_AlbumID("")
, mstr_Title("")
, mn_Numphotos(0)
{
    
}

googlePhotoAlbumInfo::~googlePhotoAlbumInfo()
{
    clear();
}

void googlePhotoAlbumInfo::clear()
{
    for (auto iter = googlePhotoVec.begin(); iter != googlePhotoVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    googlePhotoVec.clear();
    
    mn_increasePhotos = 0;
}

int googlePhotoAlbumInfo::getGooglePhotoCount()
{
    return (int)googlePhotoVec.size();
}

void googlePhotoAlbumInfo::pushGooglePhotoInfo(googlePhotoInfo *pInfo)
{
    googlePhotoVec.push_back(pInfo);
}

googlePhotoInfo* googlePhotoAlbumInfo::getGooglePhotoInfo(int nIndex)
{
    if (nIndex < 0 || nIndex >= googlePhotoVec.size())
    {
        return nullptr;
    }
    
    return googlePhotoVec.at(nIndex);
}

void googlePhotoAlbumInfo::setIncreasePhotos(int nCount)
{
    mn_increasePhotos = mn_increasePhotos + nCount;
}

int googlePhotoAlbumInfo::getIncreasePhotos()
{
    return mn_increasePhotos;
}

bool googlePhotoAlbumInfo::isEnd()
{
    if(getIncreasePhotos() == getNumphotos())
    {
        return true;
    }
    
    return false;
}
