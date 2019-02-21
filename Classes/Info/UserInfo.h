//
//  UserInfo.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 12..
//
//

// 사용자 정보 info

#ifndef UserInfo_h
#define UserInfo_h

#include "cocos2d.h"
#include "../Model/PixImageModel.h"
#include "../Util/PixUserDefault.h"
#include "LocalImageInfo.h"
#include "CategoryInfo.h"
#include "../Model/PixSmartMaskBookModel.h"

class PixDictionary;

typedef enum
{
    USER_NONE = 0,
    USER_GENERAL,
    USER_FACEBOOK,
    USER_KAKAO,
    USER_GOOGLE,
    
} PIX_USER_TYPE;


void localImageSort(std::vector<LocalImageInfo*> &array, bool (*compare)(const LocalImageInfo*, const LocalImageInfo*));
bool comparelocalImageDateHigh(const LocalImageInfo* left, const LocalImageInfo* right);
bool comparelocalImageDateLow(const LocalImageInfo* left, const LocalImageInfo* right);

class UserInfo
{
public :
    
    UserInfo();
    virtual ~UserInfo();
    
    void removeAllImageModel();
    void localImageInfoVecClear();
    
    void smartMaskBookModelVecClear();    
    int getImageModelCount();
    void addImageModel(PixImageModel *pImageModel);
    PixImageModel* getImageModelBySeq(int nSeqIndex);
    bool removeImageModel(PixImageModel *pImageModel);
    PixImageModel* getImageModelBySmartIndex(int nSmartIndex);
    void localImageDataDelete();
    std::vector<LocalImageInfo*>& getlocalImageInfoVec();
    int getLocalImageCount();
    void pushLocalImageInfo(LocalImageInfo *info);
    LocalImageInfo* getLocalImageInfo(int nIndex);
    LocalImageInfo* getLocalImageInfoByPath(const std::string &path);
    int getSmartMaskBookModelCount();
    void pushSmartMaskBookModel(PixSmartMaskBookModel *model);
    PixSmartMaskBookModel* getSmartMaskBookModel(int nIndex);
    void destroyLocalImageData();
    void destroyThumbnailImageData();
    void sortLocalImageDateHigh();
    void sortLocalImageDatelow();
    void localImageVecReverse();
    void addThumbnailData(const std::string &strPath, Data *pData, const std::string &strWidth, const std::string &strHeight)
    ;
    void addFullScreenData(const std::string &strPath, Data *pData, const std::string &strWidth, const std::string &strHeight)
    ;
    
    void setLocalImageDeselect();
    int getLocalImageSelectImageCount();
     
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Member_id, Member_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, strUserID, UserID)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, name, Name)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, phoneNumber, PhoneNumber)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, email, Email)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, vendor, Vendor)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_smartRenderingID, SmartRenderingID)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Grade, Grade)
    CC_SYNTHESIZE(PIX_USER_TYPE, me_UserType, UserType)
    CC_SYNTHESIZE(bool, mbln_Alarm, Alarm)
    CC_SYNTHESIZE(PixDictionary*, mSelectThemeInfo, SelectThemeInfo)
    CC_SYNTHESIZE(PixDictionary*, mSelectThemeChangeInfo, SelectThemeChangeInfo)
    CC_SYNTHESIZE(int, mn_CartCount, CartCount)
    CC_SYNTHESIZE(bool, mbln_NoticeNew, NoticeNew)
    CC_SYNTHESIZE(bool, mbln_EventNew, EventNew)
    CC_SYNTHESIZE(bool, mbln_ShowMembershipEventLayer, ShowMembershipEventLayer)
    CC_SYNTHESIZE(bool, mbln_ShowMyAlbumEvent, ShowMyAlbumEvent)
    
    
private:
    
    // 화면에 보여주기 위한 모델 해쉬맵
    std::unordered_map<int, PixImageModel*> imageModels;
    // 로컬이미지 정보 리스트
    std::vector<LocalImageInfo*> localImageInfoVec;
    // 저장된 앨범 리스트
    std::vector<PixSmartMaskBookModel*> smartMaskBookModelVec;
};

#endif /* UserInfo_h */


