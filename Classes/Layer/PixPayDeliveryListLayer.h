//
//  PixPayDeliveryListLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 23..
//
//

#ifndef PixPayDeliveryListLayer_h
#define PixPayDeliveryListLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Widget/PixPayDeliveryCell.h"

class DeliveryInfo;
class PixPayDeliveryListLayerDelegate;

class PixPayDeliveryListLayer : public PixModalLayer, public PixPayDeliveryCellDelegate
{
    
public:
    
    static PixPayDeliveryListLayer* create(std::vector<DeliveryInfo*> *pDeliveryVec);
    
    PixPayDeliveryListLayer(std::vector<DeliveryInfo*> *pDeliveryVec);
    virtual ~PixPayDeliveryListLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private :
    
    Node *rootNode;
    
    PixGalleryListView *deliveryListView;
    std::vector<DeliveryInfo*> *mDeliveryVec;
    
    CC_SYNTHESIZE(PixPayDeliveryListLayerDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE(bool, mbln_IsAction, IsAction)
    
    void createDeliveryListView();
    void hideLayerAction();
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Delegate
    
    virtual void payDeliveryCellcheckBoxPressed(DeliveryInfo *pInfo);
    virtual void payDeliveryCellDeleteActionStart(Ref *pTarget);
    virtual void payDeliveryCellDeleteActionFinish(Ref *pTarget);
    virtual void payDeliveryCellNoAddressAction(Ref *pTarget);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
    
};

class PixPayDeliveryListLayerDelegate
{
public:
    
    virtual void payDeliveryListLayerCompletePressed(DeliveryInfo *pInfo) {};
    virtual void payDeliveryListLayerNoAddressAction() {};
};




#endif /* PixPayDeliveryListLayer_h */
