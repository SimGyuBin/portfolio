//
//  PixLoadingSmartalbum.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 6..
//
//

#include "PixLoadingSmartalbum.h"

PixLoading* PixLoadingSmartalbum::create()
{
    PixLoading *loading = new PixLoading();
    
    if(loading && loading->init("00_Common/Loading_Smartalbum_2.csb","00_Common/loading_2.csb"))
    {
        return loading;
    }
    else
    {
        CC_SAFE_DELETE(loading);
        return nullptr;
    }
}
