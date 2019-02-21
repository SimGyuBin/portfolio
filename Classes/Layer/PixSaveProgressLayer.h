//
//  PixSaveProgressLayer.h
//  Pzle
//
//  Created by 심규빈 on 2017. 1. 19..
//
//

#ifndef PixSaveProgressLayer_h
#define PixSaveProgressLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"

USING_NS_CC;

class PixSaveProgressLayer : public PixModalLayer
{
    
public:
    
    static PixSaveProgressLayer* create(std::function<void(Ref*,ui::Widget::TouchEventType)> saveCancelCallback);
    
    PixSaveProgressLayer(std::function<void(Ref*,ui::Widget::TouchEventType)> saveCancelCallback);
    virtual ~PixSaveProgressLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    
    void updateImageUpload(const std::string &strMessage, float percent);
    void updatePreviewUpload(const std::string &strMessage, float percent);
    void resetUpload();
    void hideLayerAction();
    
private:
 
    Node *rootNode;
    ui::Text *upload_text;
    Node *loading;

    ui::LoadingBar *imageUploadLoadingBar;
    ui::Text *imageUploadPercentLabel;
    ui::Text *imageUploadLabel;
    
    ui::LoadingBar *previewUploadLoadBar;
    ui::Text *previewUploadPercentLabel;
    ui::Text* previewUploadLabel;
    
    std::function<void(Ref*,ui::Widget::TouchEventType)> _saveCancelCallback;
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};





#endif /* PixSaveProgressLayer_h */
