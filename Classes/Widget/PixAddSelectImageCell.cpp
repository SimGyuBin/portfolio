//
//  PixAddSelectImageCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 21..
//
//

#include "PixAddSelectImageCell.h"
#include "../UIControl/PixGalleryListView.h"
#include "../Info/UserInfo.h"
#include "../UIControl/PixGallerySprite.h"
#include "../Common.h"

PixAddSelectImageCell* PixAddSelectImageCell::create()
{
    PixAddSelectImageCell* widget = new (std::nothrow) PixAddSelectImageCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixAddSelectImageCell::PixAddSelectImageCell()
: _customItem(nullptr)
, _galleryListView(nullptr)
{
    clear();
}

PixAddSelectImageCell::~PixAddSelectImageCell()
{
    clear();
}

void PixAddSelectImageCell::clear()
{
    std::vector<PixImageModel*>::iterator iter;
    
    for (iter = imageModelVec.begin(); iter != imageModelVec.end(); ++iter)
    {
        CC_SAFE_DELETE(*iter);
    }
    
    imageModelVec.clear();
}

bool PixAddSelectImageCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixAddSelectImageCell::onEnter()
{
    Widget::onEnter();
}

void PixAddSelectImageCell::onExit()
{
    Widget::onExit();
}

void PixAddSelectImageCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixAddSelectImageCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
    
    if(_galleryListView != nullptr && _isTransitionFinished)
    {
        std::vector<PixImageModel*>::iterator iter;
        
        for (iter = imageModelVec.begin(); iter != imageModelVec.end(); ++iter)
        {
            if(visible)
            {
                if( _galleryListView->isAsyncLoadCall())
                {
                    (*iter)->addThumbnailImageAsync(this, imageloader_selector(PixAddSelectImageCell::addThumbnailImageAsyncCallBack), (*iter));
                }
            }
        }
    }
}

void PixAddSelectImageCell::addThumbnailImageAsyncCallBack(Texture2D *pTexture, void *pUserData)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
 
        PixImageModel *pModel = static_cast<PixImageModel*>(pUserData);
        
        if(pModel != nullptr)
        {
            pModel->setTexture(pTexture);
            LocalImageInfo *localImageInfo = pModel->getLocalImageInfo();
            
            if(localImageInfo)
            {
                Node* item = pModel->getCellItem();
                
                if(item)
                {
                    item->removeChildByTag(pModel->getIndex());
                    
                    PixGallerySprite *sprite = PixGallerySprite::createWithTexture(pTexture, pModel->getMaskFile());
                    
                    Node *loading = item->getChildByName("loading");
                    
                    if(loading)
                    {
                        sprite->setloading(loading);
                    }
                    
                    item->addChild(sprite, -1, pModel->getIndex());
                    
                    if(pModel->isSelected())
                    {
                        sprite->setScale(0.8f);
                    }
                    
                    pModel->addImageCache();
                    pModel->deleteImageLoader();
                }
                
                localImageInfo->destroyThumbnailImageData();
            }
        }
    });
}

void PixAddSelectImageCell::createCustomItem()
{
    _customItem = CSLoader::createNode("06_EditAlbum/AddSelectImageCell.csb");
    
    if(_customItem)
    {
        for (int i = 0; i < SELECT_ADDPHOTO_ITEM_COUNT; ++i)
        {
            std::string strItemName = StringUtils::format("item_%d", i);
            Node *item = dynamic_cast<Node*>(_customItem->getChildByName(strItemName));
            
            if (item)
            {
                Sprite *frame = dynamic_cast<Sprite*>(item->getChildByName("frame"));
                
                if(frame)
                {
                    item->reorderChild(frame, 100);
                }
            }
        }
        
        setContentSize(_customItem->getContentSize());   
        this->addChild(_customItem);
    }
}

PixImageModel* PixAddSelectImageCell::pushImageModel(LocalImageInfo *info, int nIndex)
{
    PixImageModel *model = new PixImageModel();
    
    model->setMaskFile("Common/photo_thum_180X180.png");
    model->setLocalImageInfo(info);
    model->setIndex(nIndex);
    
    Node *cell = dynamic_cast<Node*>(getChildren().at(0));
    
    if (cell)
    {
        Node *item = (cell->getChildren()).at(nIndex);
        
        if(item)
        {
            model->setCellItem(item);
            model->setCell(this);
        }
    }
    
    imageModelVec.push_back(model);
    
    return model;
}

Node* PixAddSelectImageCell::getCellItem(const std::string &name)
{
    if(_customItem != nullptr)
    {
        return _customItem->getChildByName(name);
    }
    
    return nullptr;
}

void PixAddSelectImageCell::removeCellItem(Node *pItme)
{
    if(_customItem != nullptr && pItme != nullptr)
    {
        _customItem->removeChild(pItme);
    }
}

void PixAddSelectImageCell::setCellSelected(int nIndex, bool isSelected)
{
    if(nIndex >= 0 || imageModelVec.size() <= nIndex)
    {
        imageModelVec.at(nIndex)->setSelected(isSelected);
    }
}

bool PixAddSelectImageCell::isCellSelected(int nIndex)
{
    if(nIndex >= 0 || imageModelVec.size() <= nIndex)
    {
        return imageModelVec.at(nIndex)->isSelected();
    }
    
    return false;
}

PixImageModel* PixAddSelectImageCell::getImageModel(int nIndex)
{
    if(nIndex >= 0 || imageModelVec.size() <= nIndex)
    {
        return imageModelVec.at(nIndex);
    }
    
    return nullptr;
}

PixImageModel* PixAddSelectImageCell::getImageModelByPath(const std::string &path)
{
    for (auto iter = imageModelVec.begin(); iter != imageModelVec.end(); ++iter)
    {
        if((*iter)->getLocalImageInfo())
        {
            LocalImageInfo *info = (*iter)->getLocalImageInfo();
            
            if(info->getPath().compare(path) == 0)
            {
                return (*iter);
            }
        }
    }
    
    return nullptr;
}

Texture2D* PixAddSelectImageCell::getImageModelTexture(int nIndex)
{
    if(nIndex >= 0 || imageModelVec.size() <= nIndex)
    {
        return imageModelVec.at(nIndex)->getTexture();
    }
    
    return nullptr;
}


void PixAddSelectImageCell::deleteImageLoader()
{
    std::vector<PixImageModel*>::iterator iter;
    
    for (iter = imageModelVec.begin(); iter != imageModelVec.end(); ++iter)
    {
        (*iter)->deleteImageLoader();
    }
}
