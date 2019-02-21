//
//  PixMenuCustomerLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 13..
//
//

#ifndef PixMenuCustomerLayer_h
#define PixMenuCustomerLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "PixLayerActionDelegate.h"
#include "PixMenuCustomerCategoryLayer.h"
#include "../Info/FaqInfo.h"

USING_NS_CC;

class PixMenuCustomerLayer : public PixModalLayer, public PixLayerActionDelegate, public PixMenuCustomerCategoryLayerDelegate
{
public:
    
    static PixMenuCustomerLayer* create();
    
    PixMenuCustomerLayer();
    virtual ~PixMenuCustomerLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    PixHttpClient *pixHttpClient;
    Node  *rootNode;
    Node *MenuCustomer_Top;
    ui::Button *Button_cancel;
    PixGalleryListView *FAQlistView;
    ui::Text *Text_Category;
    
    std::vector<FaqInfo*> faqInfoVec;
    std::vector<FaqCategoryInfo*> faqCategoryInfoVec;
    
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE(int, mn_Kind, Kind)
    
    void createFAQlistView();
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonCategoryPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - ListView Event
    
    void listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType);
    
#pragma mark - Action
    
    void showMenuCustomerLayerAction();
    void hideMenuCustomerLayerAction();
    void showMenuCustomerLayerActionFinish(Node* node);
    void hideMenuCustomerLayerActionFinish(Node* node);
    
#pragma mark - Delegate
    
    virtual void showLayerActionStart(Ref *pTarget);
    virtual void showLayerActionFinish(Ref *pTarget);
    virtual void hideLayerActionStart(Ref *pTarget);
    virtual void hideLayerActionFinish(Ref *pTarget);
    virtual void customerCategoryPressed(FaqCategoryInfo *pFaqCategoryInfo);

#pragma mark - Network
    
    void boardFaqCall();
    void boardFaqCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixMenuCustomerLayer_h */
