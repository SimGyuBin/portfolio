//
//  PixPushToast.h
//  Pzle
//
//  Created by 심규빈 on 2017. 7. 7..
//
//

#ifndef PixPushToast_h
#define PixPushToast_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;

#define PUSH_TOAST_TAG           593

class PixPushToast : public Layer
{
    
public :
    
    static PixPushToast* showToast(const std::string &message);
    
    PixPushToast();
    virtual ~PixPushToast();
    
    void initPushToast(const std::string &message);
    virtual void onEnterTransitionDidFinish();
    
private :
    
    Node *rootNode;
    ui::ImageView *Image_background_push_toast;
    Vec2 startPos;
    Vec2 endPos;
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Message, Message)
    
    void showPushToastAction();
    void showPushToastActionFinish(Node* node);
    
};

#endif /* PixPushToast_h */
