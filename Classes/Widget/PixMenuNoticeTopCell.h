//
//  PixMenuNoticeTopCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 19..
//
//

#ifndef PixMenuNoticeTopCell_h
#define PixMenuNoticeTopCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixMenuNoticeTopCell : public ui::Widget
{
    
public :
    
    static PixMenuNoticeTopCell* create();
    
    PixMenuNoticeTopCell();
    virtual ~PixMenuNoticeTopCell();
    
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    
    void createCustomItem();
    
private:
    
    Node *_customItem;
};


#endif /* PixMenuNoticeTopCell_h */
