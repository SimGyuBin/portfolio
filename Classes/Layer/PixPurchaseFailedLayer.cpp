//
//  PixPurchaseFailedLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 4..
//
//

#include "PixPurchaseFailedLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPurchaseFailedLayer* PixPurchaseFailedLayer::create(const std::string &strFail_reason)
{
    PixPurchaseFailedLayer *purchaseFailedLayer = new (std::nothrow) PixPurchaseFailedLayer(strFail_reason);
    if (purchaseFailedLayer && purchaseFailedLayer->init())
    {
        purchaseFailedLayer->autorelease();
        return purchaseFailedLayer;
    }
    else
    {
        CC_SAFE_DELETE(purchaseFailedLayer);
        return nullptr;
    }
}

PixPurchaseFailedLayer::PixPurchaseFailedLayer(const std::string &strFail_reason)
: rootNode(nullptr)
, failedLayerListView(nullptr)
, mstr_Fail_reason(strFail_reason)
{
    hideLayerFunction = std::bind(&PixPurchaseFailedLayer::hideLayerAction,this);
}

PixPurchaseFailedLayer::~PixPurchaseFailedLayer()
{
    
}

void PixPurchaseFailedLayer::initLayer()
{
    rootNode = CSLoader::createNode("12_Oder/MenuOrderFailed_PopLayer.csb");
    
    if(rootNode)
    {
        Node *Node_MenuOrderFailed_Pop = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuOrderFailed_Pop"));
     
        if(Node_MenuOrderFailed_Pop)
        {
            Node *Node_MenuOrderFailed_Pop_Top = dynamic_cast<Node*>(Node_MenuOrderFailed_Pop->getChildByName("Node_MenuOrderFailed_Pop_Top"));
            
            if(Node_MenuOrderFailed_Pop_Top)
            {
                ui::Layout *Top_01 =  dynamic_cast<ui::Layout*>(Node_MenuOrderFailed_Pop_Top->getChildByName("Top_01"));
                
                if(Top_01)
                {
                    ui::Text *Text_Title = dynamic_cast<ui::Text*>(Top_01->getChildByName("Text_Title"));
                    
                    if(Text_Title)
                    {
                        Text_Title->setString(CODE_NAME(Text_Title->getString()));
                    }
                    
                    ui::Button *Button_cancel = dynamic_cast<ui::Button*>(Top_01->getChildByName("Button_cancel"));
                    
                    if(Button_cancel)
                    {
                         Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixPurchaseFailedLayer::buttonCancelPressed, this));
                    }
                }
            }
            
            ui::ListView *ListView = dynamic_cast<ui::ListView*>(Node_MenuOrderFailed_Pop->getChildByName("ListView"));
            
            if(ListView)
            {
                failedLayerListView = PixGalleryListView::create();
                failedLayerListView->copyClonedListView(ListView);
                createFailedLayerListView();
            }
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        
        addChild(rootNode);
    }
}

void PixPurchaseFailedLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixPurchaseFailedLayer::createFailedLayerListView()
{
    if(failedLayerListView == nullptr)
        return;
    
    PixPurchaseFailedTopCell *topCell = PixPurchaseFailedTopCell::create(getFail_reason());
    failedLayerListView->pushBackCustomItem(topCell);
    
    PixPurchaseFailedBottomCell *bottomCell = PixPurchaseFailedBottomCell::create();
    failedLayerListView->pushBackCustomItem(bottomCell);
}

void PixPurchaseFailedLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Button

void PixPurchaseFailedLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        hideLayerAction();
    }
}



