//
//  PixMenuMemberNameLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#ifndef PixMenuMemberNameLayer_h
#define PixMenuMemberNameLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Network/PixHttpClient.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixEditBox.h"
#include "PixLayerActionDelegate.h"

USING_NS_CC;

class PixMenuMemberNameLayer : public PixModalLayer, public ui::EditBoxDelegate
{
public:
    
    static PixMenuMemberNameLayer* create();
    
    PixMenuMemberNameLayer();
    virtual ~PixMenuMemberNameLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
private:
    
    Node  *rootNode;
    PixHttpClient *pixHttpClient;
    
    Node *MenuMemberName_Top;
    ui::Button *Button_back;
    ui::Button *Button_ok;
    
    PixEditBox *name;
    ui::Button *btn_X;
    
    Sprite *Background_name;
    Sprite *Background_name_on;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonOkPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonNamebtnXPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(ui::EditBox* editBox);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Network
    
    void member_chgName();
    void member_chgNameCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};


#endif /* PixMenuMemberNameLayer_h */
