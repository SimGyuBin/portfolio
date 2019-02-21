//
//  PixCSLoaderManager.hpp
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 11..
//
//

#ifndef PixCSLoaderManager_h
#define PixCSLoaderManager_h

#include "cocos2d.h"

#define CSLOADER_MANAGER     (PixCSLoaderManager::sharedObject())

USING_NS_CC;

class PixCSLoaderManager
{
public:
    static PixCSLoaderManager* sharedObject();
    
    PixCSLoaderManager();
    virtual ~PixCSLoaderManager();
    
    void init();
    
    Node* getCustomItem(const std::string &filename, const std::string &childname = "Panel");
    Node* getNode(int type);
    
private:
    
    std::unordered_map<std::string, Node*> customItemMap;
    std::unordered_map<int, Node*> nodeMap;
};

#endif /* PixCSLoaderManager_h */


