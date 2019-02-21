//
//  PixInfinitePageView.h
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 7..
//
//

#ifndef PixInfinitePageView_h
#define PixInfinitePageView_h

#include "PixPageView.h"

USING_NS_CC;

class PixInfinitePageView : public PixPageView
{
    
public :
    
    static PixInfinitePageView* create();
    
    PixInfinitePageView();
    virtual ~PixInfinitePageView();
    
    virtual bool init() override;
    
    void addPage(Widget* page);
    void setInfinitePageView();
    
    ssize_t getInfinitePageIndex() const
    {
        return _currentPageIndex % mn_realItemSize;
    }

protected:
    
    virtual void handleReleaseLogic(Touch *touch) override;
    
private:
    
    ssize_t mn_realItemSize;
    
    Vector<Widget*> _frontItems;
    Vector<Widget*> _backItems;
    
};


#endif /* PixInfinitePageView_h */
