//
//  PixLoginPersonalInfoLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 18..
//
//

#ifndef PixLoginPersonalInfoLayer_h
#define PixLoginPersonalInfoLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixLoginPersonalInfoLayer : public PixModalLayer
{
public:
    
    static PixLoginPersonalInfoLayer* create();
    
    PixLoginPersonalInfoLayer();
    virtual ~PixLoginPersonalInfoLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node  *rootNode;
    ui::Button *btn_back;
    Node *mWebView;
    
    bool mbln_IsAction;
    
#pragma mark - Button
    
    void btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixLoginPersonalInfoLayer_h */
