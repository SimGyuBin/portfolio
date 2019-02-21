//
//  BoardBastInfo.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 26..
//
//

#ifndef BoardBastInfo_h
#define BoardBastInfo_h

#include "cocos2d.h"

class BoardBastInfo
{
    
public:
    
    BoardBastInfo();
    virtual ~BoardBastInfo();
    
private:
    
    CC_SYNTHESIZE(int, mn_ID, ID)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Title, Title)
    CC_SYNTHESIZE(int, mn_Board_type, Board_type)
};

#endif /* BoardBastInfo_h */
