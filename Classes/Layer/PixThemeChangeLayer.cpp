//
//  PixThemeChangeLayer.cpp
//  Pzle
//
//  Created by 심규빈 on 2017. 5. 9..
//
//

#include "PixThemeChangeLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Widget/PixThemeCell.h"
#include "../Widget/PixThemeHeaderCell.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixThemeChangeLayer* PixThemeChangeLayer::create()
{
    PixThemeChangeLayer *themeChangeLayer = new (std::nothrow) PixThemeChangeLayer();
    if (themeChangeLayer && themeChangeLayer->init())
    {
        themeChangeLayer->autorelease();
        return themeChangeLayer;
    }
    else
    {
        CC_SAFE_DELETE(themeChangeLayer);
        return nullptr;
    }
}

PixThemeChangeLayer::PixThemeChangeLayer()
: rootNode(nullptr)
, ThemeTopLayer(nullptr)
, Node_ThemeCategoryLayer(nullptr)
, themeCategoryLine(nullptr)
, ThemeBottomLayer(nullptr)
, btn_back(nullptr)
, btn_addphoto(nullptr)
, themePageView(nullptr)
, PageDot(nullptr)
, mbln_IsAction(false)
, mDelegate(nullptr)
, mThemeChangeDelegate(nullptr)
, m_AddphotoPos(Vec2::ZERO)
, addphotoAction(nullptr)
{
    pageDotVec.clear();
    categoryButtonVec.clear();
    
    hideLayerFunction = std::bind(&PixThemeChangeLayer::hideLayerAction,this);
}

PixThemeChangeLayer::~PixThemeChangeLayer()
{
    pageDotVec.clear();
    categoryButtonVec.clear();
}

void PixThemeChangeLayer::initLayer()
{
    rootNode = CSLoader::createNode("03_Theme/ThemeLayer_2.csb");
    
    if(rootNode)
    {
        ThemeTopLayer = dynamic_cast<Node*>(rootNode->getChildByName("Node_ThemeTopLayer_2"));
        
        if(ThemeTopLayer)
        {
            ThemeTopLayer->setLocalZOrder(1);
            
            btn_back = dynamic_cast<ui::Button*>(ThemeTopLayer->getChildByName("close"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixThemeChangeLayer::btnBackPressed, this));
            }
            
            ui::Text *title = dynamic_cast<ui::Text*>(ThemeTopLayer->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
        }
        
        Node_ThemeCategoryLayer = dynamic_cast<Node*>(rootNode->getChildByName("Node_ThemeCategoryLayer"));
        
        if(Node_ThemeCategoryLayer)
        {
            ui::Layout *layout = dynamic_cast<ui::Layout*>(Node_ThemeCategoryLayer->getChildByName("Panel"));
            
            if(layout)
            {
                themeCategoryLine = dynamic_cast<ui::Layout*>(layout->getChildByName("Line"));
                
                if(themeCategoryLine)
                {
                    int nCount = SMARTALBUM_MANAGER->getGroupCount();
                    float fWidth = Node_ThemeCategoryLayer->getContentSize().width / nCount;
                    float fheight = themeCategoryLine->getContentSize().height;
                    
                    themeCategoryLine->setContentSize(Size(fWidth,fheight));
                }
                
                ui::Button *button = dynamic_cast<ui::Button*>(layout->getChildByName("Button_1"));
                
                if(button)
                {
                    int nCount = SMARTALBUM_MANAGER->getGroupCount();
                    float fposX = 0;
                    float fposY = button->getPosition().y;
                    float fWidth = Node_ThemeCategoryLayer->getContentSize().width / nCount;
                    float fheight = button->getContentSize().height;
                    
                    for(int i = 0; i < nCount; ++i)
                    {
                        std::string title;
                        PixDictionary *group = SMARTALBUM_MANAGER->getGroupDictionary(i);
                        
                        if(group)
                        {
                            title = group->stringForKey("title");
                        }
                        
                        ui::Button *categoryButton = dynamic_cast<ui::Button*>(button->clone());
                        
                        categoryButton->setPosition(Vec2(fposX,fposY));
                        categoryButton->setContentSize(Size(fWidth,fheight));
                        categoryButton->setTag(i);
                        categoryButton->addTouchEventListener(CC_CALLBACK_2(PixThemeChangeLayer::buttonCategoryPressed, this));
                        
                        Vec2 pos = Vec2(categoryButton->getContentSize().width * 0.5f,
                                        categoryButton->getContentSize().height * 0.5f);
                        
                        ui::Text *label = dynamic_cast<ui::Text*>(categoryButton->getChildByName("label"));
                        
                        if(label)
                        {
                            label->setString(title);
                            label->setPosition(pos);
                        }
                        
                        ui::Text *label_on = dynamic_cast<ui::Text*>(categoryButton->getChildByName("label_on"));
                        
                        if(label_on)
                        {
                            label_on->setString(title);
                            label_on->setPosition(pos);
                        }
                        
                        Node_ThemeCategoryLayer->addChild(categoryButton);
                        
                        categoryButtonVec.push_back(categoryButton);
                        
                        fposX += fWidth;
                    }
                    
                    button->removeFromParentAndCleanup(true);
                    
                    setCategoryButton(0);
                }
            }
        }
        
        ThemeBottomLayer = dynamic_cast<Node*>(rootNode->getChildByName("Node_ThemeBottomLayer_2"));
        
        if(ThemeBottomLayer)
        {
            ThemeBottomLayer->setLocalZOrder(2);
            btn_addphoto = dynamic_cast<ui::Button*>(ThemeBottomLayer->getChildByName("Button_ok"));
            
            if(btn_addphoto)
            {
                btn_addphoto->addTouchEventListener(CC_CALLBACK_2(PixThemeChangeLayer::btnAddphotoPressed, this));
                
                Vec2 pos = btn_addphoto->getPosition();
                Size size = btn_addphoto->getContentSize();
                btn_addphoto->setPosition(Vec2(pos.x, pos.y - size.height));
                setAddphotoPos(pos);
                
                ui::Text *text = dynamic_cast<ui::Text*>(btn_addphoto->getChildByName("Text"));
                
                if(text)
                {
                    text->setString(CODE_NAME(text->getString()));
                }
            }
        }
        
        ui::PageView *pageView = dynamic_cast<ui::PageView*>(rootNode->getChildByName("pageView"));
        
        if(pageView)
        {
            themePageView = PixThemePageView::create();
            themePageView->copyClonedPageView(pageView);
            themePageView->setLocalZOrder(0);
            createThemePageView();
            themePageView->setCurrentPageIndex(0);
        }
        
        if(SMARTALBUM_MANAGER->getGroupCount() > 1)
        {
            PageDot = dynamic_cast<ui::Layout*>(rootNode->getChildByName("PageDot"));
            
            if(PageDot)
            {
                PageDot->setLocalZOrder(100);
                createPageDot();
            }
        }
        
        Size size = rootNode->getContentSize();
        setAnchorPoint(Vec2::ZERO);
        setPosition(Vec2(0,-size.height));
        
        addChild(rootNode);
        
    }
}

void PixThemeChangeLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
  
    showLayerAction();
}

void PixThemeChangeLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(PageDot)
    {
        if(themePageView)
        {
            setPageDot(themePageView->getCurrentPageIndex());
        }
    }
    
    setThemeCategoryLine();
    setCategoryButton((int)themePageView->getCurrentPageIndex());
}

void PixThemeChangeLayer::createThemePageView()
{
    int nCount = SMARTALBUM_MANAGER->getGroupCount();
    
    for(int i = 0; i < nCount; ++i)
    {
        PixDictionary *group = SMARTALBUM_MANAGER->getGroupDictionary(i);
        
        if(group)
        {
            PixThemeGroupCell *cell = PixThemeGroupCell::create(group, ThemeTopLayer, ThemeBottomLayer, true);
            cell->setDelegate(this);
            cell->setThemePreviewLayerDelegate(this);
            cell->createCustomItem();
            themePageView->addPage(cell);
        }
    }
}

void PixThemeChangeLayer::createPageDot()
{
    int nCount = SMARTALBUM_MANAGER->getGroupCount();
    
    Size size = Size::ZERO;
    
    for(int i = 0; i < nCount; ++i)
    {
        Node *dot = CSLoader::createNode("03_Theme/ThemePageDot.csb");
        
        if(dot)
        {
            dynamic_cast<Sprite*>(dot->getChildByName("page_next_on"))->setVisible(false);
            dot->setPosition(Vec2(size.width,dot->getContentSize().height));
            size = size + dot->getContentSize();
            PageDot->setContentSize(size);
            PageDot->addChild(dot);
            pageDotVec.push_back(dot);
        }
    }
}

void PixThemeChangeLayer::setPageDot(ssize_t index)
{
    if(PageDot)
    {
        for(ssize_t i = 0; i < pageDotVec.size(); ++i)
        {
            Node *dot = pageDotVec.at(i);
            
            if(dot)
            {
                dynamic_cast<Sprite*>(dot->getChildByName("page_next_on"))->setVisible((i == index) ? true : false);
            }
        }
    }
}

bool PixThemeChangeLayer::checkCellSelected()
{
    if(themePageView)
    {
        bool isSelected = false;
        
        Vector<ui::Widget*>& items = themePageView->getItems();
        ssize_t nCount = items.size();
        
        for(ssize_t i = 0; i < nCount; ++i )
        {
            PixThemeGroupCell *cell = dynamic_cast<PixThemeGroupCell*>(items.at(i));
            
            if(cell &&  cell->checkCellSelected())
            {
                return true;
            }
        }
        
        return isSelected;
    }
    
    return false;
}

void PixThemeChangeLayer::showAddphoto()
{   
    btn_addphoto->stopAllActions();
    Vec2 movePos = getAddphotoPos();
    
    MoveTo *moveTo = MoveTo::create(0.2f,movePos);
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixThemeChangeLayer::addphotoActionFinish, this));
    addphotoAction = Sequence::create(easeSineOut, callFuncN, NULL);
    
    btn_addphoto->runAction(addphotoAction);
}

void PixThemeChangeLayer::hideAddphoto()
{
    btn_addphoto->stopAllActions();
    
    Vec2 pos = getAddphotoPos();
    Size size = btn_addphoto->getContentSize();
    Vec2 movePos = Vec2(pos.x, pos.y - size.height);
    
    MoveTo *moveTo = MoveTo::create(0.2f,movePos);
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixThemeChangeLayer::addphotoActionFinish, this));
    addphotoAction = Sequence::create(easeSineOut, callFuncN, NULL);
    
    btn_addphoto->runAction(addphotoAction);
}

void PixThemeChangeLayer::addphotoActionFinish()
{
    addphotoAction = nullptr;
}

void PixThemeChangeLayer::setCategoryButton(int nIndex)
{
    for (auto iter = categoryButtonVec.begin(); iter != categoryButtonVec.end(); ++iter)
    {
        if((*iter)->getTag() == nIndex)
        {
            ui::Text *label = dynamic_cast<ui::Text*>((*iter)->getChildByName("label"));
            
            if(label)
            {
                label->setVisible(false);
            }
            
            ui::Text *label_on = dynamic_cast<ui::Text*>((*iter)->getChildByName("label_on"));
            
            if(label_on)
            {
                label_on->setVisible(true);
            }
            
            (*iter)->setEnabled(false);
            
        }
        else
        {
            ui::Text *label = dynamic_cast<ui::Text*>((*iter)->getChildByName("label"));
            
            if(label)
            {
                label->setVisible(true);
            }
            
            ui::Text *label_on = dynamic_cast<ui::Text*>((*iter)->getChildByName("label_on"));
            
            if(label_on)
            {
                label_on->setVisible(false);
            }
            
            (*iter)->setEnabled(true);
        }
    }
}

void PixThemeChangeLayer::setThemeCategoryLine()
{
    if(themeCategoryLine)
    {
        if(themePageView)
        {
            Size containerSize = themePageView->getInnerContainerSize();
            Size pageViewSize = themePageView->getContentSize();
            Vec2 offset = themePageView->getInnerContainerPosition();
            
            float fPos = pageViewSize.width - containerSize.width;
            float fPercent = offset.x / fPos;
            
            if(Node_ThemeCategoryLayer)
            {
                Size size = Node_ThemeCategoryLayer->getContentSize();
                float offset = (size.width - themeCategoryLine->getContentSize().width) * fPercent;
                
                themeCategoryLine->setPositionX(offset);
            }
        }
    }
}

void PixThemeChangeLayer::themeChange()
{
    if(mThemeChangeDelegate)
    {
        mThemeChangeDelegate->themeChangePressed();
    }
    
    hideLayerAction();
}

#pragma mark - Action

void PixThemeChangeLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixThemeChangeLayer::showLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixThemeChangeLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixThemeChangeLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixThemeChangeLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixThemeChangeLayer::hideLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(getParent())
    {
        if(mDelegate)
        {
            mDelegate->hideLayerActionFinish(this);
        }
        
        this->removeFromParentAndCleanup(true);
    }
}


#pragma mark - Button

void PixThemeChangeLayer::btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        hideLayerAction();
        
        //        std::string strUserID = PIXAPP->userInfo->getUserID();
        //
        //        if(strUserID.compare("0") == 0)
        //        {
        //            PIXAPP->goLevel(0, PT_LOGIN, TR_FADE);
        //        }
        //        else
        //        {
        //            PIXAPP->goLevel(0, PT_MYALBUM, TR_FADE);
        //        }
    }
}

void PixThemeChangeLayer::btnAddphotoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        if(addphotoAction != nullptr)
            return;
        
        if(checkCellSelected())
        {
            std::string msg = CODE_NAME("P57");
            PixAlert::show(msg.c_str(), nullptr, ALERT_TYPE_ALL, 0, this, callfunc_selector(PixThemeChangeLayer::themeChange));
        }
        else
        {
            std::string msg = CODE_NAME("P37");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }
    }
}

void PixThemeChangeLayer::buttonCategoryPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED && getNumberOfRunningActions() == 0)
    {
        ui::Button *button = dynamic_cast<ui::Button*>(pSender);
        
        if(button)
        {
            if(themePageView)
            {
                themePageView->scrollToPage(button->getTag());
            }
            
            setCategoryButton(button->getTag());
        }
    }
}

#pragma mark - Delegate

void PixThemeChangeLayer::themeCellSelectedInitialize()
{
    if(themePageView)
    {
        ssize_t nIndex = themePageView->getCurrentPageIndex();
        
        Vector<ui::Widget*>& items = themePageView->getItems();
        ssize_t nCount = items.size();
        
        for(ssize_t i = 0; i < nCount; ++i )
        {
            if(i != nIndex)
            {
                PixThemeGroupCell *cell = dynamic_cast<PixThemeGroupCell*>(items.at(i));
                
                if(cell)
                {
                    cell->setThemeCellSelected(false);
                }
            }
        }
    }
}

void PixThemeChangeLayer::themePressed(bool isCellSelected)
{
    if(isCellSelected)
    {
        showAddphoto();
    }
    else
    {
        hideAddphoto();
    }
}

void PixThemeChangeLayer::themePreviewAddPhotoPressed()
{
    if(addphotoAction != nullptr)
        return;
    
    themeChange();
    
}

#pragma mark - Android BackPressed

void PixThemeChangeLayer::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(mbln_IsAction == false)
    {
        if(getAndroidBackPressedAction() == false)
        {
            setAndroidBackPressedAction(true);
            hideLayerAction();
        }
    }
}

