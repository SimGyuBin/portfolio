//
//  PixTheneCell.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 22..
//
//

#ifndef PixTheneCell_h
#define PixTheneCell_h

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class PixDictionary;
class PixArray;

class PixThemeCell : public ui::Widget
{
public :
    
    static PixThemeCell* create(bool isThemeChange = false);
    
    PixThemeCell(bool isThemeChange);
    virtual ~PixThemeCell();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void setVisible(bool visible) override;
    
    void createCustomItem();
    void pushDefaultInfo(PixDictionary *pDic);
    void reomveDefaultInfo(PixDictionary *pDic);
    int getThemeCount();
    PixDictionary* getThemeInfo(int nIndex);
    PixArray *getPreview(int nIndex);
    Node* getCellItem(int nIndex);
    Node* getCellItem(const std::string &name);
    void removeCellItem(Node *pItme);
    void setCellSelected(int nIndex, bool isSelected);
    bool isCellSelected(int nIndex);
    ui::ImageView* getClickNonSprite(int nIndex);
    ui::ImageView* getClickSprite(int nIndex);
    ui::ImageView *getPreviewImage(int nIndex);
    int getCellItemTag(int nIndex);
    
private:
    
    Node *_customItem;
    std::vector<PixDictionary*> themeVec;
    
    CC_SYNTHESIZE(bool, mbln_ThemeChange, ThemeChange)
};

#endif /* PixTheneCell_h */
