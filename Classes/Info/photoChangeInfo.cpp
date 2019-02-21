//
//  photoChangeInfo.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 17..
//
//

#include "photoChangeInfo.h"

void photoChangeInfoSort(std::deque<photoChangeInfo> &array, bool (*compare)(const photoChangeInfo, const photoChangeInfo))
{
    // 선택정렬 알고리즘
    int k;
    photoChangeInfo temp;
    
    for (int i = 1; i < array.size(); i++)
    {
        temp = array[i];
        k = i;
        
        while (k > 0 && !((*compare)(array[k - 1], temp)) )
        {
            array[k] = array[k - 1];
            k--;
        }
        array[k] = temp;
    }
}

bool compareUnionWithRectHigh(const photoChangeInfo left, const photoChangeInfo right)
{
    double leftPixel = left.unionWithRect.size.width * left.unionWithRect.size.height;
    double rightPixel = right.unionWithRect.size.width * right.unionWithRect.size.height;
    
    return leftPixel <= rightPixel;
}

