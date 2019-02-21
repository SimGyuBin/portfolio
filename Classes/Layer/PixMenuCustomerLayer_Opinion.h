//
//  PixMenuCustomerLayer_Opinion.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#ifndef PixMenuCustomerLayer_Opinion_h
#define PixMenuCustomerLayer_Opinion_h

#include <stdio.h>
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Network/PixHttpClient.h"

USING_NS_CC;

class PixMenuCustomerLayer_Opinion : public Layer
{
public:
    
    static PixMenuCustomerLayer_Opinion *create();
    
    PixMenuCustomerLayer_Opinion();
    virtual ~PixMenuCustomerLayer_Opinion();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
private:
    
    PixHttpClient *pixHttpClient;
    
    Node *rootNode;
    
    Sprite *Background_email;
    ui::Text *Text_email;
    
    Sprite *Background_name;
    ui::Text *Text_name;
    
    ui::TextField *Text_writing;
    
    ui::Button *Button_SendOpinion;
    
#pragma mark - Button
    
    void buttonSendOpinionPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Network
    
    void onHttpManagerRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};


#endif /* PixMenuCustomerLayer_Opinion_h */
