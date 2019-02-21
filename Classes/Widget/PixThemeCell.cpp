//
//  PixTheneCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 3. 22..
//
//

#include "PixThemeCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Network/PixHttpClient.h"
#include "../Util/PixStringUtils.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixThemeCell* PixThemeCell::create(bool isThemeChange)
{
    PixThemeCell* widget = new (std::nothrow) PixThemeCell(isThemeChange);
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixThemeCell::PixThemeCell(bool isThemeChange)
: _customItem(nullptr)
, mbln_ThemeChange(isThemeChange)
{
    themeVec.clear();
}

PixThemeCell::~PixThemeCell()
{
    themeVec.clear();
}

bool PixThemeCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixThemeCell::onEnter()
{
    Widget::onEnter();
}

void PixThemeCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
}

void PixThemeCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("03_Theme/ThemeCell.csb");
    
    if(_customItem)
    {
        for (int i = 0 ; i < SELECT_THEME_COUNT; ++i)
        {
            std::string strItemName = StringUtils::format("item_%d", i);
            
            Node *item = dynamic_cast<Node*>(_customItem->getChildByName(strItemName));
            
            if(item)
            {
                PixDictionary *pDic = getThemeInfo(i);
            
                if(pDic)
                {
                    ui::Layout *Panel_ThemeImg = dynamic_cast<ui::Layout*>(item->getChildByName("Panel_ThemeImg"));
                    
                    if(Panel_ThemeImg)
                    {
                        Size size = Panel_ThemeImg->getContentSize();
                        Vec2 position = Vec2(size.width * 0.5f, size.height * 0.5f);
                        Vec2 anchorPoint = Vec2(0.5f,0.5f);
                        
                        std::string strName = Panel_ThemeImg->getName();
                        
                        PixDictionary *imageDic = pDic->dictionaryForKey("image_path");
                        
                        if(imageDic)
                        {
                            std::string image_path = imageDic->stringForKey(SMARTALBUM_MANAGER->getLanguageCode());
                            
                            if(image_path.empty())
                            {
                                image_path = imageDic->stringForKey(DEFAULT_LANGUAGE);
                            }
                            
                            if(!image_path.empty())
                            {
                                std::vector<std::string> tokens;
                                PixStringUtils::getTokens(tokens, image_path, "/");
                                
                                std::string filename;
                                filename.clear();
                                
                                if(!tokens.empty())
                                {
                                    filename = tokens[tokens.size() - 1];
                                }
                                
                                std::string fullPath = std::string(THEME_PATH).append(filename);
                                
                                Sprite *theemeSprite = Sprite::create(fullPath);
                                
                                if(theemeSprite)
                                {
                                    theemeSprite->setPosition(position);
                                    theemeSprite->setAnchorPoint(anchorPoint);
                                    theemeSprite->setName(strName);
                                    
                                    Panel_ThemeImg->addChild(theemeSprite);
                                }
                            }
                        }
                    }
                    
                    ui::ImageView *Click_non = dynamic_cast<ui::ImageView*>(item->getChildByName("Click_non"));
                    
                    if(Click_non)
                    {
                        Click_non->setVisible(true);
                    }
                    
                    ui::ImageView *Click = dynamic_cast<ui::ImageView*>(item->getChildByName("Click"));
                    
                    if(Click)
                    {
                        Click->setVisible(false);
                    }
                    
                    ui::Text *label_Title = dynamic_cast<ui::Text*>(item->getChildByName("label_Title"));
                    
                    if(label_Title)
                    {
                        label_Title->setString(pDic->stringForKey("title"));
                    }
                }
            }
        }

        setContentSize(_customItem->getContentSize());
        
        this->addChild(_customItem);
    }
}

void PixThemeCell::pushDefaultInfo(PixDictionary *pDic)
{
    themeVec.push_back(pDic);
}

void PixThemeCell::reomveDefaultInfo(PixDictionary *pDic)
{
    if(themeVec.empty())
        return;
    
    for (auto iter = themeVec.begin(); iter != themeVec.end(); ++iter)
    {
        if((*iter) == pDic)
        {
            themeVec.erase(iter);
            return;
        }
    }
}

int PixThemeCell::getThemeCount()
{
    return (int)themeVec.size();
}

PixDictionary* PixThemeCell::getThemeInfo(int nIndex)
{
    if (nIndex < 0 || nIndex >= themeVec.size())
    {
        return nullptr;
    }
    
    return themeVec.at(nIndex);
}

PixArray* PixThemeCell::getPreview(int nIndex)
{
    PixDictionary *pDic = getThemeInfo(nIndex);
    
    if (pDic)
    {
        return pDic->arrayForKey("preview");
    }
    
    return nullptr;
}

Node* PixThemeCell::getCellItem(int nIndex)
{
    std::string strItemName = StringUtils::format("item_%d", nIndex);
    
    return getCellItem(strItemName);
}

Node* PixThemeCell::getCellItem(const std::string &name)
{
    if(_customItem != nullptr)
    {
        return _customItem->getChildByName(name);
    }
    
    return nullptr;
}

void PixThemeCell::removeCellItem(Node *pItme)
{
    if(_customItem != nullptr && pItme != nullptr)
    {
        _customItem->removeChild(pItme);
    }
}

void PixThemeCell::setCellSelected(int nIndex, bool isSelected)
{
    ui::ImageView *click = getClickSprite(nIndex);
    ui::ImageView *clickNon = getClickNonSprite(nIndex);
    
    if(isSelected)
    {
        if(click)
        {
            click->setVisible(true);
        }
        
        if(clickNon)
        {
            clickNon->setVisible(false);
        }
    }
    else
    {
        if(click)
        {
            click->setVisible(false);
        }
        
        if(clickNon)
        {
            clickNon->setVisible(true);
        }
    }
}

bool PixThemeCell::isCellSelected(int nIndex)
{
    ui::ImageView *btn_click = getClickSprite(nIndex);
    
    if(btn_click)
    {
        return btn_click->isVisible();
    }
    
    return false;
}

ui::ImageView* PixThemeCell::getClickNonSprite(int nIndex)
{
    if(_customItem)
    {
        std::string strItemName = StringUtils::format("item_%d", nIndex);
        
        Node *item = dynamic_cast<Node*>(_customItem->getChildByName(strItemName));
        
        if(item)
        {
            ui::ImageView *btn_click = dynamic_cast<ui::ImageView*>(item->getChildByName("Click_non"));
            
            if(btn_click)
            {
                return btn_click;
            }
        }
    }
    
    return nullptr;
}

ui::ImageView* PixThemeCell::getClickSprite(int nIndex)
{
    if(_customItem)
    {
        std::string strItemName = StringUtils::format("item_%d", nIndex);
        
        Node *item = dynamic_cast<Node*>(_customItem->getChildByName(strItemName));
        
        if(item)
        {
            ui::ImageView *btn_click = dynamic_cast<ui::ImageView*>(item->getChildByName("Click"));
            
            if(btn_click)
            {
                return btn_click;
            }
        }
    }
    
    return nullptr;
}

ui::ImageView* PixThemeCell::getPreviewImage(int nIndex)
{
    if(_customItem)
    {
        std::string strItemName = StringUtils::format("item_%d", nIndex);
        
        Node *item = dynamic_cast<Node*>(_customItem->getChildByName(strItemName));
        
        if(item)
        {
            ui::ImageView *btn_click = dynamic_cast<ui::ImageView*>(item->getChildByName("Image_view"));
            
            if(btn_click)
            {
                return btn_click;
            }
        }
    }
    
    return nullptr;
}


int PixThemeCell::getCellItemTag(int nIndex)
{
    Node *item = _customItem->getChildren().at(nIndex);
    
    if(item)
    {
        return item->getTag();
    }
    
    return -1;
}

