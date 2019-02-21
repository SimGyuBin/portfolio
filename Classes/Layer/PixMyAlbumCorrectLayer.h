//
//  PixMyAlbumCorrectLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 21..
//
//

#ifndef PixMyAlbumCorrectLayer_h
#define PixMyAlbumCorrectLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixEditBox.h"
#include "PixLayerActionDelegate.h"
#include "../Network/PixHttpClient.h"

USING_NS_CC;

class MyAlbumModel;

class PixMyAlbumCorrectLayer : public PixModalLayer, public ui::EditBoxDelegate
{
    
public:
    
    static PixMyAlbumCorrectLayer* create(MyAlbumModel *pModel);
    
    PixMyAlbumCorrectLayer(MyAlbumModel *pModel);
    virtual ~PixMyAlbumCorrectLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float delta);
    
private:
    
    PixHttpClient *pixHttpClient;
    
    Node *rootNode;
    PixEditBox *title;
    
    Node *MyAlbumCorrectTopLayer;
    ui::Button *btn_back;
    ui::Button *Button_Save;
    
    Sprite *Background_title;
    Sprite *Background_title_on;
    
    bool mbln_IsAction;
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Title, Title)
    CC_SYNTHESIZE(MyAlbumModel*, mMyAlbumModel, MyAlbumModel)
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)

#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    
#pragma mark - Button
    
    void buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonSavePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - EditBox
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(ui::EditBox* editBox);

#pragma mark - Network

    void albumModify(const std::string &strRename);
    void albumModifyCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
    
};

#endif /* PixMyAlbumCorrectLayer_h */

