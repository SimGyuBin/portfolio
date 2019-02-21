//
//  PixPurchaseFailedLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 4..
//
//

#ifndef PixPurchaseFailedLayer_h
#define PixPurchaseFailedLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Widget/PixPurchaseFailedTopCell.h"
#include "../Widget/PixPurchaseFailedBottomCell.h"

USING_NS_CC;

class PixPurchaseFailedLayer : public PixModalLayer
{
public:
    
    static PixPurchaseFailedLayer* create(const std::string &strFail_reason);
    
    PixPurchaseFailedLayer(const std::string &strFail_reason);
    virtual ~PixPurchaseFailedLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node *rootNode;
    PixGalleryListView *failedLayerListView;
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Fail_reason, Fail_reason)
    
    void createFailedLayerListView();
    void hideLayerAction();
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
};

#endif /* PixPurchaseFailedLayer_h */
