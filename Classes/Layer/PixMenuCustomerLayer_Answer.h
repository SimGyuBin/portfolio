//
//  PixMenuCustomerLayer_Answer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#ifndef PixMenuCustomerLayer_Answer_h
#define PixMenuCustomerLayer_Answer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "PixLayerActionDelegate.h"
#include "../Info/FaqInfo.h"

USING_NS_CC;

class PixMenuCustomerLayer_Answer : public PixModalLayer
{
public:
    
    static PixMenuCustomerLayer_Answer* create(FaqInfo *pInfo);
    
    PixMenuCustomerLayer_Answer(FaqInfo *pInfo);
    virtual ~PixMenuCustomerLayer_Answer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node *rootNode;
    Node *MenuCustomerAnswer_Top;
    ui::Button *Button_back;
    
    ui::ScrollView *scrollView;
    ui::Text *Text_question;
    ui::Text *Text_Answer;
    
    FaqInfo *mInfo;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void ButtonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};


#endif /* PixMenuCustomerLayer_Answer_h */
