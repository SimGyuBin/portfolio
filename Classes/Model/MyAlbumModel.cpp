//
//  MyAlbumModel.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 14..
//
//

#include "MyAlbumModel.h"
#include "../Widget/PixAlbumCoverCell.h"

void usedImageSort(std::vector<MyAlbumUsedImageModel*> &array, bool (*compare)(const MyAlbumUsedImageModel*, const MyAlbumUsedImageModel*))
{
    // 선택정렬 알고리즘
    int k;
    MyAlbumUsedImageModel *temp;
    
    for (int i = 1; i < array.size(); i++)
    {
        temp = array[i];
        k = i;
        
        while (k > 0 && !((*compare)(array[k - 1], temp)) )
        {
            array[k] = array[k - 1];
            k--;
        }
        array[k] = temp;
    }
}

// 생성일 오름차순
bool compareCreateDateLow(const MyAlbumUsedImageModel *left, const MyAlbumUsedImageModel *right)
{
    return left->getFile_create_date() >= right->getFile_create_date();
}

// 생성일 내림차순
bool compareCreateDateHigh(const MyAlbumUsedImageModel *left, const MyAlbumUsedImageModel *right)
{
    return left->getFile_create_date() <= right->getFile_create_date();
}

MyAlbumModel::MyAlbumModel()
: _album_id(0)
, _title("")
, _contents("")
, _created_date("")
, _start_date("")
, _end_date("")
, _card_skin_id(0)
, _card_skin_url("")
, _cart_id(0)
, _total_cnt(0)
, _total_page(0)
, _total_image_count(0)
, _make_flag(false)
, mbln_IsVisible(false)
, mbln_loading(false)
, mCoverCell(nullptr)
{
    clear();
}

MyAlbumModel::~MyAlbumModel()
{
    clear();
}

MyAlbumModel* MyAlbumModel::clone()
{
    MyAlbumModel *clone = new MyAlbumModel();
 
    clone->_album_id = _album_id;
    clone->_title = _title;
    clone->_contents = _contents;
    clone->_created_date = _created_date;
    clone->_start_date = _start_date;
    clone->_end_date = _end_date;
    clone->_card_skin_id = _card_skin_id;
    clone->_card_skin_url = _card_skin_url;
    clone->_cart_id = _cart_id;
    clone->_total_cnt = _total_cnt;
    clone->_total_page = _total_page;
    clone->_total_image_count = _total_image_count;
    clone->_make_flag = _make_flag;
    clone->mbln_IsVisible = mbln_IsVisible;
    clone->mbln_loading = mbln_loading;
    clone->mCoverCell = mCoverCell;
    
    for(auto iter = myAlbumImageModelVec.begin(); iter != myAlbumImageModelVec.end(); ++iter)
    {
        clone->pushMyAlbumImageModel((*iter)->clone());
    }
    
    for(auto iter = myAlbumPreviewModelVec.begin(); iter != myAlbumPreviewModelVec.end(); ++iter)
    {
        clone->pushMyAlbumPreviewModel((*iter)->clone());
    }
    
    for(auto iter = myAlbumUsedImageModelVec.begin(); iter != myAlbumUsedImageModelVec.end(); ++iter)
    {
        clone->pushMyAlbumUsedImageModel((*iter)->clone());
    }
    
    return clone;
}

void MyAlbumModel::clear()
{    
    myAlbumImageModelVecClear();
    myAlbumPreviewModelClear();
    myAlbumUsedImageModelVecClear();
}

void MyAlbumModel::myAlbumImageModelVecClear()
{
    for(auto iter = myAlbumImageModelVec.begin(); iter != myAlbumImageModelVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    myAlbumImageModelVec.clear();
}

void MyAlbumModel::myAlbumPreviewModelClear()
{
    for(auto iter = myAlbumPreviewModelVec.begin(); iter != myAlbumPreviewModelVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    myAlbumPreviewModelVec.clear();
}

void MyAlbumModel::myAlbumUsedImageModelVecClear()
{
    for(auto iter = myAlbumUsedImageModelVec.begin(); iter != myAlbumUsedImageModelVec.end(); ++iter)
    {
        CC_SAFE_DELETE((*iter));
    }
    
    myAlbumUsedImageModelVec.clear();
}

void MyAlbumModel::removeImageData()
{
    for(auto iter = myAlbumImageModelVec.begin(); iter != myAlbumImageModelVec.end(); ++iter)
    {
        (*iter)->deleteThreadSafeImageLoader();
//        (*iter)->clear();
    }

    for(auto iter = myAlbumPreviewModelVec.begin(); iter != myAlbumPreviewModelVec.end(); ++iter)
    {
        (*iter)->deleteThreadSafeImageLoader();
//        (*iter)->clear();
    }
    
    for(auto iter = myAlbumUsedImageModelVec.begin(); iter != myAlbumUsedImageModelVec.end(); ++iter)
    {
        (*iter)->deleteThreadSafe_S_ImageLoader();
        (*iter)->deleteThreadSafe_M_ImageLoader();
    }
    
    if(mCoverCell)
    {
        mCoverCell->removeAlbumImages();
    }
    
    setLoading(false);
}

void MyAlbumModel::pushMyAlbumImageModel(PixMyAlbumImageModel *pModel)
{
    myAlbumImageModelVec.push_back(pModel);
}

PixMyAlbumImageModel *MyAlbumModel::getMyAlbumImageModel(int nIndex)
{
    if (nIndex < 0 || nIndex >= myAlbumImageModelVec.size())
    {
        return nullptr;
    }
    
    return myAlbumImageModelVec.at(nIndex);
}

PixMyAlbumImageModel *MyAlbumModel::getMyAlbumImageModelByUrl(const std::string &url)
{
    for(auto iter = myAlbumImageModelVec.begin(); iter != myAlbumImageModelVec.end(); ++iter)
    {
        if((*iter)->getUrl().compare(url) == 0)
        {
            return (*iter);
        }
    }
    
    return nullptr;
}

int MyAlbumModel::getMyAlbumImageModelSeqByIndex(PixMyAlbumImageModel *pModel)
{
    for (int i = 0; i < myAlbumImageModelVec.size(); ++i)
    {
        if(myAlbumImageModelVec.at(i) == pModel)
        {
            return i;
        }
    }
    
    return -1;
}

int MyAlbumModel::getMyAlbumImageCount()
{
    return (int)myAlbumImageModelVec.size();
}

void MyAlbumModel::pushMyAlbumPreviewModel(MyAlbumPreviewModel *pInfo)
{
    myAlbumPreviewModelVec.push_back(pInfo);
}

MyAlbumPreviewModel *MyAlbumModel::getMyAlbumPreviewModel(int nIndex)
{
    if (nIndex < 0 || nIndex >= myAlbumPreviewModelVec.size())
    {
        return nullptr;
    }
    
    return myAlbumPreviewModelVec.at(nIndex);
}

MyAlbumPreviewModel *MyAlbumModel::getMyAlbumPreviewModelByThumbUrl(const std::string &thumbUrl)
{
    if(myAlbumPreviewModelVec.empty())
        return nullptr;

    for(auto iter = myAlbumPreviewModelVec.begin(); iter != myAlbumPreviewModelVec.end(); ++iter)
    {
        if((*iter)->getThumb_url().compare(thumbUrl) == 0)
        {
            return (*iter);
        }
    }
    
    return nullptr;
}

int MyAlbumModel::getMyAlbumPreviewCount()
{
    return (int)myAlbumPreviewModelVec.size();
}

void MyAlbumModel::pushMyAlbumUsedImageModel(MyAlbumUsedImageModel *pInfo)
{
    myAlbumUsedImageModelVec.push_back(pInfo);
}

MyAlbumUsedImageModel *MyAlbumModel::getMyAlbumUsedImageModel(int nIndex)
{
    if (nIndex < 0 || nIndex >= myAlbumUsedImageModelVec.size())
    {
        return nullptr;
    }
    
    return myAlbumUsedImageModelVec.at(nIndex);
}

MyAlbumUsedImageModel *MyAlbumModel::getMyAlbumUsedImageModelBy_S_Url(const std::string &sUrl)
{
    if(myAlbumUsedImageModelVec.empty())
        return nullptr;
        
    for(auto iter = myAlbumUsedImageModelVec.begin(); iter != myAlbumUsedImageModelVec.end(); ++iter)
    {
        if((*iter)->get_s_image().compare(sUrl) == 0)
        {
            return (*iter);
        }
    }

    return nullptr;
}

MyAlbumUsedImageModel *MyAlbumModel::getMyAlbumUsedImageModelBy_M_Url(const std::string &mUrl)
{
    if(myAlbumUsedImageModelVec.empty())
        return nullptr;

    for(auto iter = myAlbumUsedImageModelVec.begin(); iter != myAlbumUsedImageModelVec.end(); ++iter)
    {
        if((*iter)->get_m_image().compare(mUrl) == 0)
        {
            return (*iter);
        }
    }
    
    return nullptr;
}

int MyAlbumModel::getMyAlbumUsedImageModelBySeq(MyAlbumUsedImageModel *pModel)
{
    for(int i = 0; i < myAlbumUsedImageModelVec.size(); ++i)
    {
        if(myAlbumUsedImageModelVec.at(i) == pModel)
        {
            return i;
        }
    }
    
    return -1;
}

std::vector<MyAlbumUsedImageModel*>& MyAlbumModel::getAlbumUsedImageModelVec()
{
    return myAlbumUsedImageModelVec;
}

int MyAlbumModel::getMyAlbumUsedImageCount()
{
    return (int)myAlbumUsedImageModelVec.size();
}

void MyAlbumModel::usedImagesCreateDateLowSort()
{
    usedImageSort(myAlbumUsedImageModelVec, compareCreateDateLow);
    
    int nCount = (int)myAlbumUsedImageModelVec.size();
    
    for(int i = 0; i < nCount; ++i)
    {
        myAlbumUsedImageModelVec.at(i)->setIndex(i);
    }
}

void MyAlbumModel::usedImagesCreateDateHighSort()
{
    usedImageSort(myAlbumUsedImageModelVec, compareCreateDateHigh);
}
