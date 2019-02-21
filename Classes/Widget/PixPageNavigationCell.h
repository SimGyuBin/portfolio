//
//  PixPageNavigationCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 8. 4..
//
//

#ifndef PixPageNavigationCell_h
#define PixPageNavigationCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"

USING_NS_CC;

class PixGalleryListView;
class PixPageNavigationCellDelegate;

class PixPageNavigationCell : public ui::Widget
{
    
public :
    
    static PixPageNavigationCell* create(int nIndex, PixGalleryListView *pListView);
    
    PixPageNavigationCell(int nIndex, PixGalleryListView *pListView);
    virtual ~PixPageNavigationCell();
    
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    
    void createCustomItem();
    void setDisabled(bool var);
    void setPressed(bool var);
    
private:
    
    Node *_customItem;
    PixScrollMenu *normal;
    PixScrollMenu *pressed;
    PixScrollMenu *disabled;
    PixGalleryListView *ListView;
    
    CC_SYNTHESIZE(int, mn_Index, Index)
    CC_SYNTHESIZE(PixPageNavigationCellDelegate*, mDelegate, Delegate)
    
    void ButtonNormalPressed(cocos2d::Ref *pSender);
    void ButtonPressedPressed(cocos2d::Ref *pSender);
    void ButtonDisabledPressed(cocos2d::Ref *pSender);
};

class PixPageNavigationCellDelegate
{
public:
    
    virtual void pageNavigationCellPressed(int nIndex) {};
};


#endif /* PixPageNavigationCell_h */
