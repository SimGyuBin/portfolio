//
//  PixPhotoInsertLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 1..
//
//

#ifndef PixPhotoInsertLayer_h
#define PixPhotoInsertLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Model/PixSmartMaskBookModel.h"
#include "../UIControl/PixEditSprite.h"

class PixPhotoInsertLayerDelegate;

class PixPhotoInsertLayer : public Layer
{
public:
    
    static PixPhotoInsertLayer* create(PixEditSprite *pSprite);
    
    PixPhotoInsertLayer(PixEditSprite *pSprite);
    virtual ~PixPhotoInsertLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
    Node *rootNode;
    ui::Button *Button_putphoto;
    
    PixEditSprite *editSprite;
    
    CC_SYNTHESIZE(PixPhotoInsertLayerDelegate*, mDelegate, Delegate)

#pragma mark - Button
    
    void btnInsertPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

class PixPhotoInsertLayerDelegate
{
public:
    virtual void photoInsertLayerPressed(PixEditSprite* pSprite) {};
};


#endif /* PixPhotoInsertLayer_h */
