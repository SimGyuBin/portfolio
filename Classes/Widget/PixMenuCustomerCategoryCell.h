//
//  PixMenuCustomerCategoryCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 28..
//
//

#ifndef PixMenuCustomerCategoryCell_h
#define PixMenuCustomerCategoryCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"

USING_NS_CC;

class FaqCategoryInfo;
class PixMenuCustomerCategoryCellDelegate;

class PixMenuCustomerCategoryCell : public ui::Widget
{
public:
    
    static PixMenuCustomerCategoryCell* create(FaqCategoryInfo *pFaqCategoryInfo);
    
    PixMenuCustomerCategoryCell(FaqCategoryInfo *pFaqCategoryInfo);
    virtual ~PixMenuCustomerCategoryCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();

private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE(FaqCategoryInfo*, mFaqCategoryInfo, FaqCategoryInfo)
    CC_SYNTHESIZE(PixMenuCustomerCategoryCellDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void buttonCategoryPressed(cocos2d::Ref *pSender);
    
};

class PixMenuCustomerCategoryCellDelegate
{
public :
    
    virtual void categoryPressed(FaqCategoryInfo *pFaqCategoryInfo) {};
};


#endif /* PixMenuCustomerCategoryCell_h */
