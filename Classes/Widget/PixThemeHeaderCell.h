//
//  PixThemeHeaderCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 26..
//
//

#ifndef PixThemeHeaderCell_h
#define PixThemeHeaderCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixDictionary;

class PixThemeHeaderCell : public ui::Widget
{
public :
    
    static PixThemeHeaderCell* create(const Size &contentSize);
    
    PixThemeHeaderCell(const Size &contentSize);
    virtual ~PixThemeHeaderCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();
    
private:
    
    CC_SYNTHESIZE(PixDictionary*, mDictionary, Dictionary)
};


#endif /* PixThemeHeaderCell_h */
