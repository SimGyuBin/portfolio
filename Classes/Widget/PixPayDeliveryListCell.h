//
//  PixPayDeliveryListCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 23..
//
//

#ifndef PixPayDeliveryListCell_h
#define PixPayDeliveryListCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"
#include "../UIControl/PixEditBox.h"

USING_NS_CC;

class DeliveryInfo;
class PixPayDeliveryListCellDelegate;

class PixPayDeliveryListCell : public ui::Widget, public ui::EditBoxDelegate
{
public:
    
    static PixPayDeliveryListCell *create();
    
    PixPayDeliveryListCell();
    virtual ~PixPayDeliveryListCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    
    void createCustomItem();
    const char* getMemo();
    
private:
    
    Node *_customItem;
    Node *Node_MenuPayItem_06;
    Node *Node_MenuPayItem_04_2;
    Node *Node_MenuPayItem_07;
    PixScrollMenu *Button_More;
    ui::Text *Text_Name;
    ui::Text *Text_Phone;
    ui::Text *Text_Address;
    ui::Text *Text_Default;
    PixEditBox *memo;
    
    CC_SYNTHESIZE(DeliveryInfo*, mDeliveryInfo, DeliveryInfo)
    CC_SYNTHESIZE(PixPayDeliveryListCellDelegate*, mDelegate, Delegate)
    
    void updateDelivery();
   
#pragma mark - Button
    
    void ButtonMoerPressed(cocos2d::Ref *pSender);
    
#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox) override;
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(ui::EditBox* editBox) override;
};

class PixPayDeliveryListCellDelegate
{
public:
    
    virtual void payDeliveryListCellMoerPressed() {};
};


#endif /* PixPayDeliveryListCell_h */
