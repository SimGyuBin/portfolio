//
//  PixEditManager.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 26..
//
//

#ifndef PixEditManager_h
#define PixEditManager_h

#include "cocos2d.h"
#include "../Common.h"

#define EDIT_MANAGER     (PixEditManager::sharedObject())

USING_NS_CC;

class PixEditSprite;
class PixSmartMaskImageModel;
class PixSmartImageModel;

class PhotoEditInfo
{
public:
    
    PhotoEditInfo() : mSmartImageModel(nullptr) {};
    
    CC_SYNTHESIZE(PixSmartImageModel*, mSmartImageModel, SmartImageModel)
};

class CC_DLL PixEditManager : public Ref
{
public :
    
    static PixEditManager* sharedObject();
    
    PixEditManager();
    virtual ~PixEditManager();
    
    void clear();
    
    void photoUsedVecClear();
    void photoUnusedVecClear();
    
    void pushPhotoUnused(PhotoEditInfo *pInfo);
    PhotoEditInfo* getPhotoUnused(int nIndex);
    bool reomvePhotoUnused(PhotoEditInfo *pInfo);
    int getPhotoUnusedCount();
    
    void pushPhotoUsed(PhotoEditInfo *pInfo);
    PhotoEditInfo* getPhotoUsed(int nIndex);
    bool reomvePhotoUsed(PhotoEditInfo *pInfo);
    int getPhotoUsedCount();
    
    void pushSmartImageModelTemp(PixSmartImageModel *pImageModel);
    PixSmartImageModel* getSmartImageModelTemp(bool isWidth);
    void smartImageModelTempClear();
    void sortSmartImageModelTempDateHigh();
    void sortSmartImageModelTempLow();
    
private :
    
    std::vector<PhotoEditInfo*> photoUnusedVec;
    std::vector<PhotoEditInfo*> photoUsedVec;
    
    std::vector<PixSmartImageModel*> smartImageModelTempVec;
    
};


#endif /* PixEditManager_h */
