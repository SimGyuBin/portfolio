//
//  PixSwitchButton.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 27..
//
//

#ifndef PixSwitchButton_h
#define PixSwitchButton_h

#include "cocos2d.h"

USING_NS_CC;

#define PIXSWITCH_ANI_SPEED    0.2f   // 애니메이션 스피드

class PixSwitchDelegate;

class PixSwitchButton : public ClippingNode {
    
public:
    static PixSwitchButton* create(const char *leftImageName, const char *rightImageName,
                                   const char *maskImageName, const char *thumbImageName,
                                   float space, bool isActive);
    PixSwitchButton();
    virtual ~PixSwitchButton();
    
    void setDelegate(PixSwitchDelegate *delegate);
    void removeDelegate(PixSwitchDelegate *delegate);
    
    void setValue(bool value, bool isAnimate);
    bool getValue();
    
    
private:
    void init(const char *leftImageName, const char *rightImageName,
              const char *maskImageName, const char *thumbImageName,
              float space, bool isActive);
    
    void moveImage(Vec2 addPosition);
    void goFixedPosition(bool isAnimate);
    void autoMove(float dt);
    float getImagePositionX();
    
    virtual void onEnter();
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);
    
private:
    
    EventListenerTouchOneByOne *mListener;
    
    std::vector<PixSwitchDelegate*> arr_Delegate;
    bool m_isActive;
    
    Sprite *mask;
    Sprite *leftImage;
    Sprite *rightImage;
    Sprite *thumbImage;
    
    Size maskSize;
    float thumbHalfWidth;
    float mf_MoveX;
    float mf_Sspace;
    bool isTab;
};

class PixSwitchDelegate {
public:
    virtual void changeSwitch(PixSwitchButton *switchButton, bool isActive) {};
};

#endif /* PixSwitchButton_h */
