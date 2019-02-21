//
//  PixPurchaseAgreementLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 22..
//
//

#ifndef PixPurchaseAgreementLayer_h
#define PixPurchaseAgreementLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

class PixPurchaseAgreementLayer : public PixModalLayer
{
    
public:
    
    static PixPurchaseAgreementLayer* create();
    
    PixPurchaseAgreementLayer();
    virtual ~PixPurchaseAgreementLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private :
    
    Node *rootNode;
    
    void hideLayerAction();
    
#pragma mark - Button
    
    void buttonCancelPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
    
};

#endif /* PixPurchaseAgreementLayer_h */
