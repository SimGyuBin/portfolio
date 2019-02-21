//
//  PixPhotoGuideLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 11..
//
//

#ifndef PixPhotoGuideLayer_h
#define PixPhotoGuideLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;

class PixPhotoGuideLayer : public PixModalLayer
{
public:
    
    static PixPhotoGuideLayer* create();
    
    PixPhotoGuideLayer();
    virtual ~PixPhotoGuideLayer();
    
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

#endif /* PixPhotoGuideLayer_h */
