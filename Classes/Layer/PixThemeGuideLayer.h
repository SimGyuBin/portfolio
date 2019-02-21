//
//  PixThemeGuideLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 11..
//
//

#ifndef PixThemeGuideLayer_h
#define PixThemeGuideLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;

class PixThemeGuideLayer : public PixModalLayer
{
public:
    
    static PixThemeGuideLayer* create();
    
    PixThemeGuideLayer();
    virtual ~PixThemeGuideLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    Node *rootNode;
    
    void hideLayerAction();
    
#pragma mark - Button
    
    void buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();    
};

#endif /* PixThemeGuideLayer_h */
