//
//  PixLoadin.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 25..
//
//

#include "PixLoading.h"
#include "../Util/PixUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixLoading::PixLoading()
: Loading(nullptr)
, timeLine(nullptr)
{
    
}

PixLoading::~PixLoading()
{
    CC_SAFE_RELEASE_NULL(Loading);
    CC_SAFE_RELEASE_NULL(timeLine);
}

bool PixLoading::init(const std::string& filename, const std::string& timeline)
{
    Loading = CSLoader::createNode(filename);
    Loading->retain();
    setModalLayer(Loading);
    
    if(Loading)
    {
        Node *loading = dynamic_cast<Node*>(Loading->getChildByName("loading"));
        
        if(loading)
        {
            timeLine = CSLoader::createTimeline(timeline);
            
            if(timeLine)
            {
                timeLine->gotoFrameAndPlay(0,true);
                loading->runAction(timeLine);
                timeLine->retain();
            }
        }
        
        return true;
    }

    return false;
}

Node* PixLoading::getLoading()
{
    play();
    return Loading;
}

void PixLoading::play()
{
    if(timeLine)
    {
        timeLine->gotoFrameAndPlay(0,true);
    }
}

void PixLoading::setLabelString()
{
    if(Loading)
    {
        ui::Text *label =  dynamic_cast<ui::Text*>(Loading->getChildByName("label"));
        
        if(label)
        {
            label->setString(CODE_NAME(label->getString()));
        }
    }
}
