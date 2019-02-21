//
//  PixOrderSearchLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 12..
//
//

#include "PixOrderSearchLayer.h"
#include "PixMenuOrderLayer.h"
#include "../Manager/PixMessageManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"

PixOrderSearchLayer* PixOrderSearchLayer::create()
{
    PixOrderSearchLayer *orderSearchLayer = new (std::nothrow) PixOrderSearchLayer();
    if (orderSearchLayer && orderSearchLayer->init())
    {
        orderSearchLayer->autorelease();
        return orderSearchLayer;
    }
    else
    {
        CC_SAFE_DELETE(orderSearchLayer);
        return nullptr;
    }
}

PixOrderSearchLayer::PixOrderSearchLayer()
: rootNode(nullptr)
, Node_MenuOrder_Pop(nullptr)
, mDelegate(nullptr)
{
    hideLayerFunction = std::bind(&PixOrderSearchLayer::hideLayerAction,this);
}

PixOrderSearchLayer::~PixOrderSearchLayer()
{
    
}

void PixOrderSearchLayer::initLayer()
{
    rootNode = CSLoader::createNode("12_Oder/MenuOrder_PopLayer.csb");
    
    if(rootNode)
    {
        Node_MenuOrder_Pop = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuOrder_Pop"));
        
        if(Node_MenuOrder_Pop)
        {
            int nCount = (int)Node_MenuOrder_Pop->getChildrenCount();
            
            for (int i = 0; i < nCount; ++i)
            {
                std::string strKey = StringUtils::format("Button_%02d", i + 1);
                
                ui::Button* button = dynamic_cast<ui::Button*>(Node_MenuOrder_Pop->getChildByName(strKey));
                
                if (button)
                {
                    int nTag = getItemTag(i);
                    
                    button->setTag(nTag);
                    button->addTouchEventListener(CC_CALLBACK_2(PixOrderSearchLayer::buttonSearchPressed, this));
                    
                    strKey = StringUtils::format("Text_%02d", i + 1);
                    ui::Text *text = dynamic_cast<ui::Text*>(button->getChildByName(strKey));
                    
                    if(text)
                    {
                        text->setTag(nTag);
                        text->setString(CODE_NAME(text->getString()));
                    }
                }
            }
        }
        
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixOrderSearchLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixOrderSearchLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

void PixOrderSearchLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

int PixOrderSearchLayer::getItemTag(int nIndex)
{
//    ORDER_NONE = 0,
//    ORDER_DEPOSIT_CHECK = 1,         // 입금 확인중
//    ORDER_PRODUCTION_PREPARING = 2,  // 제작 준비중
//    ORDER_IN_PRODUCTION = 4,         // 제작중
//    ORDER_DELIVERY = 8,              // 배송중
//    ORDER_COMPLETE_DELIVERY = 16,    // 배송완료
//    ORDER_CANCELLATION = 32,         // 주문취소
   
    ORDER_TYPE eType = ORDER_NONE;
    
    switch (nIndex)
    {
        case 0:
            eType = ORDER_NONE;
            break;
            
        case 1:
            eType = ORDER_DEPOSIT_CHECK;
            break;
            
        case 2:
            eType = ORDER_PRODUCTION_PREPARING;
            break;
            
        case 3:
            eType = ORDER_IN_PRODUCTION;
            break;
            
        case 4:
            eType = ORDER_DELIVERY;
            break;
            
        case 5:
            eType = ORDER_COMPLETE_DELIVERY;
            break;
            
        case 6:
            eType = ORDER_CANCELLATION;
            break;
            
        default:
            break;
    }
    
    return (int)eType;
}

#pragma mark - Button

void PixOrderSearchLayer::buttonSearchPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        ui::Button* button = dynamic_cast<ui::Button*>(pSender);
        
        if(button)
        {
            int nTag = button->getTag();
            ui::Text *text = dynamic_cast<ui::Text*>(button->getChildByTag(nTag));
            
            if(text)
            {
                if(mDelegate)
                {
                    mDelegate->orderSearchLayerSearchPressed(nTag, text->getString());
                }
            }
            
            hideLayerAction();
        }
    }
}

#pragma mark - Touch & Touches

void PixOrderSearchLayer::onTouchEnded(Touch* touch, Event* event)
{
    if(Node_MenuOrder_Pop)
    {
        if(NodeContainsPoint(this, touch->getLocation()))
        {
            if(!NodeContainsPoint(Node_MenuOrder_Pop, touch->getLocation()))
            {
                hideLayerAction();
            }
        }
    }
}

#pragma mark - Android Destroy

void PixOrderSearchLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    hideLayerAction();
}
