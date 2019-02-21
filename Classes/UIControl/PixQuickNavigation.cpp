//
//  PixQuickNavigation.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 29..
//
//

#include "PixQuickNavigation.h"
#include "PixGalleryListView.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"

std::string PixQuickNavigation::getQuickNavigationYearMonth(int nYear, int nMonth)
{
    std::string strYearMonth = "";
        
    if(nMonth <= 12 && nMonth >= 1)
    {
        std::string strYear = StringUtils::format("%d",nYear);
        std::string strLanguageCode = MASSAGE_MANAGER->getLanguageCode();
        
        if(strLanguageCode.compare("ko") == 0)
        {
            std::string strMonth = QUICKNAVIGATION_MONTH[MULTILINGUAL_KO][nMonth - 1];
            
            strYearMonth.append(strYear).append("년");
            strYearMonth.append(" ");
            strYearMonth.append(strMonth).append("월");
        }
        else
        {
            std::string strMonth = QUICKNAVIGATION_MONTH[MULTILINGUAL_ETC][nMonth - 1];
          
            strYearMonth.append(strMonth);
            strYearMonth.append(" ");
            strYearMonth.append(strYear);
        }
    }
 
    return strYearMonth;
}

PixQuickNavigation::PixQuickNavigation(PixGalleryListView *pListView)
: _listView(nullptr)
, _quickNavigation(nullptr)
, _quickNavigationBar(nullptr)
, _quickNavigationLabelBackground(nullptr)
, _quickNavigationLabel(nullptr)
, _startPos(0.0f)
, _endPos(0.0f)
, _previousLocation(Vec2::ZERO)
, _isMoved(false)
, _isQuickNavigationBarShow(false)
{
    this->_listView = pListView;
}

PixQuickNavigation::~PixQuickNavigation()
{
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    scheduler->unschedule(schedule_selector(PixQuickNavigation::hideLayerAction), this);
}

void PixQuickNavigation::createQuickNavigation()
{
    if(_listView && _listView->getParent())
    {
        _quickNavigation = CSLoader::createNode("04_SelectPhoto/Navigation.csb");
        
        if(_quickNavigation)
        {
            float fPosX = 0.0f;
            
            _quickNavigationBar = dynamic_cast<ui::Button*>(_quickNavigation->getChildByName("navigation_bar"));
            
            if(_quickNavigationBar)
            {
                _quickNavigationBar->addTouchEventListener(CC_CALLBACK_2(PixQuickNavigation::onQuickNavigationBarTouch, this));
                
                fPosX = _listView->getContentSize().width + _quickNavigationBar->getContentSize().width;
                
                _startPos = _listView->getContentSize().height - _quickNavigationBar->getContentSize().height;
                
                if(_listView->getTopLayer())
                {
                    _startPos -= _listView->getTopLayer()->getContentSize().height;
                }
            }
            
            _quickNavigationLabelBackground = dynamic_cast<Sprite*>(_quickNavigation->getChildByName("navigation_day"));
            
            if(_quickNavigationLabelBackground)
            {
                _quickNavigationLabelBackground->setVisible(false);
                _quickNavigationLabel = dynamic_cast<ui::Text*>(_quickNavigationLabelBackground->getChildByName("Text"));
                
                if(_quickNavigationLabel)
                {
                    _quickNavigationLabel ->setString("");
                }
            }
            
            if(_listView->getBottomLayer())
            {
                _endPos += _listView->getBottomLayer()->getContentSize().height;
            }
            
            _quickNavigation->setPosition(Vec2(fPosX, _startPos));
            _listView->getParent()->addChild(_quickNavigation, 1000, QUICK_NAVIGATION_TAG);
        }
    }
}

void PixQuickNavigation::removeQuickNavigation()
{
    if(_listView && _listView->getParent())
    {
        if(_quickNavigation)
        {
            Scheduler *scheduler = Director::getInstance()->getScheduler();
            scheduler->unschedule(schedule_selector(PixQuickNavigation::hideLayerAction), this);
            
            _quickNavigation->stopAllActions();
            _listView->getParent()->removeChild(_quickNavigation);
            
            _quickNavigation = nullptr;
            _quickNavigationBar = nullptr;
            _quickNavigationLabelBackground = nullptr;
            _quickNavigationLabel = nullptr;
        }
    }
}

void PixQuickNavigation::hideQuickNavigation()
{
    if(_quickNavigation && _quickNavigationBar)
    {
        if(_quickNavigation)
        {
            _quickNavigation->stopActionByTag(QUICK_NAVIGATION_ACTION_TAG);
         
            Vec2 pos = Vec2(_listView->getContentSize().width + _quickNavigationBar->getContentSize().width,_quickNavigation->getPositionY());
            _quickNavigation->setPosition(pos);
        }
    }
}

void PixQuickNavigation::updateQuickNavigation()
{
    if(_listView && _quickNavigation)
    {
        if(_listView->isVisible() == false)
            return;
        
        if(_quickNavigation->getPositionX() > _listView->getContentSize().width)
        {
            _isQuickNavigationBarShow = false;
        }
        else
        {
            _isQuickNavigationBarShow = true;
        }
        
        Size containerSize = _listView->getInnerContainerSize();
        Size listViewSize = _listView->getContentSize();
        
        if(_isMoved == false)
        {
            Vec2 offset = _listView->getInnerContainerPosition();
           
            float fTopPos = listViewSize.height - containerSize.height;
            float fPercent = offset.y / fTopPos;
            
            if(_quickNavigationBar)
            {
                float fNavigationPos = (((_startPos - _endPos) - _quickNavigationBar->getContentSize().height) * fPercent) + _endPos;
                
                float minOffset = _endPos;
                float maxOffset = ((_startPos - _endPos) - _quickNavigationBar->getContentSize().height) + _endPos;
                
                if(fNavigationPos <= minOffset)
                {
                    fNavigationPos = minOffset;
                }
                
                if(fNavigationPos >= maxOffset)
                {
                    fNavigationPos = maxOffset;
                }
                
                _quickNavigation->setPositionY(fNavigationPos);
            }
            
            if(_quickNavigationLabelBackground)
            {
                _quickNavigationLabelBackground->setVisible(false);
            }
        }
        else
        {
            if(_quickNavigationLabelBackground)
            {
                Vec2 offset = _listView->getInnerContainerPosition();
                offset.y = fabsf(offset.y);
                offset.y += listViewSize.height * 0.5f;
                
                for (auto item : _listView->getItems())
                {
                    Rect rect =  item->getBoundingBox();
                    
                    if(rect.containsPoint(offset))
                    {
                        PixQuickNavigationUpdateLabel *updateLabel = dynamic_cast<PixQuickNavigationUpdateLabel*>(item);
                        
                        if(updateLabel)
                        {
                            updateLabel->setQuickNavigationUpdateLabel(_quickNavigationLabel);
                        }
                        
                        _quickNavigationLabelBackground->setVisible(true);
                    }
                }
            }
        }
        
        float fAllItemHeight = 0.0f;
        
        for (auto item : _listView->getItems())
        {
            fAllItemHeight += item->getContentSize().height;
        }
        
        if(fAllItemHeight < (listViewSize.height * 2))
        {
            _quickNavigation->setVisible(false);
        }
        else
        {
            _quickNavigation->setVisible(true);
        }
    }
}

float PixQuickNavigation::getStartPos()
{
    return _startPos;
}

void PixQuickNavigation::setStartPos(float fStart)
{
    _startPos = fStart;
}

float PixQuickNavigation::getEndPos()
{
    return _endPos;
}

void PixQuickNavigation::setEndPos(float fEnd)
{
    _endPos = fEnd;
}

#pragma mmark - QuickNavigationBar Event

void PixQuickNavigation::onQuickNavigationBarTouch(cocos2d::Ref *pSender, ui::Widget::TouchEventType type)
{
    if(_quickNavigation->getActionByTag(QUICK_NAVIGATION_ACTION_TAG))
        return;
    
    ui::Button *navigationBar = dynamic_cast<ui::Button*>(pSender);
    
    if(navigationBar)
    {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
            {
                _isMoved = false;
                _previousLocation = navigationBar->getTouchBeganPosition();
                
                if(_quickNavigationLabelBackground)
                {
                    _quickNavigationLabelBackground->setVisible(false);
                }
            }
                break;
                
            case ui::Widget::TouchEventType::MOVED:
            {
                _isMoved = true;

                Scheduler *scheduler = Director::getInstance()->getScheduler();
                
                if(scheduler->isScheduled(schedule_selector(PixQuickNavigation::hideLayerAction), this))
                {
                    scheduler->unschedule(schedule_selector(PixQuickNavigation::hideLayerAction), this);
                }
                
                if(_quickNavigation && _quickNavigationBar)
                {
                    Vec2 difference = navigationBar->getTouchBeganPosition() - navigationBar->getTouchMovePosition();
                
                    float minOffset = _endPos;
                    float maxOffset = ((_startPos - _endPos) - _quickNavigationBar->getContentSize().height) + _endPos;
                    
                    float fPosY = _quickNavigation->getPositionY();

                    Vec2 point = _quickNavigation->convertToNodeSpace(navigationBar->getTouchMovePosition());
                    Vec2 bef = _previousLocation;
                    bef = _quickNavigation->convertToNodeSpace(bef);
                    
                    fPosY = fPosY + (point.y - bef.y);

                    if(fPosY <= minOffset)
                    {
                        fPosY = minOffset;
                    }
                    
                    if(fPosY >= maxOffset)
                    {
                        fPosY = maxOffset;
                    }
                
                    _quickNavigation->setPositionY(fPosY);
                    
                    float fNavigationHight = (_startPos - _endPos) - _quickNavigationBar->getContentSize().height;
                    
                    float fPercent = (fPosY - _endPos) / fNavigationHight;
                    
                    if(_listView)
                    {
                        Vec2 offset = _listView->getInnerContainerPosition();
                        Size containerSize = _listView->getInnerContainerSize();
                        Size listViewSize = _listView->getContentSize();
                        
                        float fPercentVertical = (1.0f - fPercent) * 100;
                        
                        _listView->jumpToPercentVertical(fPercentVertical);
                    }
                }
                
                _previousLocation = navigationBar->getTouchMovePosition();
            }
                break;
                
            case ui::Widget::TouchEventType::ENDED:
            case ui::Widget::TouchEventType::CANCELED:
            {
                _isMoved = false;
                _previousLocation = Vec2::ZERO;
                
                Scheduler *scheduler = Director::getInstance()->getScheduler();
                scheduler->schedule(schedule_selector(PixQuickNavigation::hideLayerAction), this, QUICK_NAVIGATION_HIDE, false);
            }
                break;
                
            default:
                break;
        }
    }
}

#pragma mmark - ListView Touch

void PixQuickNavigation::onListViewTouchBegan(Touch *touch, Event *event)
{
    
}

void PixQuickNavigation::onListViewTouchMoved(Touch *touch, Event *event)
{
    if(_listView && _listView->isVisible())
    {
        Vec2 difference = touch->getLocation() - touch->getStartLocation();
        
        float move = 30.f;
        
        if ((difference.x > move || difference.y > move) || (difference.x < -move || difference.y < -move))
        {
            showLayerAction();
        }
    }
}

void PixQuickNavigation::onListViewTouchEnded(Touch *touch, Event *event)
{
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule(schedule_selector(PixQuickNavigation::hideLayerAction), this, QUICK_NAVIGATION_HIDE, false);
}

void PixQuickNavigation::onListViewTouchCancelled(Touch *touch, Event *event)
{
    
}

#pragma mark - Action

void PixQuickNavigation::showLayerAction()
{
    if(_listView && _quickNavigation)
    {
        if(_quickNavigation->getActionByTag(QUICK_NAVIGATION_ACTION_TAG))
            return;
     
        if(_listView->isVisible() == false)
            return;
        
        Scheduler *scheduler = Director::getInstance()->getScheduler();
        
        if(scheduler->isScheduled(schedule_selector(PixQuickNavigation::hideLayerAction), this))
        {
            scheduler->unschedule(schedule_selector(PixQuickNavigation::hideLayerAction), this);
        }
        
        if(_quickNavigation)
        {
            Vec2 pos = Vec2(_listView->getContentSize().width, _quickNavigation->getPositionY());
            MoveTo *moveTo = MoveTo::create(0.3f, pos);
            EaseExponentialOut *easeExponentialOut = EaseExponentialOut::create(moveTo);
            CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixQuickNavigation::showLayerActionFinish, this));
            
            Sequence *sequence = Sequence::create(easeExponentialOut, callFuncN, NULL);
            sequence->setTag(QUICK_NAVIGATION_ACTION_TAG);
            
            _quickNavigation->runAction(sequence);
            
            _quickNavigationBar->setEnabled(true);
        }
    }
}

void PixQuickNavigation::hideLayerAction(float dt)
{
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    scheduler->unschedule(schedule_selector(PixQuickNavigation::hideLayerAction), this);
    
    if(_listView && _quickNavigation)
    {
        if(_quickNavigation->getActionByTag(QUICK_NAVIGATION_ACTION_TAG))
            return;
        
        if(_quickNavigation && _quickNavigationBar)
        {
            Vec2 pos = Vec2(_listView->getContentSize().width + _quickNavigationBar->getContentSize().width,_quickNavigation->getPositionY());
            MoveTo *moveTo = MoveTo::create(0.3f, pos);
            EaseExponentialOut *easeExponentialOut = EaseExponentialOut::create(moveTo);
            CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixQuickNavigation::showLayerActionFinish, this));
            Sequence *sequence = Sequence::create(easeExponentialOut, callFuncN, NULL);
            sequence->setTag(QUICK_NAVIGATION_ACTION_TAG);
            
            _quickNavigation->runAction(sequence);
            
            _quickNavigationBar->setEnabled(false);
        }
    }
}

void PixQuickNavigation::showLayerActionFinish(Node* node)
{
    
}

void PixQuickNavigation::hideLayerActionFinish(Node* node)
{

}




