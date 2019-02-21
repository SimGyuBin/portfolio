//
//  PixAlbumLoadingCoverCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 9. 5..
//
//

#ifndef PixAlbumLoadingCoverCell_h
#define PixAlbumLoadingCoverCell_h

#include "cocos2d.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixAlbumLoadingCoverCell : public ui::Widget
{

public :
    
    static PixAlbumLoadingCoverCell* create();
    
    PixAlbumLoadingCoverCell();
    virtual ~PixAlbumLoadingCoverCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    virtual void onExit() override;
    virtual void setVisible(bool visible) override;
    virtual void update(float dt) override;
    
    void createCustomItem();
    void setRetry(bool var);
    bool isRetry();
    Node* getCellChildByName(const std::string& name);
    
private:
    
    Node *_customItem;
    bool mbln_Retry;

};

#endif /* PixAlbumLoadingCoverCell_h */
