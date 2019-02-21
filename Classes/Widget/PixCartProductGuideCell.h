//
//  PixCartProductGuideCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 10..
//
//

#ifndef PixCartProductGuideCell_h
#define PixCartProductGuideCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"

USING_NS_CC;

class PixCartProductGuideCell : public ui::Widget
{
public:
    
    static PixCartProductGuideCell* create();
    
    PixCartProductGuideCell();
    virtual ~PixCartProductGuideCell();
    
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
#pragma mark - Button
    
    void buttonProductGuidPressed(cocos2d::Ref *pSender);

};


#endif /* PixCartProductGuideCell_h */
