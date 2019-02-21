//
//  PixSelectImageCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 1. 26..
//
//

#include "PixSelectImageCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixGallerySprite.h"
#include "../Info/UserInfo.h"
#include "flatbuffers/flatbuffers.h"
#include "../Common.h"

PixSelectImageCell* PixSelectImageCell::create()
{
    PixSelectImageCell* widget = new (std::nothrow) PixSelectImageCell();
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixSelectImageCell::PixSelectImageCell()
: _customItem(nullptr)
, _dateCell(nullptr)
, _galleryListView(nullptr)
, menu(nullptr)
{
    imageModelVec.clear();
}

PixSelectImageCell::~PixSelectImageCell()
{
    std::vector<PixImageModel*>::iterator iter;
    
    for (iter = imageModelVec.begin(); iter != imageModelVec.end(); ++iter)
    {
        IMAGECACHE_MANAGER->removeImageModel((*iter));
        (*iter)->removeThumbnailImage();
        CC_SAFE_DELETE(*iter);
    }
    
    imageModelVec.clear();
}

bool PixSelectImageCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}
void PixSelectImageCell::onEnter()
{
    Widget::onEnter();
}

void PixSelectImageCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();   
}

void PixSelectImageCell::onExit()
{
    Widget::onExit();
}

void PixSelectImageCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
    
    if(_galleryListView != nullptr && _galleryListView->isVisible() && _isTransitionFinished && _galleryListView->isAsyncLoadCall())
    {
        std::vector<PixImageModel*>::iterator iter;
        
        for (iter = imageModelVec.begin(); iter != imageModelVec.end(); ++iter)
        {
            if(visible)
            {
                if((*iter)->getLoading() == false)
                {
                    (*iter)->addThumbnailImageAsync(this, imageloader_selector(PixSelectImageCell::addThumbnailImageAsyncCallBack), (*iter));
                }
                else
                {
//                    Node* item = (*iter)->getCellItem();
//                    
//                    if(item)
//                    {
//                        PixGallerySprite *sprite = dynamic_cast<PixGallerySprite*>(item->getChildByTag((*iter)->getIndex()));
//                        
//                        if(sprite)
//                        {
//                            if(sprite->getTexture() == nullptr)
//                            {
//                                
//                            }
//                        }
//                    }
                }
            }
        }
    }
}

void PixSelectImageCell::update(float dt)
{
    Widget::update(dt);
}

void PixSelectImageCell::addThumbnailImageAsyncCallBack(Texture2D *pTexture, void *pUserData)
{
    // 메인 스레드 에서 메모리 해제
    
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        PixImageModel *pModel = static_cast<PixImageModel*>(pUserData);
        
        if(pModel != nullptr)
        {
            pModel->setTexture(pTexture);
            pModel->deleteImageLoader();
            
            
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
                
                item->addChild(sprite, 10, pModel->getIndex());
                
                if(pModel->isSelected())
                {
                    sprite->setScale(0.8f);
                }
            }
            
            LocalImageInfo *localImageInfo = pModel->getLocalImageInfo();
            
            if(localImageInfo)
            {
                localImageInfo->destroyThumbnailImageData();
            }
            
            pModel->addImageCache();
        }
    });
}

void PixSelectImageCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("04_SelectPhoto/SelectImageCell.csb");
    
    if(_customItem)
    {
        for (int i = 0; i < SELECT_ITEM_COUNT; ++i)
        {
            std::string strItemName = StringUtils::format("item_%d", i);
            
            Node *item = dynamic_cast<Node*>(_customItem->getChildByName(strItemName));
            
            if(item)
            {            
                std::string normalImg = "PixGalleryLevel/selectphoto_frame_3.png";
                std::string pressedImg = "PixGalleryLevel/selectphoto_frame_2.png";
                std::string disabledImg = "";
                
                auto menuItem = MenuItemImage::create(normalImg, pressedImg);
                menuItem->setCallback(CC_CALLBACK_1(PixSelectImageCell::itemPressed, this));
                menuItem->setTag(i);
                
                menu = PixScrollMenu::create(menuItem, nullptr);
                menu->setPosition(Vec2(item->getContentSize().width / 2, item->getContentSize().height / 2));
                menu->setAnchorPoint(Vec2::ZERO);
                menu->setContentSize(item->getContentSize());
                item->addChild(menu, 1000);
                
                Node *selected = dynamic_cast<Node*>(item->getChildByName("selected"));
                
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
            }
        }
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
    }
}

void PixSelectImageCell::pushImageModel(PixImageModel *pModel, int nIndex)
{
    if(pModel)
    {
        pModel->setIndex(nIndex);
        
        Node *cell = dynamic_cast<Node*>(getChildren().at(0));
        
        if (cell)
        {
            Node *item = (cell->getChildren()).at(nIndex);
            
            if(item)
            {
                pModel->setCellItem(item);
                pModel->setCell(this);
            }
        }
        
        imageModelVec.push_back(pModel);
    }
}

PixImageModel* PixSelectImageCell::pushImageModel(LocalImageInfo *info, int nIndex)
{
    PixImageModel *model = new PixImageModel();
    
    model->setMaskFile("Common/photo_thum_177X177.png");
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

Node* PixSelectImageCell::getCellItem(const std::string &name)
{
    if(_customItem != nullptr)
    {
        return _customItem->getChildByName(name);
    }
    
    return nullptr;
}

void PixSelectImageCell::removeCellItem(Node *pItme)
{
    if(_customItem != nullptr && pItme != nullptr)
    {
        _customItem->removeChild(pItme);
    }
}

void PixSelectImageCell::setCellSelected(int nIndex, bool isSelected)
{
    if(nIndex >= 0 || imageModelVec.size() <= nIndex)
    {        
        imageModelVec.at(nIndex)->setSelected(isSelected);
    }
}

void PixSelectImageCell::setCellSelectImage(int nIndex, bool isVisible)
{
    if(nIndex >= 0 || imageModelVec.size() <= nIndex)
    {
        imageModelVec.at(nIndex)->setSelectImage(isVisible);
    }
}

void PixSelectImageCell::setCellSelectedEffect(int nIndex, bool isVisible)
{
    if(nIndex >= 0 || imageModelVec.size() <= nIndex)
    {
        imageModelVec.at(nIndex)->selectedEffect(isVisible);
    }
}

bool PixSelectImageCell::isCellSelected(int nIndex)
{
    if(nIndex >= 0 || imageModelVec.size() <= nIndex)
    {
        return imageModelVec.at(nIndex)->isSelected();
    }
    
    return false;
}

void PixSelectImageCell::updateSelected()
{
    if(!imageModelVec.empty())
    {
        for(auto iter = imageModelVec.begin(); iter != imageModelVec.end(); ++iter)
        {
            (*iter)->updateSelected();
        }
    }
}

int PixSelectImageCell::getImageModelCount()
{
    return (int)imageModelVec.size();
}

PixImageModel* PixSelectImageCell::getImageModel(int nIndex)
{
    if(nIndex >= 0 || imageModelVec.size() <= nIndex)
    {
        return imageModelVec.at(nIndex);
    }
    
    return nullptr;
}

PixImageModel* PixSelectImageCell::getImageModelByPath(const std::string &path)
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

Texture2D* PixSelectImageCell::getImageModelTexture(int nIndex)
{
    if(nIndex >= 0 || imageModelVec.size() <= nIndex)
    {
        return imageModelVec.at(nIndex)->getTexture();
    }

    return nullptr;
}


void PixSelectImageCell::deleteImageLoader()
{
    std::vector<PixImageModel*>::iterator iter;
    
    for (iter = imageModelVec.begin(); iter != imageModelVec.end(); ++iter)
    {
        (*iter)->deleteImageLoader();
    }
}


void PixSelectImageCell::setQuickNavigationUpdateLabel(ui::Text *pLabel)
{
    if(pLabel)
    {
        if(!imageModelVec.empty())
        {
            PixImageModel *pModel = (*imageModelVec.begin());
            LocalImageInfo *localImageInfo = pModel->getLocalImageInfo();
            
            if(localImageInfo)
            {
                std::string strDate = localImageInfo->getFullDate();
                
                int nYear = atoi(strDate.substr(0,4).c_str());
                int nMonth = atoi(strDate.substr(4,2).c_str());
                
                std::string strYearMonth = PixQuickNavigation::getQuickNavigationYearMonth(nYear,nMonth);

                pLabel->setString(strYearMonth);
            }
        }
    }
}

#pragma mark - Button

void PixSelectImageCell::itemPressed(cocos2d::Ref *pSender)
{
/*
 
    디버깅용 코드
 
    if(pSender)
    {
        Node *node = dynamic_cast<Node*>(pSender);
        
        if (node)
        {
            int nTag = node->getTag();
            
            PixImageModel *pModel = getImageModel(nTag);
            
            if(pModel)
            {
                
            }
        }
    }
 
 */
}
