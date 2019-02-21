//
//  facebookImageInfo.h
//  Pzle
//
//  Created by 심규빈 on 2017. 2. 27..
//
//

#ifndef facebookImageInfo_h
#define facebookImageInfo_h

#include "cocos2d.h"

class facebookImageInfo
{
  
public:
    
    facebookImageInfo();
    virtual ~facebookImageInfo();
    
private:
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_ID, ID)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Name, Name)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Updated_time, Updated_time)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Source, Source)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Picture, Picture)
    CC_SYNTHESIZE(int, mn_Width, Width)
    CC_SYNTHESIZE(int, mn_Height, Height)
};

#endif /* facebookImageInfo_h */
