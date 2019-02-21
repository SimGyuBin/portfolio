//
//  EventInfo.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 26..
//
//

#ifndef EventInfo_h
#define EventInfo_h

#include "cocos2d.h"

class EventInfo
{
    
public:
    
    EventInfo();
    virtual ~EventInfo();
    
private:
    
    CC_SYNTHESIZE(int, mn_ID, ID)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Banner_path, Banner_path)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Title, Title)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Sdate, Sdate)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Edate, Edate)
    CC_SYNTHESIZE(bool, mbln_End, End)
};


#endif /* EventInfo_h */
