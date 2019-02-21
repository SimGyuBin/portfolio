//
//  NoticeInfo.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 21..
//
//

#ifndef NoticeInfo_h
#define NoticeInfo_h

#include "cocos2d.h"

class NoticeInfo
{
public:
    
    NoticeInfo()
    {
        mn_ID = 0;
        mstr_Title.clear();
        mn_Kind = 0;
        mstr_Content.clear();
        mstr_File.clear();
        mn_Hit = 0;
        mstr_Date.clear();
    }
    
    ~NoticeInfo() {}
    
    CC_SYNTHESIZE(int, mn_ID, ID)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Title, Title)
    CC_SYNTHESIZE(int, mn_Kind, Kind)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Content, Content)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_File, File)
    CC_SYNTHESIZE(int, mn_Hit, Hit)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Date, Date)
};

#endif /* NoticeInfo_h */
