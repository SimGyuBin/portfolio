//
//  PixPayNewDeliveryCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 20..
//
//

#ifndef PixPayNewDeliveryCell_h
#define PixPayNewDeliveryCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"
#include "../UIControl/PixEditBox.h"
#include "../Layer/PixSearchAddressLayer.h"
#include "../Network/PixHttpClient.h"

class PixPayNewDeliveryCell : public ui::Widget, public ui::EditBoxDelegate, public PixSearchAddressLayerDelegate
{
public:
    
    static PixPayNewDeliveryCell* create();
    
    PixPayNewDeliveryCell();
    virtual ~PixPayNewDeliveryCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    const char* getName();
    const char* getPhoneNumber();
    const char* getZipCode();
    const char* getAddress();
    const char* getDetailedAddress();
    const char* getMemo();
    
private:
    
    Node *_customItem;
    PixHttpClient *pixHttpClient;
    
    PixEditBox *name;
    PixEditBox *phoneNumber;
    PixEditBox *detailedAddress;
    PixEditBox *memo;
    
    ui::Text *text_ZipCode;
    PixScrollMenu *button_Search;
    
    ui::Text *text_Address;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Post, Post)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Old_post, Old_post)
    CC_SYNTHESIZE(int, mn_Backwoods_list_id, Backwoods_list_id)
    CC_SYNTHESIZE(int, mn_Pay_add_ship_amount, Pay_add_ship_amount)
        
#pragma mark - Button
    
    void buttonSearchPressed(cocos2d::Ref *pSender);
    
#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox) override;
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox) override;
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text) override;
    virtual void editBoxReturn(ui::EditBox* editBox) override;

#pragma mark - Delegate
    
    virtual void searchAddressCompile(const std::string& str) override;
    
#pragma mark - Network
    
    void getAddShipPriceCall();
    void getAddShipPriceCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};

#endif /* PixPayNewDeliveryCell_h */
