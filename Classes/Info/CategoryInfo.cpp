//
//  CategoryInfo.cpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 14..
//
//

#include "CategoryInfo.h"
#include "../Manager/PixSmartAlbumManager.h"

void CategoryInfo::imageModelDateHighSort()
{
    imageModelSort(imageModelVec, compareImageModelDateHigh);
}

void categorySort(std::vector<CategoryInfo*> &array, bool (*compare)(const CategoryInfo*, const CategoryInfo*))
{
    // 선택정렬 알고리즘
    int k;
    CategoryInfo *temp;
    
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

bool compareCategoryHigh(const CategoryInfo* left, const CategoryInfo* riget)
{
    return left->getCategoryName() >= riget->getCategoryName();
}

bool compareCategoryLow(const CategoryInfo* left, const CategoryInfo* riget)
{
    return left->getCategoryName() <= riget->getCategoryName();
}
