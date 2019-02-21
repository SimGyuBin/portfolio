//
//  PixPurchaseCompletedInfoCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 5..
//
//

#ifndef PixPurchaseCompletedInfoCell_h
#define PixPurchaseCompletedInfoCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixPurchaseCompletedInfoCell : public ui::Widget
{
public:
    
    static PixPurchaseCompletedInfoCell* create(int nAmount, const std::string& strPay_method, const std::string& strPay_method_text, const std::string& strVbank_name, const std::string& strVbank_num, const std::string& strVbank_holder, const std::string& strVbank_date);
    
    PixPurchaseCompletedInfoCell(int nAmount, const std::string& strPay_method, const std::string& strPay_method_text, const std::string& strVbank_name, const std::string& strVbank_num, const std::string& strVbank_holder, const std::string& strVbank_date);
    virtual ~PixPurchaseCompletedInfoCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
    
    CC_SYNTHESIZE(int, mn_Amount, Amount)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Pay_method, Pay_method)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Pay_method_text, Pay_method_text)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Vbank_name, Vbank_name)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Vbank_num, Vbank_num)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Vbank_holder, Vbank_holder)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Vbank_date, Vbank_date)
    
    Node* createCell();
    Node* createVbankCell();
};


#endif /* PixPurchaseCompletedInfoCell_h */
