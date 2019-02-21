//
//  PixPhotoEditMenuLayer.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 24..
//
//

#ifndef PixPhotoEditMenuLayer_h
#define PixPhotoEditMenuLayer_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Model/PixSmartMaskBookModel.h"
#include "../UIControl/PixEditSprite.h"
#include "../Model/PixSmartMaskPageModel.h"


typedef enum
{
    MENU_NONE = 0,
    MENU_ONE_BUTTON,
    MENU_TWO_BUTTON,
    
}EDIT_MENU_TYPE;


class PixPhotoEditMenuLayerDelegate;

class PixPhotoEditMenuLayer : public Layer
{
public:
    
    static PixPhotoEditMenuLayer* create(PixEditSprite *pSprite, EDIT_MENU_TYPE type);
    
    PixPhotoEditMenuLayer(PixEditSprite *pSprite);
    virtual ~PixPhotoEditMenuLayer();
    
    virtual void initLayer();
    virtual void onEnterTransitionDidFinish();
    
    void initCoverMenu();
    void initPageMenu();
    
    void setLeft();
    void setCenter();
    void setRight();
    
    inline PixEditSprite *getEditSprite()
    {
        return editSprite;
    }
    
private:
        
    Node *rootNode;
    
    Sprite *bnt_phototouch_back1;
    Sprite *bnt_phototouch_back2;
    Sprite *bnt_phototouch_back3;
    
    ui::Button *Button_change;
    ui::Button *Button_replace;
    ui::Button *Button_delete;
    
    PixEditSprite *editSprite;
   
    CC_SYNTHESIZE(PixPhotoEditMenuLayerDelegate*, mDelegate, Delegate)
    CC_SYNTHESIZE(EDIT_MENU_TYPE, mType, Type)
    
#pragma mark - Button
    
    void btnChangePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnCoverChangePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnReplacePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void btnDeletePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
#pragma mark - Action
    
    void showLayerAction();
    void hideLayerAction();
    void showLayerActionFinish(Node* node);
    void hideLayerActionFinish(Node* node);
};

class PixPhotoEditMenuLayerDelegate
{
public:
    virtual void photoEditMenuLayerChangePressed(PixEditSprite* pSprite) {};
    virtual void photoEditMenuLayerReplacePressed(PixEditSprite* pSprite) {};
    virtual void photoEditMenuLayerDeletePressed(PixEditSprite* pSprite) {};
};



#endif /* PixPhotoEditMenuLayer_h */
