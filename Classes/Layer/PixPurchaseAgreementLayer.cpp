//
//  PixPurchaseAgreementLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 22..
//
//

#include "PixPurchaseAgreementLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixPurchaseAgreementLayer* PixPurchaseAgreementLayer::create()
{
    PixPurchaseAgreementLayer *purchaseAgreementLayer = new (std::nothrow) PixPurchaseAgreementLayer();
    if (purchaseAgreementLayer && purchaseAgreementLayer->init())
    {
        purchaseAgreementLayer->autorelease();
        return purchaseAgreementLayer;
    }
    else
    {
        CC_SAFE_DELETE(purchaseAgreementLayer);
        return nullptr;
    }
}

PixPurchaseAgreementLayer::PixPurchaseAgreementLayer()
: rootNode(nullptr)
{
    hideLayerFunction = std::bind(&PixPurchaseAgreementLayer::hideLayerAction,this);
}

PixPurchaseAgreementLayer::~PixPurchaseAgreementLayer()
{
    
}

void PixPurchaseAgreementLayer::initLayer()
{
    rootNode = CSLoader::createNode("12_Oder/MenuPay_Pop3Layer.csb");
    
    if(rootNode)
    {
        Node *Node_MenuPay_Pop3 = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuPay_Pop3"));
     
        if(Node_MenuPay_Pop3)
        {
            Node *Node_MenuPay_Pop3_Top = dynamic_cast<Node*>(Node_MenuPay_Pop3->getChildByName("Node_MenuPay_Pop3_Top"));
            
            if(Node_MenuPay_Pop3_Top)
            {
                ui::Button *Button_cancel = dynamic_cast<ui::Button*>(Node_MenuPay_Pop3_Top->getChildByName("Button_cancel"));
                
                if(Button_cancel)
                {
                    Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixPurchaseAgreementLayer::buttonCancelPressed, this));
                }
                
                ui::Text *label = dynamic_cast<ui::Text*>(Node_MenuPay_Pop3_Top->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
            }
        
            ui::Text *Text_Title_1 = dynamic_cast<ui::Text*>(Node_MenuPay_Pop3->getChildByName("Text_Title_1"));
            
            if(Text_Title_1)
            {
                Text_Title_1->setString(CODE_NAME(Text_Title_1->getString()));
            }
            
            ui::Text *Text_1 = dynamic_cast<ui::Text*>(Node_MenuPay_Pop3->getChildByName("Text_1"));
            
            if(Text_1)
            {
                Text_1->setString(CODE_NAME(Text_1->getString()));
            }
            
            ui::Text *Text_Title_2 = dynamic_cast<ui::Text*>(Node_MenuPay_Pop3->getChildByName("Text_Title_2"));
            
            if(Text_Title_2)
            {
                Text_Title_2->setString(CODE_NAME(Text_Title_2->getString()));
            }
            
            ui::Text *Text_2 = dynamic_cast<ui::Text*>(Node_MenuPay_Pop3->getChildByName("Text_2"));
            
            if(Text_2)
            {
                Text_2->setString(CODE_NAME(Text_2->getString()));
            }
            
            ui::Text *Text_Title_3 = dynamic_cast<ui::Text*>(Node_MenuPay_Pop3->getChildByName("Text_Title_3"));
            
            if(Text_Title_3)
            {
                Text_Title_3->setString(CODE_NAME(Text_Title_3->getString()));
            }
            
            ui::Text *Text_3 = dynamic_cast<ui::Text*>(Node_MenuPay_Pop3->getChildByName("Text_3"));
            
            if(Text_3)
            {
                Text_3->setString(CODE_NAME(Text_3->getString()));
            }
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        
        addChild(rootNode);
    }
}

void PixPurchaseAgreementLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixPurchaseAgreementLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Button

void PixPurchaseAgreementLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED )
    {
        hideLayerAction();
    }
}

#pragma mark - Android BackPressed

void PixPurchaseAgreementLayer::androidBackPressed()
{
    hideLayerAction();
}
