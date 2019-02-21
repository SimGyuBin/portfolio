//
//  PixAlbumViewLoading.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 17..
//
//

#include "PixAlbumViewLoading.h"
#include "../Util/PixUtils.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixAlbumViewLoading::PixAlbumViewLoading()
: Loading(nullptr)
, timeLine(nullptr)
{
    
}

PixAlbumViewLoading::~PixAlbumViewLoading()
{

}

bool PixAlbumViewLoading::init(const std::string& filename, const std::string& timeline)
{
    Loading = CSLoader::createNode(filename);
    
    if(Loading)
    {
        ui::Layout *Panel = dynamic_cast<ui::Layout*>(Loading->getChildByName("Panel"));
        
        if(Panel)
        {
            Node *LoadingLayer = dynamic_cast<Node*>(Panel->getChildByName("LoadingLayer"));
            
            if(LoadingLayer)
            {
                Node *loading = dynamic_cast<Node*>(LoadingLayer->getChildByName("loading"));
             
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
                
                ui::Text *label =  dynamic_cast<ui::Text*>(LoadingLayer->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
            }
        }

        return true;
    }
    
    return false;
}

Node* PixAlbumViewLoading::getLoading()
{
    play();
    return Loading;
}

void PixAlbumViewLoading::play()
{
    if(timeLine)
    {
        timeLine->gotoFrameAndPlay(0,true);
    }
}


