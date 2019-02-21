//
//  MyAlbumModel.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 14..
//
//

#ifndef MyAlbumModel_h
#define MyAlbumModel_h

#include "cocos2d.h"
#include "PixMyAlbumImageModel.h"
#include "PixSmartMaskBookModel.h"
#include "MyAlbumPreviewModel.h"
#include "MyAlbumUsedImageModel.h"


USING_NS_CC;

void usedImageSort(std::vector<MyAlbumUsedImageModel*> &array, bool (*compare)(const MyAlbumUsedImageModel*, const MyAlbumUsedImageModel*));
bool compareCreateDateLow(const MyAlbumUsedImageModel *left, const MyAlbumUsedImageModel *right);
bool compareCreateDateHigh(const MyAlbumUsedImageModel *left, const MyAlbumUsedImageModel *right);

class PixAlbumCoverCell;

class MyAlbumModel : public Ref
{
    
public:
    
    MyAlbumModel();
    virtual ~MyAlbumModel();
    MyAlbumModel* clone();
    void clear();
    void myAlbumImageModelVecClear();
    void myAlbumPreviewModelClear();
    void myAlbumUsedImageModelVecClear();
    void removeImageData();
    
    void pushMyAlbumImageModel(PixMyAlbumImageModel *pModel);
    PixMyAlbumImageModel *getMyAlbumImageModel(int nIndex);
    PixMyAlbumImageModel *getMyAlbumImageModelByUrl(const std::string &url);
    int getMyAlbumImageModelSeqByIndex(PixMyAlbumImageModel *pModel);
    int getMyAlbumImageCount();
    void pushMyAlbumPreviewModel(MyAlbumPreviewModel *pInfo);
    MyAlbumPreviewModel *getMyAlbumPreviewModel(int nIndex);
    MyAlbumPreviewModel *getMyAlbumPreviewModelByThumbUrl(const std::string &thumbUrl);
    int getMyAlbumPreviewCount();
    void pushMyAlbumUsedImageModel(MyAlbumUsedImageModel *pInfo);
    MyAlbumUsedImageModel *getMyAlbumUsedImageModel(int nIndex);
    MyAlbumUsedImageModel *getMyAlbumUsedImageModelBy_S_Url(const std::string &sUrl);
    MyAlbumUsedImageModel *getMyAlbumUsedImageModelBy_M_Url(const std::string &mUrl);
    int getMyAlbumUsedImageModelBySeq(MyAlbumUsedImageModel *pModel);
    std::vector<MyAlbumUsedImageModel*>& getAlbumUsedImageModelVec();
    int getMyAlbumUsedImageCount();
    
    void usedImagesCreateDateLowSort();
    void usedImagesCreateDateHighSort();
    
    void setVisible(bool visible)
    {
        mbln_IsVisible = visible;
    }
    
    inline bool isVisible()
    {
        return mbln_IsVisible;
    }
    
private:
    
    CC_SYNTHESIZE(int, _album_id, Album_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _title, Title)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _contents, Contents)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _created_date, Created_date)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _start_date, Start_date)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _end_date, End_date)
    CC_SYNTHESIZE(int, _card_skin_id, Card_skin_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _card_skin_url, Card_skin_url)
    CC_SYNTHESIZE(int, _cart_id, Cart_id)
    
    CC_SYNTHESIZE(int, _total_cnt, UsedImagCount)
    CC_SYNTHESIZE(int, _total_page, Total_page)
    CC_SYNTHESIZE(int, _total_image_count, Total_image_count)
    CC_SYNTHESIZE(bool, _make_flag, Make_flag)
    
    std::vector<PixMyAlbumImageModel*> myAlbumImageModelVec;
    std::vector<MyAlbumPreviewModel*> myAlbumPreviewModelVec;
    std::vector<MyAlbumUsedImageModel*> myAlbumUsedImageModelVec;
    
    bool mbln_IsVisible;
    CC_SYNTHESIZE(bool, mbln_loading, Loading)
    CC_SYNTHESIZE(PixAlbumCoverCell*, mCoverCell, CoverCell)
};



#endif /* MyAlbumModel_h */
