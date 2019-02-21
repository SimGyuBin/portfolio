//
//  PixOrderCancelLayer.hpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 18..
//
//

#ifndef PixOrderCancelLayer_hpp
#define PixOrderCancelLayer_hpp

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixEditBox.h"
#include "../UIControl/PixGalleryListView.h"
#include "PixLayerActionDelegate.h"
#include "../Widget/PixBankItemCell.h"

USING_NS_CC;

class OrderCheckInfo;
class PixOrderCancelLayerDelegate;

class PixOrderCancelLayer : public PixModalLayer, public PixLayerActionDelegate, public ui::EditBoxDelegate
{
public:
    
    static PixOrderCancelLayer* create(OrderCheckInfo *pOrderCheckInfo);
    
    PixOrderCancelLayer(OrderCheckInfo *pOrderCheckInfo);
    virtual ~PixOrderCancelLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    
    PixEditBox *Name;
    ui::Text *Text_Bank;
    PixEditBox *Number;
    ui::Text *Pay;
    
    ui::Button *Button_Ok;
    
    ui::Layout *Panel_Bank;
    PixGalleryListView *listView;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(OrderCheckInfo*, mOrderCheckInfo, OrderCheckInfo)
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE(PixOrderCancelLayerDelegate*, mOrderCancelLayerDelegate, OrderCancelLayerDelegate)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Bank_code, Bank_code)
    
    void createListView();
    bool checkOrderCancellation();
    const char* getName();
    const char* getNumber();
    void setPanelBankVisible(bool bVisible);
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonBankPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Touch
    
    void listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType);
    
#pragma mark - Touch & Touches
    
    virtual void onTouchEnded(Touch* touch, Event* event);
    
#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(ui::EditBox* editBox);
    
#pragma mark - Action
    
    void showOrderCancelLayerAction();
    void hideOrderCancelLayerAction();
    void showOrderCancelLayerActionFinish(Node* node);
    void hideOrderCancelLayerActionFinish(Node* node);
    
#pragma mark - Android BackPressed
    
    void androidBackPressed();
};

class PixOrderCancelLayerDelegate
{
public :
    
    virtual void orderCancellation(const std::string &strBank_num, const std::string &strBank_code, const std::string &strBank_holder) {};
};

#endif /* PixOrderCancelLayer_hpp */
