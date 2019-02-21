//
//  PixGalleryTouchControlLayer.h
//  Pzle
//
//  Created by 심규빈 on 2016. 10. 12..
//
//

#ifndef PixGalleryTouchControlLayer_h
#define PixGalleryTouchControlLayer_h

#include "cocos2d.h"
#include "../UIControl/PixModalLayer.h"
#include "../UIControl/PixGestureRecognizer.h"

USING_NS_CC;

class PixGalleryLevel;
class PixAlbumViewLayer;

class PixGalleryTouchControlLayer : public PixModalLayer
{
    
public :
    
    static PixGalleryTouchControlLayer* create(PixGalleryLevel *galleryLevel);
    
    PixGalleryTouchControlLayer();
    virtual ~PixGalleryTouchControlLayer();
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    inline bool isTouchMoveSelectPhotosLayer()
    {
        return mbln_IsTouchMoveSelectPhotosLayer;
    }
    
#pragma mark - Touch & Touches
    
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch* touch, Event* event);
    
#pragma mark - Android Destroy
    
    virtual void androidBackPressed();
    
private :
    
    bool mbln_IsTouchMoveSelectPhotosLayer;
    PixGestureRecognizer *gestureRecognizer;
    Vec2 touchesStart;
    
    CC_SYNTHESIZE(PixGalleryLevel*, mGalleryLevel, GalleryLevel)
    CC_SYNTHESIZE(PixAlbumViewLayer*, mAlbumViewLayer, AlbumViewLayer)
};

#endif /* PixGalleryTouchControlLayer_h */
