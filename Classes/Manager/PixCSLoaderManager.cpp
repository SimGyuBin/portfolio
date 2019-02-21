//
//  PixCSLoaderManager.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 11..
//
//

#include "PixCSLoaderManager.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"

static PixCSLoaderManager* _sharedObject = nullptr;

PixCSLoaderManager* PixCSLoaderManager::sharedObject()
{
    if (nullptr == _sharedObject)
    {
        _sharedObject = new PixCSLoaderManager();
    }
    
    return _sharedObject;

}

PixCSLoaderManager::PixCSLoaderManager()
{
    customItemMap.clear();
}

PixCSLoaderManager::~PixCSLoaderManager()
{
    for (auto iter = customItemMap.begin(); iter != customItemMap.end(); ++iter)
    {
        CC_SAFE_RELEASE((*iter).second);
    }
    
    customItemMap.clear();
    
    for (auto iter = nodeMap.begin(); iter != nodeMap.end(); ++iter)
    {
        CC_SAFE_RELEASE((*iter).second);
    }
    
    nodeMap.clear();
}

Node* PixCSLoaderManager::getCustomItem(const std::string &filename, const std::string &childname)
{
    ui::Widget *widget = nullptr;
    std::string strKey = filename;
    auto it = customItemMap.find(strKey);
    
    if (it != customItemMap.end())
    {
        ui::Widget *child = dynamic_cast<ui::Widget*>(((*it).second)->getChildByName(childname));
        
        if(child)
        {
            widget = child->clone();
        }
    }
    else
    {
        Node *node = CSLoader::createNode(filename);
        
        if(node)
        {
            node->retain();
            
            auto ret = customItemMap.insert(std::make_pair(filename, node));
            
            if (ret.second == false)
            {
                CC_SAFE_RELEASE((ret.first->second));
                customItemMap[filename] = node;
            }
            
            ui::Widget *child = dynamic_cast<ui::Widget*>(node->getChildByName(childname));
            
            if(child)
            {
                widget = child->clone();
            }
        }
    }
    
    return widget;
}

Node* PixCSLoaderManager::getNode(int type)
{
    // 미구현
    Node *node = nullptr;
    
    auto it = nodeMap.find(type);
    
    if (it != nodeMap.end())
    {
        node = ((*it).second);
    }
    else
    {
//        switch (type)
//        {
//            case PIXLOADER_THEMELAYER:
//            {
//                node = createThemeLayer(false);
//            }
//            break;
//                
//            case PIXLOADER_THEMELAYER_SHOWACTION:
//            {
//                node = createThemeLayer(true);
//            }
//            break;
//                
//            default:
//                break;
//        }
        
        if(node)
        {
            node->retain();
            
            auto ret = nodeMap.insert(std::make_pair(type, node));
            
            if (ret.second == false)
            {
                CC_SAFE_RELEASE((ret.first->second));
                nodeMap[type] = node;
            }
        }
    }
    
    return node;
}
