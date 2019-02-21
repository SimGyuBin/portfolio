//
//  PixDeliveryTrackingLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 18..
//
//

#ifndef PixDeliveryTrackingLayer_h
#define PixDeliveryTrackingLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "../UIControl/PixModalLayer.h"
#include "PixLayerActionDelegate.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class OrderCheckInfo;

class PixDeliveryTrackingLayer : public PixModalLayer, public PixLayerActionDelegate
{
public:
  
    static PixDeliveryTrackingLayer* create(OrderCheckInfo *pOrderCheckInfo);
    
    PixDeliveryTrackingLayer(OrderCheckInfo *pOrderCheckInfo);
    virtual ~PixDeliveryTrackingLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node *rootNode;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(OrderCheckInfo*, mOrderCheckInfo, OrderCheckInfo)
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Action
    
    void showDeliveryTrackingLayerAction();
    void hideDeliveryTrackingLayerAction();
    void showDeliveryTrackingLayerActionFinish(Node* node);
    void hideDeliveryTrackingLayerActionFinish(Node* node);
    
#pragma mark - Android BackPressed
    
    void androidBackPressed();
};

#endif /* PixDeliveryTrackingLayer_h */
