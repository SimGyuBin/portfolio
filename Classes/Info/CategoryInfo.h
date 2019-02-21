//
//  CategoryInfo.h
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 14..
//
//

#ifndef CategoryInfo_h
#define CategoryInfo_h

#include "cocos2d.h"

class PixImageModel;

class CategoryInfo
{
    
public:
    
    CategoryInfo()
    {
        mstr_CategoryPath.clear();
        mstr_CategoryName.clear();
        imageModelVec.clear();
    }
    
    int getImageModelCount()
    {
        return (int)imageModelVec.size();
    }
    
    void pushImageModel(PixImageModel *pModel)
    {
        imageModelVec.push_back(pModel);
    }
    
    PixImageModel* getImageModel(int nIndex)
    {
        if (nIndex < 0 || nIndex >= imageModelVec.size())
        {
            return nullptr;
        }
        
        return imageModelVec.at(nIndex);
    }
    
    void imageModelDateHighSort();
   
private:
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_CategoryPath, CategoryPath)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_CategoryName, CategoryName)
    std::vector<PixImageModel*> imageModelVec;
};

void categorySort(std::vector<CategoryInfo*> &array, bool (*compare)(const CategoryInfo*, const CategoryInfo*));
bool compareCategoryHigh(const CategoryInfo* left, const CategoryInfo* riget);
bool compareCategoryLow(const CategoryInfo* left, const CategoryInfo* riget);

#endif /* CategoryInfo_h */
