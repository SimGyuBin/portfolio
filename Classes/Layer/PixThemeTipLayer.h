//
//  PixThemeTipLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 28..
//
//

#ifndef PixThemeTipLayer_h
#define PixThemeTipLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixThemeTipLayer : public PixModalLayer
{
    
public :
    
    static PixThemeTipLayer* create();
    
    PixThemeTipLayer();
    virtual ~PixThemeTipLayer();
    
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

#endif /* PixThemeTipLayer_h */
