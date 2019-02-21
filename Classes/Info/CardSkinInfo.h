//
//  CardSkinInfo.hpp
//  Pzle
//
//  Created by 심규빈 on 2016. 12. 27..
//
//

#ifndef CardSkinInfo_h
#define CardSkinInfo_h

#include "cocos2d.h"

class CardSkinInfo
{
    
public:
    
    CardSkinInfo() : mn_card_skin_id(0), mstr_Card_skin_url(""), mstr_Card_skin_thumb_url("")
    {
        
    }
    
    CC_SYNTHESIZE(int, mn_card_skin_id, Card_skin_id)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Card_skin_url, Card_skin_url)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mstr_Card_skin_thumb_url, Card_skin_thumb_url)
};

#endif /* CardSkinInfo_h */
