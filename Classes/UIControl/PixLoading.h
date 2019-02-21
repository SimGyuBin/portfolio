//
//  PixLoading.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 25..
//
//

#ifndef PixLoadin_h
#define PixLoadin_h

#include "cocos2d.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixLoading
{
    
public :
    
    PixLoading();
    virtual ~PixLoading();
    
    virtual bool init(const std::string& filename, const std::string& timeline);
    virtual void play();
    virtual Node* getLoading();
    virtual void setLabelString();
    
    
private:
    
    Node *Loading;
    ActionTimeline *timeLine;
    
};

#endif /* PixLoading_h */
