//
//  PixAddPhotoSelectLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 20..
//
//

#ifndef PixAddPhotoSelectLayer_h
#define PixAddPhotoSelectLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixModalLayer.h"
#include "../Info/LocalImageDateInfo.h"
#include "PixLayerActionDelegate.h"

USING_NS_CC;

class LocalImageInfo;
class PixImageModel;

class PixAddPhotoSelectLayer : public PixModalLayer
{
public:
    
    static PixAddPhotoSelectLayer* create();
    
    PixAddPhotoSelectLayer();
    virtual ~PixAddPhotoSelectLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    virtual void onExit();
    
private:
    
    Node *rootNode;
    
    ui::Button *Button_complete;
    ui::Button *Button_back;
    PixGalleryListView *selectListView;
    
    bool mbln_IsAction;
    
    std::vector<PixImageModel*> selectImageModelVec;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE(PixImageModel*, mSelectModel, SelectModel)
    CC_SYNTHESIZE(int, mn_stepByStepIndex, StepByStepIndex)
    CC_SYNTHESIZE(int, mn_LocalImageCount, LocalImageCount)
    
    void settingStepByStepSelectScrollView(float dt = 0);
    void settingSelectScrollView(std::vector<LocalImageInfo*> &localImageInfoVec);
  
    bool isLocalImageLoading();
    
#pragma mark - Button
    
    void buttonCompletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

#pragma mark - Touch

    void listViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};

#endif /* PixAddPhotoSelectLayer_h */
