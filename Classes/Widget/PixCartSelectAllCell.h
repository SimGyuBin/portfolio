//
//  PixCartSelectAllCell.h
//  Pzle
//
//  Created by 심규빈 on 2017. 3. 9..
//
//

#ifndef PixCartSelectAllCell_h
#define PixCartSelectAllCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixCartSelectAllCellDelegate;

class PixCartSelectAllCell : public ui::Widget
{
public:
    
    static PixCartSelectAllCell* create();
    
    PixCartSelectAllCell();
    virtual ~PixCartSelectAllCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void createCustomItem();
    void setCheckSelected(bool var);
    void setCheckEnabled(bool var);
    void runDeleteAction(const std::function<void(ui::Widget*)>& callback);
    void deleteActionFinish();
    
private:
    
    Node *_customItem;
    ui::CheckBox *check;
    ui::Text *SelectAll;
    std::function<void(ui::Widget*)> mCallback;
    
    CC_SYNTHESIZE(PixCartSelectAllCellDelegate*, mDelegate, Delegate)
    
#pragma mark - Button
    
    void checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type);
};

class PixCartSelectAllCellDelegate
{
public :
    virtual void cartSelectAllCellCheckBoxPressed(bool var) {};
};

#endif /* PixCartSelectAllCell_h */
