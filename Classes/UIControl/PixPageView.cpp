//
//  PixPageView.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 23..
//
//

#include "PixPageView.h"

PixPageView* PixPageView::create()
{
    PixPageView* widget = new (std::nothrow) PixPageView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixPageView::PixPageView()
{
    
}

PixPageView::~PixPageView()
{
    
}

bool PixPageView::init()
{
    if (PageView::init())
    {
        return true;
    }
    
    return false;
}

void PixPageView::onEnter()
{
    PageView::onEnter();
}

void PixPageView::onEnterTransitionDidFinish()
{
    PageView::onEnterTransitionDidFinish(); 
}

void PixPageView::update(float dt)
{
    PageView::update(dt);
    
    if(_isTransitionFinished)
    {
        Vec2 offset = _innerContainer->getPosition();
        
        for (int i = 0; i < getChildrenCount(); i++)
        {
            Node *child = getChildren().at(i);
            
            if(child)
            {
                Size size = this->getContentSize();
                float scale = this->getScale();
                
                Rect viewRect;
                
                viewRect = Rect(-offset.x/scale, -offset.y/scale, size.width/scale, size.height/scale);
                
                if(viewRect.intersectsRect(child->getBoundingBox()))
                {
                    child->setVisible(true);
                }
                else
                {
                    child->setVisible(false);
                }
            }
        }
    }
}

void PixPageView::copyClonedPageView(ui::PageView *pPageView)
{
    this->copyProperties(pPageView);
    this->copyClonedWidgetChildren(pPageView);
    
    if(pPageView->getParent())
    {
        pPageView->getParent()->addChild(this);
        pPageView->getParent()->removeChild(pPageView);
    }
    
    pPageView->removeFromParentAndCleanup(true);
}
