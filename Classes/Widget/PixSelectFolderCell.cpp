//
//  PixSelectFolderCell.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 13..
//
//

#include "PixSelectFolderCell.h"
#include "../Model/PixImageModel.h"
#include "../Util/PixUtils.h"
#include "../Info/LocalImageInfo.h"
#include "../UIControl/PixScrollMenu.h"
#include "../UIControl/PixGalleryListView.h"

PixSelectFolderCell* PixSelectFolderCell::create(PixImageModel *pModel, const std::string &folderName, int nCount)
{
    PixSelectFolderCell* widget = new (std::nothrow) PixSelectFolderCell(pModel, folderName, nCount);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;

}

PixSelectFolderCell::PixSelectFolderCell(PixImageModel *pModel, const std::string &folderName, int nCount)
: _customItem(nullptr)
, folderName(nullptr)
, count(nullptr)
, frame(nullptr)
, sprite(nullptr)
, mModel(nullptr)
, mDelegate(nullptr)
, mstr_FolderName("")
, mstr_FolderPath("")
, mn_Count(0)
, mbln_ViewAll(false)
, _galleryListView(nullptr)
{
    if(pModel)
    {
        mModel = pModel->clone();
        mModel->setLoading(false);
        mModel->setMaskFile("Common/photo_thum_105X105.png");
    }
    
    mstr_FolderName = folderName;
    mn_Count = nCount;
}

PixSelectFolderCell::~PixSelectFolderCell()
{
    CC_SAFE_DELETE(mModel);
}

bool PixSelectFolderCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;

}

void PixSelectFolderCell::onEnter()
{
    Widget::onEnter();
}
void PixSelectFolderCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixSelectFolderCell::onExit()
{
    Widget::onExit();
}

void PixSelectFolderCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
    
    if(mModel)
    {
        if(visible)
        {
            Node* item = mModel->getCellItem();
            
            if(item)
            {
                if(item->getChildByTag(mModel->getIndex()))
                {
                    return;
                }
            }
            else
            {
                return;
            }
            
            mModel->addThumbnailImageAsync(this, imageloader_selector(PixSelectFolderCell::addThumbnailImageAsyncCallBack), mModel);
        }
    }
}

void PixSelectFolderCell::update(float dt)
{
    Widget::update(dt);
}

void PixSelectFolderCell::createCustomItem()
{
    _customItem = CSLoader::createNode("04_SelectPhoto/SelectFolderItem.csb");
    
    if(_customItem)
    {
        ui::Button *button = dynamic_cast<ui::Button*>(_customItem->getChildByName("Button"));
        
        if(button)
        {
            std::string normalImg = button->getNormalFile().file;
            std::string pressedImg = button->getPressedFile().file;
            std::string disabledImg = button->getDisabledFile().file;
            
            Vec2 pos = button->getPosition();
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                  CC_CALLBACK_1(PixSelectFolderCell::cellPressed, this));
            
            PixScrollMenu *menu = PixScrollMenu::create(menuItem, nullptr);
            menu->setPosition(pos);
            menu->setContentSize(button->getContentSize());
            
            _customItem->addChild(menu, -1);

            button->removeFromParentAndCleanup(true);
        }

        folderName = dynamic_cast<ui::Text*>(_customItem->getChildByName("label"));
        
        if(folderName)
        {
            folderName->setString(getFolderName());
            
            if(folderName->getContentSize().width > FOLDER_CELL_TEXT_LIMITWIDTH)
            {
                shortenLabelString(folderName, FOLDER_CELL_TEXT_LIMITWIDTH);
            }
        }
     
        count = dynamic_cast<ui::Text*>(_customItem->getChildByName("label_0"));
        
        if(count)
        {
            count->setString(StringUtils::format("%d",mn_Count));
        }
        
        frame = dynamic_cast<ui::Layout*>(_customItem->getChildByName("frame"));
        
        setContentSize(_customItem->getContentSize());
        addChild(_customItem);
        
        if(mModel)
        {
            mModel->setCellItem(frame);
            mModel->setCell(this);
            mModel->setIndex(100);
        }
    }
}

void PixSelectFolderCell::addThumbnailImageAsyncCallBack(Texture2D *pTexture, void *pUserData)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        PixImageModel *pModel = static_cast<PixImageModel*>(pUserData);
        
        if(pModel != nullptr)
        {
            pModel->setTexture(pTexture);
            pModel->deleteImageLoader();
         
            if(pModel->getCellItem())
            {
                sprite = PixGallerySprite::createWithTexture(pTexture, pModel->getMaskFile());
                
                if(frame)
                {
                    frame->addChild(sprite, 10, pModel->getIndex());
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

#pragma mark - Button

void PixSelectFolderCell::cellPressed(cocos2d::Ref *pSender)
{
    if(mDelegate)
    {
        if(getViewAll())
        {
            mDelegate->selectFolderCellViewAllPressed();
        }
        else
        {
            mDelegate->selectFolderCellPressed(getFolderPath(), getFolderName());
        }
    }
}
