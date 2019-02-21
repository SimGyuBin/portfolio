//
//  PixTime.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 8..
//
//

#include "PixTime.h"
#include <sys/time.h>

PixTime* pixTime = nullptr;

PixTime* PixTime::sharedObject()
{
    if (pixTime == nullptr)
    {
        pixTime = new PixTime();
    }
    return pixTime;
}

PixTime::PixTime()
{
    mbln_hold = false;
    timegap = 0;
    currenttime = 0;
    gettimeofday(&oldtv, 0);
    updateTime();
}

double PixTime::getCurrentTime()
{
    return currenttime / 1000.0f;
}

void PixTime::updateTime()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    
    timegap = ((tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0) - (oldtv.tv_sec * 1000.0 + oldtv.tv_usec / 1000.0));
    
    currenttime += timegap;
    memcpy (&oldtv, &tv, sizeof(struct timeval));
}

void PixTime::hold()
{
    mbln_hold = true;
}
void PixTime::free()
{
    mbln_hold = false;
    struct timeval tv;
    gettimeofday(&tv, 0);
    memcpy (&oldtv, &tv, sizeof(struct timeval));
}
