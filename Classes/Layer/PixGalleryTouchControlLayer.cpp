//
//  PixGalleryTouchControlLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 12..
//
//

#include "PixGalleryTouchControlLayer.h"
#include "PixAlbumViewLayer.h"
#include "../Scene/PixGalleryLevel.h"
#include "../Layer/PixSmartalbumSaveLayer.h"
#include "../Util/PixUtils.h"
#include "../Scene/PixApp.h"

PixGalleryTouchControlLayer* PixGalleryTouchControlLayer::create(PixGalleryLevel *galleryLevel)
{
    PixGalleryTouchControlLayer *layer = new (std::nothrow) PixGalleryTouchControlLayer();
    if (layer && layer->init())
    {
        layer->setGalleryLevel(galleryLevel);
        layer->autorelease();
        return layer;
    }
    else
    {
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}

PixGalleryTouchControlLayer::PixGalleryTouchControlLayer()
: mGalleryLevel(nullptr)
, mAlbumViewLayer(nullptr)
, mbln_IsTouchMoveSelectPhotosLayer(false)
, gestureRecognizer(nullptr)
, touchesStart(Vec2::ZERO)
{
    
}

PixGalleryTouchControlLayer::~PixGalleryTouchControlLayer()
{
   
}

bool PixGalleryTouchControlLayer::init()
{
    if (PixModalLayer::init())
    {
        return true;
    }
    
    return false;
}

void PixGalleryTouchControlLayer::onEnter()
{
    PixModalLayer::onEnter();
}

void PixGalleryTouchControlLayer::onExit()
{
    PixModalLayer::onExit();
}

#pragma mark - Touch & Touches

bool PixGalleryTouchControlLayer::onTouchBegan(Touch* touch, Event* event)
{
//    if(mGalleryLevel && mGalleryLevel->getMoveSelectPhotosLayer())
//    {
//        PixAlbumViewLayer *albumViewLayer = getAlbumViewLayer();
//        
//        if(albumViewLayer->isShow() && albumViewLayer->getPagePanelScale() == 1.0f)
//        {
//            if(albumViewLayer && albumViewLayer->isAction() == false)
//            {
//                Node *moveSelectPhotosLayer = mGalleryLevel->getMoveSelectPhotosLayer();
//                
//                if(moveSelectPhotosLayer && moveSelectPhotosLayer->isVisible())
//                {
//                    if(NodeContainsPoint(moveSelectPhotosLayer, touch->getLocation()))
//                    {
//                        touchesStart = touch->getLocation();
//                        mbln_IsTouchMoveSelectPhotosLayer = true;
//                        mGalleryLevel->setMoveSelectPhotosLayerType(MOVE_SELECT_PHOTOS_LAYER_PRESSED);
//                        return true;
//                    }
//                }
//            }
//        }
//    }
    
    return false;
}

void PixGalleryTouchControlLayer::onTouchMoved(Touch* touch, Event* event)
{
    PixModalLayer::onTouchMoved(touch,event);
    
//    if(mbln_IsTouchMoveSelectPhotosLayer)
//    {
//        if(mGalleryLevel)
//        {
//            mGalleryLevel->setMoveSelectPhotosLayerType(MOVE_SELECT_PHOTOS_LAYER_PRESSED);
//            
//            PixAlbumViewLayer *albumViewLayer = mGalleryLevel->getAlbumViewLayer();
//            
//            if(albumViewLayer)
//            {
//                albumViewLayer->moveAlbumViewLayerAction(touch, touchesStart);
//            }
//        }
//    }
}

void PixGalleryTouchControlLayer::onTouchEnded(Touch* touch, Event* event)
{
    PixModalLayer::onTouchEnded(touch,event);
    
//    if(mbln_IsTouchMoveSelectPhotosLayer)
//    {
//        touchesStart = Vec2::ZERO;
//        mbln_IsTouchMoveSelectPhotosLayer = false;
//
//        if(mGalleryLevel)
//        {
//            mGalleryLevel->setMoveSelectPhotosLayerType(MOVE_SELECT_PHOTOS_LAYER_NORMAL);
//            
//            PixAlbumViewLayer *albumViewLayer = mGalleryLevel->getAlbumViewLayer();
//            
//            if(albumViewLayer)
//            {
//                Vec2 difference = touch->getLocation() - touch->getStartLocation();
//                
//                float move = 30.f;
//            
//                if (difference.x > move || difference.y > move || difference.x < -move || difference.y < -move)
//                {
//                     albumViewLayer->moveAlbumViewLayerActionEnd();
//                }
//                else
//                {
//                    if(Director::getInstance()->getRunningScene()->getChildByTag(THEMECHANGE_LAYER_TAG))
//                    {
//                        return;
//                    }
//                    
//                    if(albumViewLayer->isShow())
//                    {
//                        albumViewLayer->hideAlbumViewLayerAction();
//                    }
//                    else
//                    {
//                        albumViewLayer->showAlbumViewLayerAction();
//                    }
//                }
//            }
//        }
//    }
//    else
//    {
//        if(getAlbumViewLayer())
//        {
//
//        }
//    }
}

void PixGalleryTouchControlLayer::onTouchCancelled(Touch* touch, Event* event)
{
    PixModalLayer::onTouchCancelled(touch,event);
    
//    if(mbln_IsTouchMoveSelectPhotosLayer)
//    {
//        touchesStart = Vec2::ZERO;
//        mbln_IsTouchMoveSelectPhotosLayer = false;
//        
//        if(mGalleryLevel)
//        {
//            mGalleryLevel->setMoveSelectPhotosLayerType(MOVE_SELECT_PHOTOS_LAYER_NORMAL);
//        }
//    }
}

#pragma mark - Android Destroy

void PixGalleryTouchControlLayer::androidBackPressed()
{   
    PixAlbumViewLayer *albumViewLayer = getAlbumViewLayer();
    
    if(albumViewLayer && albumViewLayer->isAction() == false)
    {
        if(albumViewLayer->isShow() == false)
        {
            if(getGalleryLevel())
            {
                getGalleryLevel()->cancelEvent();
            }
        }
        else
        {
            if(albumViewLayer->getPagePanelScale() > 1.0f)
            {
                if(albumViewLayer->getPageNavigationVisible())
                {
                    albumViewLayer->hidePanelPageNavigation();
                }
                else
                {
                    albumViewLayer->pagePaneRunAction(false);
                }
            }
            else
            {
                if(albumViewLayer->getNavigationVisible())
                {
                    albumViewLayer->hideNavigation();
                }
                else
                {
                    PixSmartalbumSaveLayer *saveLayer = dynamic_cast<PixSmartalbumSaveLayer*>(Director::getInstance()->getRunningScene()->getChildByTag(ALBUMSAVE_LAYER_TAG));
                    
                    if(saveLayer)
                    {
                        saveLayer->androidBackPressed();
                    }
                    else
                    {
                        PIXAPP->androidDestroy();
                    }
                }
            }
        }
    }
    else
    {
        PIXAPP->androidDestroy();
    }
}
