//
//  PixSelectDateCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 2..
//
//

// 카스텀 위젯 클래스
// 이미지 선택 리스트뷰 에서 사용

#ifndef PixSelectDateCell_h
#define PixSelectDateCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "../Model/PixImageModel.h"

class PixSelectDateCell : public ui::Widget
{
public :
    
    PixSelectDateCell();
    virtual ~PixSelectDateCell();
    
    static PixSelectDateCell* create();
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();
   
    void setDateString(const std::string &string);
        
    Node* getCellItem(const std::string &name);
    void removeCellItem(Node *pItme);
    
    void pushDateCellChild(PixImageModel* pModel);
    void setDateCellChildCheck(bool isSelectedState, int nSelecteCount);
    void checkSelectAlldateItem();

private:
    
    Node *_customItem;
    ui::Text *_datelabel;

    std::vector<PixImageModel*> dateCellChildVec;
    
    CC_SYNTHESIZE(ui::CheckBox*, _datecheckbox, Datecheckbox)
};


#endif /* PixSelectDateCell_h */
