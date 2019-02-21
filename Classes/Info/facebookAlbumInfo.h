//
//  facebookAlbumInfo.h
//  Pzle
//
//  Created by 심규빈 on 2017. 2. 23..
//
//

#ifndef facebookAlbumInfo_h
#define facebookAlbumInfo_h

#include "cocos2d.h"

class facebookImageInfo;

class facebookAlbumInfo
{
    
public :

    facebookAlbumInfo();
    virtual ~facebookAlbumInfo();
    
    void clear();
    int getFacebookImageCount();
    void pushFacebookImageInfo(facebookImageInfo *pInfo);
    facebookImageInfo* getFacebookImageInfo(int nIndex);
    
private :
  
    std::vector<facebookImageInfo*> facebookImageVec;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Name, Name)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_ID, ID)
    CC_SYNTHESIZE(int, mn_Count, Count);
};

#endif /* facebookAlbumInfo_h */
