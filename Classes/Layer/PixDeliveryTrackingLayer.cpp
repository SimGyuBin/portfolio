//
//  PixDeliveryTrackingLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 18..
//
//

#include "PixDeliveryTrackingLayer.h"
#include "../Manager/PixMessageManager.h"
#include "../Info/OrderCheckInfo.h"
#include "../Common.h"

PixDeliveryTrackingLayer* PixDeliveryTrackingLayer::create(OrderCheckInfo *pOrderCheckInfo)
{
    PixDeliveryTrackingLayer *deliveryTrackingLayer = new (std::nothrow) PixDeliveryTrackingLayer(pOrderCheckInfo);
    if (deliveryTrackingLayer && deliveryTrackingLayer->init())
    {
        deliveryTrackingLayer->autorelease();
        return deliveryTrackingLayer;
    }
    else
    {
        CC_SAFE_DELETE(deliveryTrackingLayer);
        return nullptr;
    }
}

PixDeliveryTrackingLayer::PixDeliveryTrackingLayer(OrderCheckInfo *pOrderCheckInfo)
: rootNode(nullptr)
, mbln_IsAction(false)
, mOrderCheckInfo(pOrderCheckInfo)
, mDelegate(nullptr)
{
    hideLayerFunction = std::bind(&PixDeliveryTrackingLayer::hideDeliveryTrackingLayerAction,this);
}

PixDeliveryTrackingLayer::~PixDeliveryTrackingLayer()
{
    
}

void PixDeliveryTrackingLayer::initLayer()
{
    if(mOrderCheckInfo == nullptr)
        return;
    
    rootNode = CSLoader::createNode("12_Oder/MenuOrderShippingLayer_2.csb");
    
    if(rootNode)
    {
        Node *Node_MenuOrderShipping_Top_2 = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuOrderShipping_Top_2"));
        
        if(Node_MenuOrderShipping_Top_2)
        {
            ui::Text *label = dynamic_cast<ui::Text*>(Node_MenuOrderShipping_Top_2->getChildByName("label"));
            
            if(label)
            {
                label->setString(CODE_NAME(label->getString()));
            }
            
            ui::Button *Button_cancel = dynamic_cast<ui::Button*>(Node_MenuOrderShipping_Top_2->getChildByName("Button_cancel"));
            
            if(Button_cancel)
            {
                Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixDeliveryTrackingLayer::buttonCancelPressed, this));
            }
        }
        
        ui::Layout *Panel_WEB = dynamic_cast<ui::Layout*>(rootNode->getChildByName("Panel_WEB"));
        
        if(Panel_WEB)
        {
            Node *loading_cell = dynamic_cast<Node*>(Panel_WEB->getChildByName("loading_cell"));
            
            if(loading_cell)
            {
                ActionTimeline *timeLine = CSLoader::createTimeline("04_SelectPhoto/loading_cell_2.csb");
                
                if(timeLine)
                {
                    timeLine->gotoFrameAndPlay(0,true);
                    loading_cell->runAction(timeLine);
                }
            }
            
            auto webView = cocos2d::experimental::ui::WebView::create();
            
            webView->setOnDidFailLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
                
            });
            
            webView->setOnJSCallback([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
                
            });
            
            webView->setOnDidFinishLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url) {
                
            });
            
            webView->setOnShouldStartLoading([](cocos2d::experimental::ui::WebView *sender, const std::string &url)->bool {
                
                
                return true;
                
            });
            
            webView->setScalesPageToFit(true);
            webView->setBounces(false);
            webView->loadURL(mOrderCheckInfo->getInvoice_url());  
            webView->setPosition(Vec2::ZERO);
            webView->setAnchorPoint(Vec2::ZERO);
            webView->setContentSize(Panel_WEB->getContentSize());
            
            Panel_WEB->addChild(webView);
        }
        
        Size size = rootNode->getContentSize();
        setPosition(Vec2(0,-size.height));
        setAnchorPoint(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixDeliveryTrackingLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    showDeliveryTrackingLayerAction();
}

void PixDeliveryTrackingLayer::update(float dt)
{
    PixModalLayer::update(dt);
}

#pragma mark - Button

void PixDeliveryTrackingLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        hideDeliveryTrackingLayerAction();
    }
}

#pragma mark - Action

void PixDeliveryTrackingLayer::showDeliveryTrackingLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixDeliveryTrackingLayer::showDeliveryTrackingLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixDeliveryTrackingLayer::hideDeliveryTrackingLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixDeliveryTrackingLayer::hideDeliveryTrackingLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    stopAllActions();
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixDeliveryTrackingLayer::showDeliveryTrackingLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixDeliveryTrackingLayer::hideDeliveryTrackingLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        if(mDelegate)
        {
            mDelegate->hideLayerActionFinish(this);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Android BackPressed

void PixDeliveryTrackingLayer::androidBackPressed()
{
    
}

