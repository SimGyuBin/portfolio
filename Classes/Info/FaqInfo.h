//
//  FaqInfo.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 6. 21..
//
//

#ifndef FaqInfo_h
#define FaqInfo_h

class FaqInfo
{
public:
    
    FaqInfo()
    {
        mstr_ID.clear();
        mstr_Title.clear();
        mn_Kind = 0;
        mstr_Content.clear();
        mstr_File.clear();
        mn_Hit = 0;
        mstr_Date.clear();
    }
    
    ~FaqInfo() {}
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_ID, ID)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Title, Title)
    CC_SYNTHESIZE(int, mn_Kind, Kind)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Content, Content)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_File, File)
    CC_SYNTHESIZE(int, mn_Hit, Hit)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Date, Date)
};

class FaqCategoryInfo
{
    
public:
    
    FaqCategoryInfo()
    : mn_ID(0)
    , mstr_Category_name("")
    {
        
    }
    
    ~FaqCategoryInfo()
    {
        
    }
    
    CC_SYNTHESIZE(int, mn_ID, ID)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Category_name, Category_name)
};

#endif /* FaqInfo_h */
