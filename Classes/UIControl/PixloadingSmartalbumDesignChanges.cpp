//
//  PixloadingSmartalbumDesignChanges.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 22..
//
//

#include "PixloadingSmartalbumDesignChanges.h"

PixLoading* PixloadingSmartalbumDesignChanges::create()
{
    PixLoading *loading = new PixLoading();
    
    if(loading && loading->init("00_Common/Loading_Smartalbum.csb","00_Common/loading_2.csb"))
    {
        return loading;
    }
    else
    {
        CC_SAFE_DELETE(loading);
        return nullptr;
    }
}
