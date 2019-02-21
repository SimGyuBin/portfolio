//
//  PixMyAlbumNewsCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 24..
//
//

#ifndef PixMyAlbumNewsCell_h
#define PixMyAlbumNewsCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../UIControl/PixTextFlow.h"

USING_NS_CC;

class BoardBastInfo;

class PixMyAlbumNewsCell : public ui::Widget, public PixTextFlowActionDelegate
{
public:
    
    static PixMyAlbumNewsCell* create();
    
    PixMyAlbumNewsCell();
    virtual ~PixMyAlbumNewsCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    void pushBoardBastInfo(BoardBastInfo *pBoardBastInfo);
    void startFlow();
    
private:
    
    Node *_customItem;
    ui::Text *Text_News;
    PixTextFlow *textFlow;
    ui::Layout *Panel;
    
    Size frame;
    const char *fontName;
    int fontSize;
    Color4B color;
    Vec2 pos;
    Vec2 anchor;
    
    std::deque<BoardBastInfo*> newsQueue;
    
    CC_SYNTHESIZE(BoardBastInfo*, mBoardBastInfo, BoardBastInfo)
    
    void createTextFlow(BoardBastInfo *pBoardBastInfo);
    
#pragma mark - Delegate
    
    virtual void textFlowActionFinish() override;

};

#endif /* PixMyAlbumNewsCell_h */
