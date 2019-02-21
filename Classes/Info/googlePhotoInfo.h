//
//  googlePhotoInfo.h
//  Pzle
//
//  Created by 심규빈 on 2017. 2. 21..
//
//

#ifndef googlePhotoInfo_h
#define googlePhotoInfo_h

#include "cocos2d.h"

class googlePhotoInfo
{

public:
    
    googlePhotoInfo();
    virtual ~googlePhotoInfo();
    
private:
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_PhotoID, PhotoID)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Published, Published)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Updateed, Updateed)
    CC_SYNTHESIZE(int, mn_Width, Width)
    CC_SYNTHESIZE(int, mn_Hight, Hight)
    CC_SYNTHESIZE(int, mn_Size, Size)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Src, Src)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Thumbnail, Thumbnail)
};

#endif /* googlePhotoInfo_h */
