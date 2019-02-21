//
//  PixAlbumSelectUsedPhotoCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 14..
//
//

#include "PixAlbumSelectUsedPhotoCell.h"
#include "../UIControl/PixGallerySprite.h"
#include "../Model/MyAlbumModel.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Common.h"

PixAlbumSelectUsedPhotoCell* PixAlbumSelectUsedPhotoCell::create(MyAlbumModel *pModel)
{
    PixAlbumSelectUsedPhotoCell* widget = new (std::nothrow) PixAlbumSelectUsedPhotoCell(pModel);
    if (widget && widget->init())
    {
        widget->createCustomItem();
        widget->autorelease();
        return widget;
    }
    
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixAlbumSelectUsedPhotoCell::PixAlbumSelectUsedPhotoCell(MyAlbumModel *pModel)
: _customItem(nullptr)
, menu(nullptr)
, mAlbumModel(nullptr)
, mbln_Isloading(false)
{
    this->mAlbumModel = pModel;
    
    albumUsedImageModelVec.clear();
    downloaderQueue.clear();
}

PixAlbumSelectUsedPhotoCell::~PixAlbumSelectUsedPhotoCell()
{
    for (auto iter = albumUsedImageModelVec.begin(); iter != albumUsedImageModelVec.end(); ++iter)
    {
        (*iter)->delete_S_ImageLoader();
    }
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        (*iter)->release();
    }
    
    downloaderQueue.clear();
    albumUsedImageModelVec.clear();
}

bool PixAlbumSelectUsedPhotoCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}
void PixAlbumSelectUsedPhotoCell::onEnter()
{
    Widget::onEnter();
}

void PixAlbumSelectUsedPhotoCell::onEnterTransitionDidFinish()
{
    Widget::onEnterTransitionDidFinish();
}

void PixAlbumSelectUsedPhotoCell::onExit()
{
    Widget::onExit();
}

void PixAlbumSelectUsedPhotoCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
    
    if(_isTransitionFinished)
    {
        if(visible)
        {
            if(!albumUsedImageModelVec.empty())
            {
                if(getLoading() == false)
                {
                    setLoading(true);
                    
                    createUsedPhoto();
                }
            }
        }
        else
        {
            
        }
    }
}

void PixAlbumSelectUsedPhotoCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("07_My_AlbumList/AlbumSelect_UsedPhoto_cell.csb");
    
    setContentSize(_customItem->getContentSize());
    
    for (int i = 0 ; i < ALBUMSELECT_THUMB_COUNT; ++i)
    {
        std::string strItemName = StringUtils::format("item_%d", i);
        
        Node *item = dynamic_cast<Node*>(_customItem->getChildByName(strItemName));
        
        if(item)
        {
            std::string normalImg = "PixGalleryLevel/selectphoto_frame_3.png";
            std::string pressedImg = "PixGalleryLevel/selectphoto_frame_2.png";
            std::string disabledImg = "";
            
            auto menuItem = MenuItemImage::create(normalImg, pressedImg, pressedImg,
                                                  CC_CALLBACK_1(PixAlbumSelectUsedPhotoCell::cellPressed, this));
            menu = PixScrollMenu::create(menuItem, nullptr);
            
            menu->setPosition(Vec2(item->getContentSize().width / 2, item->getContentSize().height / 2));
            menu->setAnchorPoint(Vec2::ZERO);
            menu->setContentSize(item->getContentSize());
            item->addChild(menu, 100);
        }
    }
    
    this->addChild(_customItem);
}

void PixAlbumSelectUsedPhotoCell::pushMyAlbumUsedImageModel(MyAlbumUsedImageModel* pModel)
{
    albumUsedImageModelVec.push_back(pModel);
}

void PixAlbumSelectUsedPhotoCell::createUsedPhoto()
{
    if(getAlbumModel() == nullptr)
        return;
    
    for(int i = 0; i < albumUsedImageModelVec.size(); ++i)
    {
        MyAlbumUsedImageModel *albumUsedImageModel = albumUsedImageModelVec.at(i);

        if(albumUsedImageModel)
        {
            std::string strItemName = StringUtils::format("item_%d", i);
            Node *item = dynamic_cast<Node*>(_customItem->getChildByName(strItemName));
            
            if(item)
            {
                albumUsedImageModel->setItem(item);
            }
            
            if(!albumUsedImageModel->get_S_Data())
            {
                std::string sUrl = albumUsedImageModel->get_s_image();
                sendGetUsedThumbImageUrl(sUrl, albumUsedImageModel);
            }
            else if(albumUsedImageModel->get_S_Data())
            {
                 albumUsedImageModel->add_S_ImageAsync(this, imageloader_selector(PixAlbumSelectUsedPhotoCell::addUsedThumbImageAsyncCallBack), albumUsedImageModel);
            }
        }
    }
}

void PixAlbumSelectUsedPhotoCell::addUsedThumbImageAsyncCallBack(Texture2D *pTexture, void* pUserData)
{
    if(pTexture == nullptr)
        return;
 
    if(pUserData == nullptr)
        return;

    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
    
        if(getAlbumModel())
        {
            MyAlbumUsedImageModel *pModel = static_cast<MyAlbumUsedImageModel*>(pUserData);
            
            if(pModel)
            {
                Node *item = pModel->getItem();
                
                if(item)
                {
                    PixGallerySprite *sprite = PixGallerySprite::createWithTexture(pTexture, "Common/photo_thum_177X177.png");
                    
                    sprite->setPosition(Vec2(item->getContentSize().width / 2,
                                             item->getContentSize().height / 2));
                    
                    ui::ImageView *gray = dynamic_cast<ui::ImageView*>(item->getChildByName("photo_thum_Gray177X177_1"));
                    
                    if(gray)
                    {
                        sprite->setloading(gray);
                        //                    gray->removeFromParentAndCleanup(true);
                    }
                    
                    if(item->getChildByTag(1000))
                    {
                        item->removeChildByTag(1000);
                    }
                    
                    item->addChild(sprite, 10, 1000);
                }
                
                pModel->destroy_S_Data();
            }
        }

    });
}

Node* PixAlbumSelectUsedPhotoCell::getCellItem(const std::string &name)
{
    if(_customItem != nullptr)
    {
        return _customItem->getChildByName(name);
    }
    
    return nullptr;
}

void PixAlbumSelectUsedPhotoCell::removeCellItem(Node *pItme)
{
    if(_customItem != nullptr && pItme != nullptr)
    {
        _customItem->removeChild(pItme);
    }
}

MyAlbumUsedImageModel* PixAlbumSelectUsedPhotoCell::getMyAlbumUsedImageModel(int nIndex)
{
    if (nIndex < 0 || nIndex >= albumUsedImageModelVec.size())
    {
        return nullptr;
    }
    
    return albumUsedImageModelVec.at(nIndex);
}


bool PixAlbumSelectUsedPhotoCell::isItemIndex(int nIndex)
{
    for (auto iter = albumUsedImageModelVec.begin(); iter != albumUsedImageModelVec.end(); ++iter)
    {
        if((*iter)->getIndex() == nIndex)
        {
            return true;
        }
    }
    
    return false;
}

#pragma mark - Button

void PixAlbumSelectUsedPhotoCell::cellPressed(cocos2d::Ref *pSender)
{
    
}

#pragma mark - Network

void PixAlbumSelectUsedPhotoCell::sendGetUsedThumbImageUrl(const std::string &url, MyAlbumUsedImageModel *pModel)
{
    PixDownloader *pixDownloader = PixDownloader::create();
    pixDownloader->setDelegate(this);
    pixDownloader->setWrite(false);
    pixDownloader->setConnectionTimeout(3);
    pixDownloader->setDownloadeUrl(url.c_str());
    pixDownloader->setUserData(pModel);
    pixDownloader->createDownloadDataTask("AlbumCoverImage");
    
    downloaderQueue.push_back(pixDownloader);
}

#pragma mark - Downloade Delegate

void PixAlbumSelectUsedPhotoCell::onError(PixDownloader *downloader, const std::string& identifier, int errorCode)
{
    auto scheduler = cocos2d::Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([=](void) -> void {
        
        for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
        {
            if((*iter) == downloader)
            {
                (*iter)->release();
                downloaderQueue.erase(iter);
                break;
            }
        }
        
    });
   
}

void PixAlbumSelectUsedPhotoCell::onProgress(PixDownloader *downloader, const std::string& identifier, int percent)
{
    
}

void PixAlbumSelectUsedPhotoCell::onSuccess(PixDownloader *downloader, const std::string& identifier, std::vector<unsigned char>& data)
{
    MyAlbumUsedImageModel *pModel = static_cast<MyAlbumUsedImageModel*>(downloader->getUserData());
    
    if(pModel)
    {
        unsigned char *buff = (unsigned char*)malloc(sizeof(unsigned char) * data.size());
        std::copy(data.begin(), data.end(), buff);
        
        Data *sData = new Data();
        sData->fastSet(buff, data.size());
        
        pModel->destroy_S_Data();
        pModel->set_S_Data(sData);
        pModel->add_S_ImageAsync(this, imageloader_selector(PixAlbumSelectUsedPhotoCell::addUsedThumbImageAsyncCallBack), pModel);
    }
    
    for (auto iter = downloaderQueue.begin(); iter != downloaderQueue.end(); ++iter)
    {
        if((*iter) == downloader)
        {
            (*iter)->release();
            downloaderQueue.erase(iter);
            break;
        }
    }
}
