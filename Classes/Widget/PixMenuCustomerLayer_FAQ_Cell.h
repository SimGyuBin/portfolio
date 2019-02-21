//
//  PixMenuCustomerLayer_FAQ_Cell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#ifndef PixMenuCustomerLayer_FAQ_Cell_h
#define PixMenuCustomerLayer_FAQ_Cell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"
#include "../Layer/PixLayerActionDelegate.h"
#include "../Info/FaqInfo.h"

USING_NS_CC;

class PixMenuCustomerLayer;

class PixMenuCustomerLayer_FAQ_Cell : public ui::Widget
{
    
public :
    
    static PixMenuCustomerLayer_FAQ_Cell* create(FaqInfo *pFaqInfo);
    
    PixMenuCustomerLayer_FAQ_Cell(FaqInfo *pFaqInfo);
    virtual ~PixMenuCustomerLayer_FAQ_Cell();
    
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    PixScrollMenu *menu;
    ui::Text *Text_question;
    
    CC_SYNTHESIZE(PixMenuCustomerLayer*, mMenuCustomerLayer, MenuCustomerLayer)
    CC_SYNTHESIZE(FaqInfo*, mFaqInfo, FaqInfo)
    
#pragma mark - Button
    
    void ButtonFAQCellPressed(cocos2d::Ref *pSender);
};

#endif /* PixMenuCustomerLayer_FAQ_Cell_h */
