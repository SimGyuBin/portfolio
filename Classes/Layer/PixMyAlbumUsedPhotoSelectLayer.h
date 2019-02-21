//
//  PixMyAlbumUsedPhotoSelectLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 27..
//
//

#ifndef PixMyAlbumUsedPhotoSelectLayer_h
#define PixMyAlbumUsedPhotoSelectLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixPageView.h"
#include "../UIControl/PixScrollMenu.h"
#include "../Widget/PixUsedPhotoSelectCell.h"
#include "PixLayerActionDelegate.h"
#include "PixImageSharepopupLayer.h"

USING_NS_CC;

class MyAlbumModel;

class PixMyAlbumUsedPhotoSelectLayer : public PixModalLayer, public PixPhotoSelectCellDelegate
{
    
public:
    
    static PixMyAlbumUsedPhotoSelectLayer* create(MyAlbumModel *pModel, int nIndex);
    
    PixMyAlbumUsedPhotoSelectLayer(MyAlbumModel *pModel, int nIndex);
    virtual ~PixMyAlbumUsedPhotoSelectLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float dt);
    
    inline MyAlbumUsedImageModel* getCurImageModel()
    {
        return mCurImageModel;
    }
    
private:
    
    Node *rootNode;
    
    Sprite *Background;
    
    Node *MyAlbumUsedPhoto2Top;
    PixPageView *pageView;
    ui::Button *btn_back;
    ui::Text *Day;
    
    Node *MyAlbumUsedPhoto2Bottom;
    ui::Button *Button_share;
    ui::Button *Button_savephoto;
    
    MyAlbumModel *mModel;
    MyAlbumUsedImageModel *mCurImageModel;
    
    int mIndex;
    bool mbln_IsMenuLayerVisible;
    bool mbln_Check_M_Texture;
    bool mbln_IsAction;
    Sprite *mSelectImageSprite;
    
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)

    void createPageView();
    void runSelectImageSpriteAction(Sprite *pSprite);
    
    void setDayString(MyAlbumUsedImageModel *pModel);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
    void showMenuLayerAction();
    void hideMenuLayerAction();
    void showMenuLayerActionFinish(Node* node);
    void hideMenuLayerActionFinish(Node* node);
    
#pragma mark - Button
    
    void buttonBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonSharePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void buttonSavePhotoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Delegate
    
    virtual void photoSelectCellPressed(cocos2d::Ref *pTaget);
    
#pragma mark - Android BackPressed
    
    virtual void androidBackPressed();
};


#endif /* PixMyAlbumUsedPhotoSelectLayer_h */
