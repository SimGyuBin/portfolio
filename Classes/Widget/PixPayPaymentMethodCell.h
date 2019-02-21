//
//  PixPayPaymentMethodCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 24..
//
//

#ifndef PixPayPaymentMethodCell_h
#define PixPayPaymentMethodCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixPayPaymentMethodCell : public ui::Widget
{
public:
    
    static PixPayPaymentMethodCell* create();
    
    PixPayPaymentMethodCell();
    virtual ~PixPayPaymentMethodCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
    bool isCreditCardSelected();
    bool isPhoneSelected();
    bool isNoBankbookSelected();
    bool isRealTimeBankSelected();
    const char *getPay_method();
    
private:
    
    Node *_customItem;

    ui::CheckBox *creditCard;
    ui::CheckBox *phone;
    ui::CheckBox *noBankbook;
    ui::CheckBox *realTimeBank;
    
    std::vector<ui::CheckBox*> checkBoxVec;
    
    void updateCheckBox(ui::CheckBox *pCheckBox);

#pragma mark - Button
    
    void creditCardPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    void phonePressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    void noBankbookPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    void realTimeBankPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
};

#endif /* PixPayPaymentMethodCell_h */
