//
//  PixPageView.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 23..
//
//

#ifndef PixPageView_h
#define PixPageView_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixPageView : public ui::PageView
{
    
public :

    static PixPageView* create();
    
    PixPageView();
    virtual ~PixPageView();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void update(float dt) override;
    
    void copyClonedPageView(ui::PageView *pPageView);
    
};

#endif /* PixPageView_h */
