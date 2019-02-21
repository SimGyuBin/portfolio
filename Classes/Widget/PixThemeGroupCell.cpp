//
//  PixThemeGroupCell.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 8. 24..
//
//

#include "PixThemeGroupCell.h"
#include "PixThemeCell.h"
#include "../Manager/PixCSLoaderManager.h"
#include "../Util/PixUtils.h"
#include "../Util/PixStringUtils.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Layer/PixThemeTipLayer.h"
#include "../Layer/PixThemePreviewLayer.h"
#include "../Scene/PixApp.h"

PixThemeGroupCell* PixThemeGroupCell::create(PixDictionary *pGroup, Node *pTopLayer, Node *pBottomLayer, bool isThemeChange)
{
    PixThemeGroupCell* widget = new (std::nothrow) PixThemeGroupCell(pGroup, pTopLayer, pBottomLayer, isThemeChange);
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

PixThemeGroupCell::PixThemeGroupCell(PixDictionary *pGroup, Node *pTopLayer, Node *pBottomLayer, bool isThemeChange)
: _customItem(nullptr)
, mGroup((nullptr))
, mBottomLayer(nullptr)
, mDelegate(nullptr)
, mThemePreviewLayerDelegate(nullptr)
, themeListView(nullptr)
, mbln_ThemeChange(isThemeChange)
{
    this->mGroup = pGroup;
    this->mTopLayer = pTopLayer;
    this->mBottomLayer = pBottomLayer;
}

PixThemeGroupCell::~PixThemeGroupCell()
{
    
}

bool PixThemeGroupCell::init()
{
    if(Widget::init())
    {
        return true;
    }
    
    return false;
}

void PixThemeGroupCell::onEnter()
{
    Widget::onEnter();
    
}

void PixThemeGroupCell::setVisible(bool visible)
{
    Widget::setVisible(visible);
}

void PixThemeGroupCell::createCustomItem()
{
    _customItem = CSLOADER_MANAGER->getCustomItem("03_Theme/ThemePage.csb");
    
    if(_customItem)
    {
        setContentSize(_customItem->getContentSize());
        
//        ThemeHeader = dynamic_cast<Node*>(_customItem->getChildByName("ThemeHeader"));
        
        ui::ListView *listView = dynamic_cast<ui::ListView*>(_customItem->getChildByName("listView"));
        
        if(listView)
        {
            themeListView = PixThemeListView::create();
            themeListView->copyClonedListView(listView);
//            themeListView->setTopLayer(ThemeHeader, true);
//            themeListView->setSelectListItemCallBack(CC_CALLBACK_3(PixThemeGroupCell::themeListViewTouchCallBack, this));
//            themeListView->setTouchEndedCallBack(CC_CALLBACK_2(PixThemeGroupCell::themeListViewTouchEndedCallback, this));
            
            settingthemeListView();
            
            themeListView->setBottomLayer(mBottomLayer, true);
        }
        
        this->addChild(_customItem);
    }
}

void PixThemeGroupCell::setThemeListViewTouchEnabled(bool enabled)
{
    if(themeListView)
    {
        if(themeListView->isTouchEnabled() != enabled)
        {
            themeListView->setTouchEnabled(enabled);
        }
    }
}

void PixThemeGroupCell::themeListViewScrollToTop()
{
    if(themeListView)
    {
        themeListView->jumpToTop();
    }
}

void PixThemeGroupCell::setThemeCellSelected(bool isCellSelected)
{
    int nItemCount = (int)themeListView->getChildrenCount();
    
    for(int i = 0; i < nItemCount; ++i)
    {
        PixThemeCell *themeCell = dynamic_cast<PixThemeCell*>(themeListView->getChildren().at(i));
        
        if(themeCell)
        {
            Node *cell = dynamic_cast<Node*>((themeCell->getChildren()).at(0));
            
            int nCount = (int)cell->getChildrenCount();
            
            for (int j = 0; j < nCount; j++)
            {
                
                PixDictionary *info = themeCell->getThemeInfo(j);
                
                if(info)
                {
                    themeCell->setCellSelected(j, isCellSelected);
                }
            }
        }
    }
}

bool PixThemeGroupCell::checkCellSelected()
{
    int nItemCount = (int)themeListView->getChildrenCount();
    
    for(int i = 0; i < nItemCount; ++i)
    {
        PixThemeCell *themeCell = dynamic_cast<PixThemeCell*>(themeListView->getChildren().at(i));
        
        if(themeCell)
        {
            Node *cell = dynamic_cast<Node*>((themeCell->getChildren()).at(0));
            
            int nCount = (int)cell->getChildrenCount();
            
            for (int j = 0; j < nCount; j++)
            {
                if(themeCell->isCellSelected(j))
                {
                    return true;
                }
            }
        }
    }
    
    return false;
}

void PixThemeGroupCell::settingthemeListView()
{
    if(mGroup)
    {
//        if(mGroup)
//        {
//            PixDictionary *imageDic = mGroup->dictionaryForKey("image_path");
//            
//            if(imageDic)
//            {
//                std::string image_path = imageDic->stringForKey(SMARTALBUM_MANAGER->getLanguageCode());
//                
//                if(image_path.empty())
//                {
//                    image_path = imageDic->stringForKey(DEFAULT_LANGUAGE);
//                }
//                
//                if(!image_path.empty())
//                {
//                        std::vector<std::string> tokens;
//                        PixStringUtils::getTokens(tokens, image_path, "/");
//                        
//                        std::string filename;
//                        filename.clear();
//                        
//                        if(!tokens.empty())
//                        {
//                            filename = tokens[tokens.size() - 1];
//                        }
//                        
//                        std::string fullPath = std::string(THEME_PATH).append(filename);
//                        
//                        Sprite *headerSprite = Sprite::create(fullPath);
//                        
//                        Size size = headerSprite->getContentSize();
//                        if(headerSprite)
//                        {
//                            if(ThemeHeader)
//                            {
//                                headerSprite->setPosition(Vec2(ThemeHeader->getContentSize().width * 0.5f,
//                                                               ThemeHeader->getContentSize().height * 0.5f));
//                                headerSprite->setAnchorPoint(Vec2(0.5f,0.5f));
//                                ThemeHeader->addChild(headerSprite);
//
//                            }
//                        }
//                }
//            }
//        }
        
        
//        Size size = _customItem->getContentSize();
//        ui::Widget *topMargin = ui::Widget::create();
//        topMargin->setContentSize(Size(size.width,30.0f));
//        themeListView->pushBackCustomItem(topMargin);
        
        PixArray *children = mGroup->arrayForKey("children");
        
        if(children)
        {
            if(getThemeChange())
            {
                int nArrayCount = (int)children->count();
               
                std::vector<PixDictionary*> themeVec;
                std::string strSelect_theme_id;
                
                PixDictionary *pSelectThemeInfo = PIXAPP->userInfo->getSelectThemeInfo();
                
                if(pSelectThemeInfo)
                {
                    PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(pSelectThemeInfo);
                    
                    if(leaf)
                    {
                        strSelect_theme_id = leaf->stringForKey("theme_id");
                    }
                }
                
                for (int i = 0; i < nArrayCount; ++i)
                {
                    PixDictionary *pDic = children->getDictionaryAtIndex(i);
                    
                    if(pDic)
                    {
                        std::string strTheme_id;

                        PixDictionary *leaf = SMARTALBUM_MANAGER->getLeafDictionary(pDic);

                        if(leaf)
                        {
                            strTheme_id = leaf->stringForKey("theme_id");
                            
                            if(strSelect_theme_id.compare(strTheme_id) == 0)
                            {
                                
                            }
                            else
                            {
                                themeVec.push_back(pDic);
                            }
                        }
                    }
                }
                
                int nCellCount = (int)themeVec.size();
                int nCount = nCellCount / SELECT_THEME_COUNT + ((nCellCount % SELECT_THEME_COUNT) > 0 ? 1 : 0);
                int nAddItemCount = 0;
                
                for (int i = 0; i < nCount; ++i)
                {
                    PixThemeCell *themeCell = PixThemeCell::create(getThemeChange());
                    PixDictionary *pDic = nullptr;
                    
                    for (int j = 0; j < SELECT_THEME_COUNT; j++)
                    {
                        if (nAddItemCount == nCellCount)
                            break;
                        
                        pDic = themeVec.at(nAddItemCount);
                        
                        if(pDic)
                        {
                            themeCell->pushDefaultInfo(pDic);
                        }
                        
                        ++nAddItemCount;
                    }
                    
                    themeCell->createCustomItem();
                    
                    if (i == (nCount - 1))
                    {
                        int nRemoveItemCount =  ( (i + 1) * SELECT_THEME_COUNT) - nCellCount;
                        
                        if (nRemoveItemCount > 0)
                        {
                            int nRemoveItemNum = (SELECT_THEME_COUNT - 1);
                            
                            for (int j = (nRemoveItemCount - 1); j >= 0; --j, --nRemoveItemNum)
                            {
                                std::string strItemName = StringUtils::format("item_%d", nRemoveItemNum);
                                Node *item = themeCell->getCellItem(strItemName);
                                
                                if (item)
                                {
                                    themeCell->removeCellItem(item);
                                }
                            }
                        }
                    }
                    
                    themeListView->pushBackCustomItem(themeCell);
                }
            }
            else
            {
                int nCellCount = (int)children->count();
                int nCount = nCellCount / SELECT_THEME_COUNT + ((nCellCount % SELECT_THEME_COUNT) > 0 ? 1 : 0);
                int nAddItemCount = 0;
                
                for (int i = 0; i < nCount; ++i)
                {
                    PixThemeCell *themeCell = PixThemeCell::create(getThemeChange());
                    PixDictionary *pDic = nullptr;
                    
                    for (int j = 0; j < SELECT_THEME_COUNT; j++)
                    {
                        if (nAddItemCount == nCellCount)
                            break;
                        
                        pDic = children->getDictionaryAtIndex(nAddItemCount);
                        
                        if(pDic)
                        {
                            themeCell->pushDefaultInfo(pDic);
                        }
                        
                        ++nAddItemCount;
                    }
                    
                    themeCell->createCustomItem();
                    
                    if (i == (nCount - 1))
                    {
                        int nRemoveItemCount =  ( (i + 1) * SELECT_THEME_COUNT) - nCellCount;
                        
                        if (nRemoveItemCount > 0)
                        {
                            int nRemoveItemNum = (SELECT_THEME_COUNT - 1);
                            
                            for (int j = (nRemoveItemCount - 1); j >= 0; --j, --nRemoveItemNum)
                            {
                                std::string strItemName = StringUtils::format("item_%d", nRemoveItemNum);
                                Node *item = themeCell->getCellItem(strItemName);
                                
                                if (item)
                                {
                                    themeCell->removeCellItem(item);
                                }
                            }
                        }
                    }
                    
                    themeListView->pushBackCustomItem(themeCell);
                }
            }
           
        }
        
        Size size = _customItem->getContentSize();
        ui::Widget *bottomMargin = ui::Widget::create();
        bottomMargin->setContentSize(Size(size.width,50.0f));
        themeListView->pushBackCustomItem(bottomMargin);
    }
}

#pragma mark - Touch

void PixThemeGroupCell::themeListViewTouchEndedCallback(Touch *touch, Event *unusedEvent)
{
//    if(ThemeHeader)
//    {
//        if(NodeContainsPoint(ThemeHeader, touch->getLocation()))
//        {
//            return;
//        }
//    }
    
    for (int i = 0; i < themeListView->getChildrenCount(); ++i)
    {
        Node *child = themeListView->getChildren().at(i);
        
        if(child)
        {
            PixThemeCell *themeCell = dynamic_cast<PixThemeCell*>(child);
            
            if (themeCell)
            {
                if(getDelegate())
                {
                    getDelegate()->themeCellSelectedInitialize();
                }
                
                PixDictionary *selectInfo = nullptr;
                
                if (NodeContainsPoint(themeCell, touch->getLocation()))
                {
                    for (int i = 0 ; i < SELECT_THEME_COUNT; ++i)
                    {
                        std::string strItemName = StringUtils::format("item_%d", i);
                        
                        Node *item = themeCell->getCellItem(i);

                        if(item)
                        {
//                            if (NodeContainsPoint(previewImage, touch->getLocation()))
//                            {
//                                Scene *scene = PIXAPP->isScene;
//                                
//                                if(scene)
//                                {
//                                    PixThemePreviewLayer *themePreviewLayer = PixThemePreviewLayer::create(themeCell->getThemeInfo(i), getThemeChange());
//                                    themePreviewLayer->setDelegate(mThemePreviewLayerDelegate);
//                                    scene->addChild(themePreviewLayer, 101);
//                                }
//                            }
//                            else if (NodeContainsPoint(item, touch->getLocation()))
//                            {
//                                bool blnSelected = !themeCell->isCellSelected(i);
//                                
//                                themeCell->setCellSelected(i, blnSelected);
//                                
//                                if(blnSelected)
//                                {
//                                    selectInfo = themeCell->getThemeInfo(i);
//                                    
//                                    if(selectInfo)
//                                    {
//                                        if(getThemeChange())
//                                        {
//                                            PIXAPP->userInfo->setSelectThemeChangeInfo(selectInfo);
//                                        }
//                                        else
//                                        {
//                                            PIXAPP->userInfo->setSelectThemeInfo(selectInfo);
//                                            
//                                            // 앱설치 후 최고 팁 레이어
//                                            if(PixUserDefault::GetThemeGuide())
//                                            {
//                                                Scene *scene = PIXAPP->isScene;
//                                                
//                                                if(scene)
//                                                {
//                                                    scene->addChild(PixThemeTipLayer::create());
//                                                }
//                                            }
//                                        }
//                                    }
//                                }
//                            }
                            
                            if (NodeContainsPoint(item, touch->getLocation()))
                            {
                                Scene *scene = PIXAPP->isScene;
                                
                                if(scene)
                                {
                                    PixThemePreviewLayer *themePreviewLayer = PixThemePreviewLayer::create(themeCell->getThemeInfo(i), getThemeChange());
                                    themePreviewLayer->setDelegate(mThemePreviewLayerDelegate);
                                    scene->addChild(themePreviewLayer, 101);
                                }
                            }
                        }
                    }
                }

                if(selectInfo)
                {
                    int nItemCount = (int)themeListView->getChildrenCount();
                    
                    for(int i = 0; i < nItemCount; ++i)
                    {
                        PixThemeCell *themeCell = dynamic_cast<PixThemeCell*>(themeListView->getChildren().at(i));
                        
                        if(themeCell)
                        {
                            for (int j = 0 ; j < SELECT_THEME_COUNT; ++j)
                            {
                                PixDictionary *info = themeCell->getThemeInfo(j);
                                
                                if(info)
                                {
                                    if(info != selectInfo)
                                    {
                                        themeCell->setCellSelected(j, false);
                                    }
                                }
                            }
                        }
                    }
                }
                
                if(getDelegate())
                {
                    getDelegate()->themePressed(checkCellSelected());
                }
            }
        }
    }
}

//void PixThemeGroupCell::themeListViewTouchCallBack(cocos2d::Ref *pSender, const std::vector<Vec2>&touchVec, LISTVIEW_TOUCH_TYPE touchType)
//{
//    PixThemeCell *themeCell = dynamic_cast<PixThemeCell*>(pSender);
//    
//    if (themeCell)
//    {
//        if(touchType == LISTVIEW_LONG_TAP)
//        {
//            
//        }
//        else
//        {
//            if(getDelegate())
//            {
//                getDelegate()->themeCellSelectedInitialize();
//            }
//            
//            PixDictionary *selectInfo = nullptr;
//            Node *cell = dynamic_cast<Node*>((themeCell->getChildren()).at(0));
//            
//            if (cell)
//            {
//                int nCount = (int)cell->getChildrenCount();
//                
//                for (int i = 0; i < nCount; i++)
//                {
//                    Node *item = (cell->getChildren()).at(i);
//                    
//                    if (NodeContainsPoint(item, touchVec.at(1)))
//                    {
//                        bool blnSelected = !themeCell->isCellSelected(i);
//                        
//                        themeCell->setCellSelected(i, blnSelected);
//                        
//                        if(blnSelected)
//                        {
//                            selectInfo = themeCell->getThemeInfo(i);
//                            
//                            if(selectInfo)
//                            {
//                                PIXAPP->userInfo->setSelectThemeInfo(selectInfo);
//                            }
//                        }
//                    }
//                }
//            }
//            
//            if(selectInfo)
//            {
//                int nItemCount = (int)themeListView->getChildrenCount();
//                
//                for(int i = 0; i < nItemCount; ++i)
//                {
//                    PixThemeCell *themeCell = dynamic_cast<PixThemeCell*>(themeListView->getChildren().at(i));
//                    
//                    if(themeCell)
//                    {
//                        Node *cell = dynamic_cast<Node*>((themeCell->getChildren()).at(0));
//                        
//                        int nCount = (int)cell->getChildrenCount();
//                        
//                        for (int j = 0; j < nCount; j++)
//                        {
//                            
//                            PixDictionary *info = themeCell->getThemeInfo(j);
//                            
//                            if(info)
//                            {
//                                if(info != selectInfo)
//                                {
//                                    themeCell->setCellSelected(j, false);
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//            
//            if(getDelegate())
//            {
//                getDelegate()->themePressed(checkCellSelected());
//            }
//        }
//    }
//}
