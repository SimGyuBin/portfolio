//
//  PixLoadingHorizontal.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 3..
//
//

#include "PixLoadingHorizontal.h"

PixLoading* PixLoadingHorizontal::create()
{  
    PixLoading *loading = new PixLoading();
    
    if(loading && loading->init("00_Common/Loading_Horizontal.csb", "00_Common/loading.csb"))
    {
        return loading;
    }
    else
    {
        CC_SAFE_DELETE(loading);
        return nullptr;
    }
}
