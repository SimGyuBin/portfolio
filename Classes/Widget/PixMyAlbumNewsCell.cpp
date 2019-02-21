//
//  PixMyAlbumNewsCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 24..
//
//

#include "PixMyAlbumNewsCell.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Info/BoardBastInfo.h"
#include "../Common.h"

PixMyAlbumNewsCell* PixMyAlbumNewsCell::create()
{
    PixMyAlbumNewsCell* widget = new (std::nothrow) PixMyAlbumNewsCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixMyAlbumNewsCell::PixMyAlbumNewsCell()
: _customItem(nullptr)
, Text_News(nullptr)
, Panel(nullptr)
, textFlow(nullptr)
, frame(Size::ZERO)
, fontName(nullptr)
, fontSize(0)
, color(Color4B::BLACK)
, pos(Vec2::ZERO)
, anchor(Vec2::ZERO)
, mBoardBastInfo(nullptr)
{
    newsQueue.clear();
}

PixMyAlbumNewsCell::~PixMyAlbumNewsCell()
{
    newsQueue.clear();
}

bool PixMyAlbumNewsCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixMyAlbumNewsCell::onEnter()
{
     Widget::onEnter();
}

void PixMyAlbumNewsCell::createCustomItem()
{
    _customItem = CSLoader::createNode("07_My_AlbumList/MyAlbumNewsLayer.csb");
    
    if(_customItem)
    {
        ui::Layout *Background = dynamic_cast<ui::Layout*>(_customItem->getChildByName("Background"));
        
        if(Background)
        {       
            ui::Layout *Panel_1 = dynamic_cast<ui::Layout*>(Background->getChildByName("Panel_1"));
            
            if(Panel_1)
            {
                Text_News = dynamic_cast<ui::Text*>(Panel_1->getChildByName("Text_News"));
                
                if(Text_News)
                {
                    Text_News->setString("");
                }
            }
            
            Panel = dynamic_cast<ui::Layout*>(Background->getChildByName("Panel_2"));
            
            if(Panel)
            {
                ui::Text *Text_Title = dynamic_cast<ui::Text*>(Panel->getChildByName("Text_Title"));
                
                if(Text_Title)
                {
                    frame = Text_Title->getContentSize();
                    fontName = Text_Title->getFontName().c_str();
                    fontSize = Text_Title->getFontSize();
                    color = Text_Title->getTextColor();
                    pos = Text_Title->getPosition();
                    anchor = Text_Title->getAnchorPoint();
                       
                    Text_Title->removeFromParentAndCleanup(true);
                }
            }
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixMyAlbumNewsCell::pushBoardBastInfo(BoardBastInfo *pBoardBastInfo)
{
    newsQueue.push_back(pBoardBastInfo);
}

void PixMyAlbumNewsCell::startFlow()
{
    BoardBastInfo *pBoardBastInfo = newsQueue.front();
    newsQueue.pop_front();
    newsQueue.push_back(pBoardBastInfo);
    
    createTextFlow(pBoardBastInfo);
}

void PixMyAlbumNewsCell::createTextFlow(BoardBastInfo *pBoardBastInfo)
{
    if(pBoardBastInfo == nullptr)
        return;
    
    mBoardBastInfo = pBoardBastInfo;
    
    if(Text_News)
    {
        std::string strNews = "";
        
        if(pBoardBastInfo->getBoard_type() == 1)
        {
            strNews = CODE_NAME("Menu03");
        }
        else if(pBoardBastInfo->getBoard_type() == 2)
        {
            strNews = CODE_NAME("Menu39");
        }
        
        Text_News->setString(strNews);
    }
    
    if(Panel)
    {   
        if(textFlow)
        {
            Panel->removeChild(textFlow);
        }
        
        textFlow = PixTextFlow::create(frame, fontName, fontSize, FLOW_LEFT);
        textFlow->setDelegate(this);
        textFlow->setTextColor(color);
        textFlow->setString(pBoardBastInfo->getTitle().c_str());
        textFlow->setPosition(pos);
        textFlow->setAnchorPoint(anchor);
        textFlow->setflowWay(2.0f);
        textFlow->startFlow(false);
        
        Panel->addChild(textFlow);
    }
}

#pragma mark - Delegate

void PixMyAlbumNewsCell::textFlowActionFinish()
{
    if(textFlow)
    {
        BoardBastInfo *pBoardBastInfo = newsQueue.front();
        newsQueue.pop_front();
        newsQueue.push_back(pBoardBastInfo);
        
        createTextFlow(pBoardBastInfo);
    }
}

