//
//  PixBankItemCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 19..
//
//

#ifndef PixBankItemCell_h
#define PixBankItemCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixBankItemCell : public ui::Widget
{
public:
    
    static PixBankItemCell* create(const std::string& strBank_name, const std::string& strBank_code);
    
    PixBankItemCell(const std::string& strBank_name, const std::string& strBank_code);
    virtual ~PixBankItemCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    void setCheckVisible(bool bVisible);
    bool isCheckVisible();
    
private:
    
    Node *_customItem;
    ui::ImageView *Image_Check_Bank;
    ui::ImageView *Image_Check_Bank_on;
    ui::Text *Text_Bank;
    
    bool mbln_CheckVisible;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Bank_name, Bank_name)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Bank_code, Bank_code)
};

#endif /* PixBankItemCell_h */
