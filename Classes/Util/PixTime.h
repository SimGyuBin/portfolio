//
//  PixTime.hpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 8..
//
//

#ifndef PixTime_h
#define PixTime_h

#include "cocos2d.h"

USING_NS_CC;

class PixTime
{
private:
    
    double currenttime;
    double timegap;
    struct timeval oldtv;
    
    double nPauseTime;
    bool mbln_hold;
public:
    static PixTime* sharedObject();
    
    PixTime();
    
    double getCurrentTime();
    void updateTime();
    
    void hold();
    void free();
  
};

#endif /* PixTime_h */
