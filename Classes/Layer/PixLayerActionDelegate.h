//
//  PixLayerActionDelegate.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#ifndef PixLayerActionDelegate_h
#define PixLayerActionDelegate_h

#include "cocos2d.h"

USING_NS_CC;

class PixLayerActionDelegate
{
public:
    
    virtual void showLayerActionStart(Ref *pTarget) {};
    virtual void showLayerActionFinish(Ref *pTarget) {};
    virtual void hideLayerActionStart(Ref *pTarget) {};
    virtual void hideLayerActionFinish(Ref *pTarget) {};
};

#endif /* PixLayerActionDelegate_h */
