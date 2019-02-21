//
//  PixMenuCustomerCategoryLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 27..
//
//

#include "PixMenuCustomerCategoryLayer.h"
#include "../Manager/PixMessageManager.h"
#include "../Util/PixUtils.h"
#include "../Info/FaqInfo.h"
#include "../Common.h"


PixMenuCustomerCategoryLayer* PixMenuCustomerCategoryLayer::create(std::vector<FaqCategoryInfo*> *pFaqCategoryInfoVec)
{
    PixMenuCustomerCategoryLayer *menuCustomerCategoryLayer = new (std::nothrow) PixMenuCustomerCategoryLayer(pFaqCategoryInfoVec);
    if (menuCustomerCategoryLayer && menuCustomerCategoryLayer->init())
    {
        menuCustomerCategoryLayer->autorelease();
        return menuCustomerCategoryLayer;
    }
    else
    {
        CC_SAFE_DELETE(menuCustomerCategoryLayer);
        return nullptr;
    }
}

PixMenuCustomerCategoryLayer::PixMenuCustomerCategoryLayer(std::vector<FaqCategoryInfo*> *pFaqCategoryInfoVec)
: rootNode(nullptr)
, Panel_Category(nullptr)
, CategoryListView(nullptr)
, mFaqCategoryInfoVec(pFaqCategoryInfoVec)
, mDelegate(nullptr)
{
    hideLayerFunction = std::bind(&PixMenuCustomerCategoryLayer::hideLayerAction,this);
}

PixMenuCustomerCategoryLayer::~PixMenuCustomerCategoryLayer()
{
    
}

void PixMenuCustomerCategoryLayer::initLayer()
{
    
    rootNode = CSLoader::createNode("11_Menu/MenuCustomerPop.csb");
    
    if(rootNode)
    {
        Panel_Category = dynamic_cast<ui::Layout*>(rootNode->getChildByName("Panel_Category"));
        
        if(Panel_Category)
        {
            ui::ListView *listView = dynamic_cast<ui::ListView*>(Panel_Category->getChildByName("ListView"));
            
            if(listView)
            {
                CategoryListView = PixGalleryListView::create();
                CategoryListView->copyClonedListView(listView);
             
                for(auto iter = mFaqCategoryInfoVec->begin(); iter != mFaqCategoryInfoVec->end(); ++iter)
                {
                    PixMenuCustomerCategoryCell *cell = PixMenuCustomerCategoryCell::create((*iter));
                    cell->setDelegate(this);
                    CategoryListView->pushBackCustomItem(cell);
                }
            }
        }
        
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2::ZERO);
        addChild(rootNode);
    }
}

void PixMenuCustomerCategoryLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
}

void PixMenuCustomerCategoryLayer::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Delegate

void PixMenuCustomerCategoryLayer::categoryPressed(FaqCategoryInfo *pFaqCategoryInfo)
{
    if (mDelegate)
    {
        mDelegate->customerCategoryPressed(pFaqCategoryInfo);
    }
    
    hideLayerAction();
}

#pragma mark - Touch & Touches

void PixMenuCustomerCategoryLayer::onTouchEnded(Touch* touch, Event* event)
{
    if(Panel_Category)
    {
        if(NodeContainsPoint(this, touch->getLocation()))
        {
            if(!NodeContainsPoint(Panel_Category, touch->getLocation()))
            {
                if(getParent())
                {
                    this->removeFromParentAndCleanup(true);
                }
            }
        }
    }
}

#pragma mark - Android BackPressed

void PixMenuCustomerCategoryLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}
