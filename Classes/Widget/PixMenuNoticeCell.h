//
//  PixMenuNoticeCell.hpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 16..
//
//

#ifndef PixMenuNoticeCell_h
#define PixMenuNoticeCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixScrollMenu.h"
#include "../Layer/PixLayerActionDelegate.h"
#include "../Info/NoticeInfo.h"

USING_NS_CC;

class PixMenuNoticeCell : public ui::Widget
{
    
public :
    
    static PixMenuNoticeCell* create(NoticeInfo *pInfo);
    
    PixMenuNoticeCell(NoticeInfo *pInfo);
    virtual ~PixMenuNoticeCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();

private:
    
    Node *_customItem;
    ui::Text *Text_title;
    ui::Text *Text_day;
    
    NoticeInfo *mInfo;
        
    CC_SYNTHESIZE(PixLayerActionDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void ButtonNoticeCellPressed(cocos2d::Ref *pSender);
};


#endif /* PixMenuNoticeCell_h */
