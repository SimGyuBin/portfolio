//
//  PixAndroidWebViewLaver.h
//  Pzle
//
//  Created by 심규빈 on 2017. 2. 23..
//
//

#ifndef PixAndroidWebViewLaver_h
#define PixAndroidWebViewLaver_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixAndroidWebViewLaver : public PixModalLayer
{
public :
  
    static PixAndroidWebViewLaver* create(Node *pWebView, bool blur);
    
    PixAndroidWebViewLaver(Node *pWebView, bool blur);
    virtual ~PixAndroidWebViewLaver();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private :
    
    Sprite *blur;
    Texture2D *blurTexture;
    Image *captureImage;
    Node *webView;
    Node *rootNode;
    ui::Layout *Top;
    ui::Layout *Web;
    
    bool mbln_Blur;
    
    void threadStack_blur_rgba32(Image *pImage);
    void createblur();
    void addWebView();
    
#pragma mark - Button

    void btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixAndroidWebViewLaver_h */
