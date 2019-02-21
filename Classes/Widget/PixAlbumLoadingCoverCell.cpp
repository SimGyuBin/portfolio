//
//  PixAlbumLoadingCoverCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 9. 5..
//
//

#include "PixAlbumLoadingCoverCell.h"

PixAlbumLoadingCoverCell* PixAlbumLoadingCoverCell::create()
{
    PixAlbumLoadingCoverCell* widget = new (std::nothrow) PixAlbumLoadingCoverCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixAlbumLoadingCoverCell::PixAlbumLoadingCoverCell()
: _customItem(nullptr)
, mbln_Retry(false)
{
    
}

PixAlbumLoadingCoverCell::~PixAlbumLoadingCoverCell()
{
    
}

bool PixAlbumLoadingCoverCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixAlbumLoadingCoverCell::onEnter()
{
    Widget::onEnter();
//    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

void PixAlbumLoadingCoverCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixAlbumLoadingCoverCell::onExit()
{
//    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Widget::onExit();
}

void PixAlbumLoadingCoverCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
}

void PixAlbumLoadingCoverCell::update(float dt)
{
    Widget::update(dt);
}

void PixAlbumLoadingCoverCell::createCustomItem()
{
    _customItem = CSLoader::createNode("07_My_AlbumList/AlbumCoverLodingCell.csb");
    
    if(_customItem)
    {
        Node *loading = dynamic_cast<Node*>(_customItem->getChildByName("loading"));
        
        if(loading)
        {
            ActionTimeline *timeLine = CSLoader::createTimeline("04_SelectPhoto/loading_cell_2.csb");
            
            if(timeLine)
            {
                loading->runAction(timeLine);
                
                //                    int nStartIndex = RandomHelper::random_int(0, timeLine->getDuration());
                timeLine->gotoFrameAndPlay(0,true);
            }
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixAlbumLoadingCoverCell::setRetry(bool var)
{
    mbln_Retry = var;
    
    if(_customItem)
    {
        Node *loading = dynamic_cast<Node*>(_customItem->getChildByName("loading"));
        
        if(loading)
        {
            loading->setVisible(!mbln_Retry);
        }
        
        Node *Button_retry = dynamic_cast<Node*>(_customItem->getChildByName("Button_retry"));
        
        if(Button_retry)
        {
            Button_retry->setVisible(mbln_Retry);
        }
    }
}

bool PixAlbumLoadingCoverCell::isRetry()
{
    return mbln_Retry;
}

Node* PixAlbumLoadingCoverCell::getCellChildByName(const std::string& name)
{
    if(_customItem)
    {
        return _customItem->getChildByName(name);
    }
    
    return nullptr;
}
