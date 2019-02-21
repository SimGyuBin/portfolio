//
//  PixSmartalbumIntro.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 6..
//
//

#ifndef PixSmartalbumIntro_h
#define PixSmartalbumIntro_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixSmartalbumIntro : public PixModalLayer
{
public:
    
    static PixSmartalbumIntro* create();
    
    PixSmartalbumIntro();
    virtual ~PixSmartalbumIntro();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    
private:
    
    Node *rootNode;
    ui::PageView *pageView;
    ui::Layout *layout;
    ui::Layout *bottom;
    ui::Button *close;
    ui::CheckBox *checkBox;
    ui::ImageView *Image_icon_next;
    int curPage;
    bool mbln_isLastPage;
    
    void setPageMarker(int nPage);
    void showCompletealbumFinish(Node* node);
    void hideLayerAction();
    
#pragma mark - Button
    
    void buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
    
#pragma mark - Touch & Touches
    
    virtual void onTouchEnded(Touch* touch, Event* event);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixSmartalbumIntro_h */
