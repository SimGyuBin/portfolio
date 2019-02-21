//
//  photoChangeInfo.h
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 17..
//
//

#ifndef photoChangeInfo_h
#define photoChangeInfo_h

#include "cocos2d.h"
#include "../UIControl/PixEditSprite.h"

class photoChangeInfo
{
    
public:
    
    photoChangeInfo()
    {
        editSprite = nullptr;
        unionWithRect = Rect::ZERO;
    }
    
    PixEditSprite *editSprite;
    Rect unionWithRect;
};

void photoChangeInfoSort(std::deque<photoChangeInfo> &array, bool (*compare)(const photoChangeInfo, const photoChangeInfo));
bool compareUnionWithRectHigh(const photoChangeInfo left, const photoChangeInfo right);

#endif /* photoChangeInfo_h */
