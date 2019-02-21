//
//  PixMenuCustomerCategoryLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 27..
//
//

#ifndef PixMenuCustomerCategoryLayer_h
#define PixMenuCustomerCategoryLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Widget/PixMenuCustomerCategoryCell.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"


class FaqCategoryInfo;
class PixMenuCustomerCategoryLayerDelegate;

class PixMenuCustomerCategoryLayer : public PixModalLayer, public PixMenuCustomerCategoryCellDelegate
{
    
public :
    
    static PixMenuCustomerCategoryLayer* create(std::vector<FaqCategoryInfo*> *pFaqCategoryInfoVec);
    
    PixMenuCustomerCategoryLayer(std::vector<FaqCategoryInfo*> *pFaqCategoryInfoVec);
    virtual ~PixMenuCustomerCategoryLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private :
    
    Node *rootNode;
    Node *Panel_Category;
    PixGalleryListView *CategoryListView;
    
    std::vector<FaqCategoryInfo*> *mFaqCategoryInfoVec;
    
    CC_SYNTHESIZE(PixMenuCustomerCategoryLayerDelegate*, mDelegate, Delegate)
    
    void hideLayerAction();
    
#pragma mark - Delegate
    
    virtual void categoryPressed(FaqCategoryInfo *pFaqCategoryInfo);
    
#pragma mark - Touch & Touches
    
    virtual void onTouchEnded(Touch* touch, Event* event);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
    
};

class PixMenuCustomerCategoryLayerDelegate
{
public :
    
    virtual void customerCategoryPressed(FaqCategoryInfo *pFaqCategoryInfo) {};
};


#endif /* PixMenuCustomerCategoryLayer_h */
