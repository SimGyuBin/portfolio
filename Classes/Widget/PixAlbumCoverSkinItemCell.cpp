//
//  PixAlbumCoverSkinItemCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 22..
//
//

#include "PixAlbumCoverSkinItemCell.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixGallerySprite.h"
#include "../Info/CardSkinInfo.h"
#include "../Util/PixStringUtils.h"
#include "../Common.h"

PixAlbumCoverSkinItemCell* PixAlbumCoverSkinItemCell::create()
{
    PixAlbumCoverSkinItemCell* widget = new (std::nothrow) PixAlbumCoverSkinItemCell();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixAlbumCoverSkinItemCell::PixAlbumCoverSkinItemCell()
: _customItem(nullptr)
{
    cardSkinInfoVec.clear();
}

PixAlbumCoverSkinItemCell::~PixAlbumCoverSkinItemCell()
{
    cardSkinInfoVec.clear();
}

bool PixAlbumCoverSkinItemCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixAlbumCoverSkinItemCell::onEnter()
{
    Widget::onEnter();
}

void PixAlbumCoverSkinItemCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixAlbumCoverSkinItemCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
}

void PixAlbumCoverSkinItemCell::createCustomItem()
{
    _customItem = CSLoader::createNode("07_My_AlbumList/AlbumCoverSkinItemCell.csb");
    
    setContentSize(_customItem->getContentSize());
    
    this->addChild(_customItem);
    
    
    for (int i = 0 ; i < cardSkinInfoVec.size(); ++i)
    {
        std::string strItemName = StringUtils::format("AlbumSkinItem_%d", i);
        
        Node *item = dynamic_cast<Node*>(_customItem->getChildByName(strItemName));
        
        if(item)
        {
            Node *selected = dynamic_cast<Node*>(item->getChildByName("Select"));

            if(selected)
            {
                selected->setVisible(false);
                item->reorderChild(selected, 100);
            }

            Node *check = dynamic_cast<Node*>(item->getChildByName("check"));

            if(check)
            {
                check->setVisible(false);
                item->reorderChild(check, 101);
            }
            
            CardSkinInfo *info = cardSkinInfoVec.at(i);
            
            if(info)
            {
                std::string image_path = info->getCard_skin_thumb_url();
                
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
                    
                    std::string fullPath = std::string(CARDSKIN_PATH).append(filename);
                    
                    Sprite *skin_thumb = Sprite::create(fullPath);
                   
                    if(skin_thumb)
                    {
                        skin_thumb->setAnchorPoint(Vec2::ZERO);
                        item->addChild(skin_thumb);
                    }
                }
            }
        }
    }
}

Node* PixAlbumCoverSkinItemCell::getCellItem(const std::string &name)
{
    if(_customItem != nullptr)
    {
        return _customItem->getChildByName(name);
    }
    
    return nullptr;
}

void PixAlbumCoverSkinItemCell::removeCellItem(Node *pItme)
{
    if(_customItem != nullptr && pItme != nullptr)
    {
        _customItem->removeChild(pItme);
    }
}

void PixAlbumCoverSkinItemCell::setCellSelected(int nIndex, bool isSelected)
{
    std::string strItemName = StringUtils::format("AlbumSkinItem_%d", nIndex);
    
    Node *item = dynamic_cast<Node*>(_customItem->getChildByName(strItemName));
    
    if(item)
    {
        Node *selected = dynamic_cast<Node*>(item->getChildByName("Select"));
        
        if(selected)
        {
            selected->setVisible(isSelected);
        }
        
        Node *check = dynamic_cast<Node*>(item->getChildByName("check"));
        
        if(check)
        {
            check->setVisible(isSelected);
        }
    }
}

bool PixAlbumCoverSkinItemCell::isCellSelected(int nIndex)
{
    std::string strItemName = StringUtils::format("AlbumSkinItem_%d", nIndex);
    
    Node *item = dynamic_cast<Node*>(_customItem->getChildByName(strItemName));
    
    if(item)
    {
        Node *selected = dynamic_cast<Node*>(item->getChildByName("Select"));
        
        if(selected)
        {
            return selected->isVisible();
        }
    }
    
    return false;
}

void PixAlbumCoverSkinItemCell::pushCardSkinInfo(CardSkinInfo *pInfo)
{
    cardSkinInfoVec.push_back(pInfo);
}

CardSkinInfo* PixAlbumCoverSkinItemCell::getCardSkinInfo(int nIndex)
{
    if (nIndex < 0 || nIndex >= cardSkinInfoVec.size())
    {
        return nullptr;
    }
    
    return cardSkinInfoVec.at(nIndex);
}
