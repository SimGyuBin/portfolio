//
//  PixPayDeliveryListLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 23..
//
//

#include "PixPayDeliveryListLayer.h"
#include "../Info/DeliveryInfo.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"
#include "../Widget/PixPayDeliveryListCell.h"

PixPayDeliveryListLayer* PixPayDeliveryListLayer::create(std::vector<DeliveryInfo*> *pDeliveryVec)
{
    PixPayDeliveryListLayer *payDeliveryListLayer = new (std::nothrow) PixPayDeliveryListLayer(pDeliveryVec);
    if (payDeliveryListLayer && payDeliveryListLayer->init())
    {
        payDeliveryListLayer->autorelease();
        return payDeliveryListLayer;
    }
    else
    {
        CC_SAFE_DELETE(payDeliveryListLayer);
        return nullptr;
    }
}
    
PixPayDeliveryListLayer::PixPayDeliveryListLayer(std::vector<DeliveryInfo*> *pDeliveryVec)
: rootNode(nullptr)
, deliveryListView(nullptr)
, mDeliveryVec(pDeliveryVec)
, mDelegate(nullptr)
, mbln_IsAction(false)
{
    hideLayerFunction = std::bind(&PixPayDeliveryListLayer::hideLayerAction,this);
}

PixPayDeliveryListLayer::~PixPayDeliveryListLayer()
{
    
}
    
void PixPayDeliveryListLayer::initLayer()
{
    rootNode = CSLoader::createNode("12_Oder/MenuPay_Pop4Layer.csb");
    
    if(rootNode)
    {
        Node *Node_MenuPay_Pop4 = dynamic_cast<Node*>(rootNode->getChildByName("Node_MenuPay_Pop4"));
        
        if(Node_MenuPay_Pop4)
        {
            Node *Node_MenuPay_Pop4_Top = dynamic_cast<Node*>(Node_MenuPay_Pop4->getChildByName("Node_MenuPay_Pop4_Top"));
            
            if(Node_MenuPay_Pop4_Top)
            {
                ui::Text *label = dynamic_cast<ui::Text*>(Node_MenuPay_Pop4_Top->getChildByName("label"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
                
                ui::Button *Button_cancel = dynamic_cast<ui::Button*>(Node_MenuPay_Pop4_Top->getChildByName("Button_cancel"));
                
                if(Button_cancel)
                {
                    Button_cancel->addTouchEventListener(CC_CALLBACK_2(PixPayDeliveryListLayer::buttonCancelPressed, this));
                }
            }
            
            ui::ListView *listView = dynamic_cast<ui::ListView*>(Node_MenuPay_Pop4->getChildByName("ListView"));
            
            if(listView)
            {
                deliveryListView = PixGalleryListView::create();
                deliveryListView->copyClonedListView(listView);
                deliveryListView->setEnabled(true);
                createDeliveryListView();
            }
            
            ui::Button *Button_ok = dynamic_cast<ui::Button*>(Node_MenuPay_Pop4->getChildByName("Button_Complete"));
            
            if(Button_ok)
            {
                Button_ok->addTouchEventListener(CC_CALLBACK_2(PixPayDeliveryListLayer::buttonCompletePressed, this));
                
                ui::Text *Text = dynamic_cast<ui::Text*>(Button_ok->getChildByName("Text"));
                
                if(Text)
                {
                    Text->setString(CODE_NAME(Text->getString()));
                }
            }
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        
        addChild(rootNode);
    }
}

void PixPayDeliveryListLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixPayDeliveryListLayer::createDeliveryListView()
{
    if(mDeliveryVec == nullptr)
        return;
    
    if(deliveryListView == nullptr)
        return;
    
    for (auto iter = mDeliveryVec->begin(); iter != mDeliveryVec->end(); ++iter)
    {
        PixPayDeliveryCell *cell = PixPayDeliveryCell::create((*iter));
        cell->setDelegate(this);
        deliveryListView->pushBackCustomItem(cell);
    }
}

void PixPayDeliveryListLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Button
    
void PixPayDeliveryListLayer::buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        hideLayerAction();
    }
}

void PixPayDeliveryListLayer::buttonCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction == true)
            return;
        
        if(mDelegate)
        {
            DeliveryInfo *pInfo = nullptr;
            
            if(deliveryListView)
            {
                for (auto item : deliveryListView->getItems())
                {
                    PixPayDeliveryCell *cell = dynamic_cast<PixPayDeliveryCell*>(item);
                    
                    if(cell)
                    {
                        if(cell->isCheckSelected() == true)
                        {
                            pInfo = cell->getDeliveryInfo();
                            break;
                        }
                    }
                }
            }
            
            mDelegate->payDeliveryListLayerCompletePressed(pInfo);
            
        }
        
        hideLayerAction();
    }
}

#pragma mark - Delegate

void PixPayDeliveryListLayer::payDeliveryCellcheckBoxPressed(DeliveryInfo *pInfo)
{
    if(pInfo == nullptr)
        return;
    
    if(deliveryListView == nullptr)
        return;
    
    for (auto item : deliveryListView->getItems())
    {
        PixPayDeliveryCell *cell = dynamic_cast<PixPayDeliveryCell*>(item);
        
        if(cell && cell->getDeliveryInfo())
        {
            if(cell->getDeliveryInfo() != pInfo)
            {
                cell->setCheckSelected(false);
            }
        }
    }
}

void PixPayDeliveryListLayer::payDeliveryCellDeleteActionStart(Ref *pTarget)
{
    mbln_IsAction = true;
    
    PixPayDeliveryCell *cell = dynamic_cast<PixPayDeliveryCell*>(pTarget);
    
    if(cell)
    {
        if(deliveryListView)
        {
            deliveryListView->setBounceEnabled(false);
            deliveryListView->setEnabled(false);
        }
    }
}

void PixPayDeliveryListLayer::payDeliveryCellDeleteActionFinish(Ref *pTarget)
{
    mbln_IsAction = false;
 
    PixPayDeliveryCell *cell = dynamic_cast<PixPayDeliveryCell*>(pTarget);
    
    if(cell)
    {
        if(deliveryListView)
        {
            DeliveryInfo *pInfo = cell->getDeliveryInfo();
            
            bool blnSelected = false;
            
            if(pInfo)
            {
                blnSelected = pInfo->getSelected();
                
                for (auto iter = mDeliveryVec->begin(); iter != mDeliveryVec->end(); ++iter)
                {
                    if((*iter) == pInfo)
                    {
                        mDeliveryVec->erase(iter);
                        break;
                    }
                }
            }
            
            ssize_t nIndex = deliveryListView->getIndex(cell);
            
            deliveryListView->removeItem(nIndex);
            
            --nIndex;
            
            if(nIndex > 0)
            {
                deliveryListView->jumpToItem(nIndex, Vec2::ZERO, Vec2::ZERO);
            }
            
            deliveryListView->setBounceEnabled(true);
            deliveryListView->setEnabled(true);
            deliveryListView->stopAutoScroll();
            
            if(blnSelected)
            {
                if(!mDeliveryVec->empty())
                {
                    pInfo = (*mDeliveryVec->begin());
                    pInfo->setSelected(true);
                    pInfo->setLast(true);
                    
                    for (auto item : deliveryListView->getItems())
                    {
                        PixPayDeliveryCell *cell = dynamic_cast<PixPayDeliveryCell*>(item);
                        
                        if(cell)
                        {
                            if(cell->getDeliveryInfo() == pInfo)
                            {
                                cell->setCheckSelected(true);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void PixPayDeliveryListLayer::payDeliveryCellNoAddressAction(Ref *pTarget)
{
    mbln_IsAction = false;
    
    PixPayDeliveryCell *cell = dynamic_cast<PixPayDeliveryCell*>(pTarget);
    
    if(cell)
    {
        if(deliveryListView)
        {
            DeliveryInfo *pInfo = cell->getDeliveryInfo();
            
            if(pInfo)
            {
                for (auto iter = mDeliveryVec->begin(); iter != mDeliveryVec->end(); ++iter)
                {
                    if((*iter) == pInfo)
                    {
                        mDeliveryVec->erase(iter);
                        break;
                    }
                }
            }
        }
        
        if(mDelegate)
        {
            mDelegate->payDeliveryListLayerNoAddressAction();
        }
        
        if(getParent())
        {
            this->removeFromParentAndCleanup(true);
        }
    }
}

#pragma mark - Android BackPressed
    
void PixPayDeliveryListLayer::androidBackPressed()
{
    if(mbln_IsAction == true)
        return;
    
    hideLayerAction();
}
