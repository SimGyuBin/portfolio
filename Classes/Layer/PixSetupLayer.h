//
//  PixSetupLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 27..
//
//

#ifndef PixSetupLayer_h
#define PixSetupLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixSwitchButton.h"
#include "../Network/PixHttpClient.h"

USING_NS_CC;

class PixSetupLayer : public Layer, public PixSwitchDelegate
{
public:
    
    static PixSetupLayer *create();
    
    PixSetupLayer();
    virtual ~PixSetupLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    PixHttpClient *pixHttpClient;
    
    Node *rootNode;
    
    Sprite *proflie;
    ui::Text *user_name;
    
    ui::Button *btn_member;
    ui::Button *btn_cart;
    ui::Button *btn_order;
    ui::Button *btn_millige;
    ui::Button *btn_infor;
    ui::Button *btn_center;
    ui::Button *btn_logout;
    ui::Button *btn_out;
    
    PixSwitchButton *AlamBotton;
    ui::Text *text_version;
    
#pragma mark - Button
    
    void btnMemberPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnCartPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnOrderPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnMilligePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnInforPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnCenterPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnLogoutPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnOutPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Delegate
    
    virtual void changeSwitch(PixSwitchButton *switchButton, bool isActive);
    
#pragma mark - Network
    
    void onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};


#endif /* PixSetupLayer_h */
