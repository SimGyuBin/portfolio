//
//  PixAlert.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 22..
//
//

#ifndef PixAlert_h
#define PixAlert_h

#include "cocos2d.h"
#include "PixModalLayer.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

#define ALERT_TAG           2605

// 알트창에 어떤 버튼들이 들어가는가
typedef enum
{
    ALERT_TYPE_OK,
    ALERT_TYPE_CANCEL,
    ALERT_TYPE_ALL,
}
ALERT_TYPE;


// 알트창에 존재하는 각 버튼의 타입
typedef enum
{
    ALERT_BUTTON_OK,
    ALERT_BUTTON_CANCEL,
}
ALERT_BUTTON_TYPE;

class PixAlertDelegate;

class PixAlert : public PixModalLayer
{
public:
    static PixAlert* show(const char* message, PixAlertDelegate *delegate, ALERT_TYPE type,
                         int tag = 0, Ref *target = nullptr, SEL_CallFunc selector = nullptr, bool isCaptureImage = true);
    
    static PixAlert* show(const char* title, const char* message, PixAlertDelegate *delegate, ALERT_TYPE type,
                         int tag = ALERT_TAG, Ref *target = nullptr, SEL_CallFunc selector = nullptr, bool isCaptureImage = true);
    
    PixAlert();
    virtual ~PixAlert();
    
protected:
    
    virtual void init(const char* title, const char* message, PixAlertDelegate *delegate, ALERT_TYPE type,
                      Ref *&target, SEL_CallFunc &selector, bool isCaptureImage);
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();

private:
    
    PixAlertDelegate *delegate;
    Ref *target;
    SEL_CallFunc selector;
  
    Node *alert;
    Sprite *blockout;
    Sprite *blur;
    Texture2D *blurTexture;
    Sprite *Background_popup;
        
    Image *captureImage;
    bool mbln_IsAction;
    bool mbln_CaptureImage;
    
    void threadStack_blur_rgba32(Image *pImage);
    void createblur();
    void hideLayerAction();
    
#pragma mark - Button
    
    void buttonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Action
    
    void showAlertAction();
    void showAlertActionFinish(Node* node);
};

// 알트창으로부터 버튼선택 콜백을 받으려면, KAAlertDelegate 상속받고 아래 메소드를 통해 콜백받을 수 있음
class PixAlertDelegate
{
public:
    virtual void clickedButtonInAlert(PixAlert *alert, ALERT_BUTTON_TYPE buttonType) {};
};



#endif /* PixAlert_h */
