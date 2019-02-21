//
//  PixNetworkImageModelDelegate.hpp
//  Pzle
//
//  Created by 심규빈 on 2017. 4. 27..
//
//

#ifndef PixNetworkImageModelDelegate_h
#define PixNetworkImageModelDelegate_h

#include "cocos2d.h"

USING_NS_CC;

class PixNetworkImageModelDelegate
{
public:
    virtual void networkImageModelcompleteTexture(Texture2D *pTexture, void *pUserData) {};
};

#endif /* PixNetworkImageModelDelegate_h */
