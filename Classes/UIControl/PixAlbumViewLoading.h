//
//  PixAlbumViewLoading.h
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 17..
//
//

#ifndef PixAlbumViewLoading_h
#define PixAlbumViewLoading_h

#include "cocos2d.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace timeline;

class PixAlbumViewLoading
{

public :
    
    PixAlbumViewLoading();
    virtual ~PixAlbumViewLoading();
    
    virtual bool init(const std::string& filename, const std::string& timeline);
    virtual void play();
    virtual Node* getLoading();
    
private:
    
    Node *Loading;
    ActionTimeline *timeLine;
    
};

#endif /* PixAlbumViewLoading_h */
