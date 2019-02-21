//
//  PixHttpRequest.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 22..
//
//

#include "PixHttpRequest.h"

PixHttpRequest::PixHttpRequest()
: mbln_Immediate(false)
, target(nullptr)
, callback(nullptr)
, error(nullptr)
{
    
}

PixHttpRequest::~PixHttpRequest()
{
    
}
