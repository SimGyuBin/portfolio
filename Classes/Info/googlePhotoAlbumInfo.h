//
//  googlePhotoAlbumInfo.h
//  Pzle
//
//  Created by 심규빈 on 2017. 2. 17..
//
//

#ifndef googlePhotoAlbumInfo_h
#define googlePhotoAlbumInfo_h

#include "cocos2d.h"

class googlePhotoInfo;

class googlePhotoAlbumInfo
{
    
public :
    
    googlePhotoAlbumInfo();
    virtual ~googlePhotoAlbumInfo();
    
    void clear();
    int getGooglePhotoCount();
    void pushGooglePhotoInfo(googlePhotoInfo *pInfo);
    googlePhotoInfo* getGooglePhotoInfo(int nIndex);
    
    void setIncreasePhotos(int nCount);
    int getIncreasePhotos();
    bool isEnd();
    
private:
    
    int mn_increasePhotos;
    
    std::vector<googlePhotoInfo*> googlePhotoVec;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_AlbumID, AlbumID);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Title, Title);
    CC_SYNTHESIZE(int, mn_Numphotos, Numphotos)
};

#endif /* googlePhotoAlbumInfo_h */
