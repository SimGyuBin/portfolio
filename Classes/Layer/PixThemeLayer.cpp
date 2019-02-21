//
//  PixThemeLayer.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 10..
//
//

#include "PixThemeLayer.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUtils.h"
#include "../Widget/PixThemeCell.h"
#include "../Widget/PixThemeHeaderCell.h"
#include "../Manager/PixSmartAlbumManager.h"
#include "../Scene/PixApp.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"
#include "editor-support/cocostudio/ActionTimeline/CCActionTimeline.h"
#include "PixThemeGuideLayer.h"
#include "../Util/PixUserDefault.h"

PixThemeLayer* PixThemeLayer::create(bool showAction)
{
    PixThemeLayer *themeLayer = new (std::nothrow) PixThemeLayer(showAction);
    if (themeLayer && themeLayer->init())
    {
        themeLayer->autorelease();
        return themeLayer;
    }
    else
    {
        CC_SAFE_DELETE(themeLayer);
        return nullptr;
    }
}

PixThemeLayer::PixThemeLayer(bool showAction)
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
, m_AddphotoPos(Vec2::ZERO)
, addphotoAction(nullptr)
{
    mbln_ShowAction = showAction;
    pageDotVec.clear();
    categoryButtonVec.clear();
    
    hideLayerFunction = std::bind(&PixThemeLayer::hideLayerAction,this);
}

PixThemeLayer::~PixThemeLayer()
{
    pageDotVec.clear();
    categoryButtonVec.clear();
}

void PixThemeLayer::initLayer()
{
    rootNode = CSLoader::createNode("03_Theme/ThemeLayer.csb");
    
    if(rootNode)
    {
        ThemeTopLayer = dynamic_cast<Node*>(rootNode->getChildByName("ThemeTopLayer"));
        
        if(ThemeTopLayer)
        {
            ThemeTopLayer->setLocalZOrder(1);
            
            btn_back = dynamic_cast<ui::Button*>(ThemeTopLayer->getChildByName("close"));
            
            if(btn_back)
            {
                btn_back->addTouchEventListener(CC_CALLBACK_2(PixThemeLayer::btnBackPressed, this));
            }
            
            ui::Text *title = dynamic_cast<ui::Text*>(ThemeTopLayer->getChildByName("label"));
            
            if(title)
            {
                title->setString(CODE_NAME(title->getString()));
            }
            
            ui::Button* Button_Tip = dynamic_cast<ui::Button*>(ThemeTopLayer->getChildByName("Button_Tip"));
            
            if(Button_Tip)
            {
                Button_Tip->addTouchEventListener(CC_CALLBACK_2(PixThemeLayer::buttonTipPressed, this));
                
                // 버튼 이펙트 추가
                ClippingNode *clippingNode = ClippingNode::create();
                clippingNode->setPosition(Button_Tip->getPosition());
                Sprite *stencil = Sprite::create(Button_Tip->getNormalFile().file);
                clippingNode->setStencil(stencil);
                
                Node *effect = CSLoader::createNode("03_Theme/Button_Effect.csb");
               
                if(effect)
                {
                    ActionTimeline *timeLine = CSLoader::createTimeline("03_Theme/Button_Effect.csb");
                    
                    if(timeLine)
                    {
                        timeLine->gotoFrameAndPlay(0,true);
                        effect->runAction(timeLine);
                    }
                    
                    clippingNode->addChild(effect);
                }
                
                ThemeTopLayer->addChild(clippingNode);
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
                        categoryButton->addTouchEventListener(CC_CALLBACK_2(PixThemeLayer::buttonCategoryPressed, this));
                        
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
        
        ThemeBottomLayer = dynamic_cast<Node*>(rootNode->getChildByName("ThemeBottomLayer"));
        
        if(ThemeBottomLayer)
        {
            ThemeBottomLayer->setLocalZOrder(2);
            btn_addphoto = dynamic_cast<ui::Button*>(ThemeBottomLayer->getChildByName("btn_addphoto"));
            
            if(btn_addphoto)
            {
                btn_addphoto->addTouchEventListener(CC_CALLBACK_2(PixThemeLayer::btnAddphotoPressed, this));

                Vec2 pos = btn_addphoto->getPosition();
                Size size = btn_addphoto->getContentSize();
                btn_addphoto->setPosition(Vec2(pos.x, pos.y - size.height));
                setAddphotoPos(pos);
                
                ui::Text *text = dynamic_cast<ui::Text*>(btn_addphoto->getChildByName("addphoto"));
                
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
        
        if(getShowAction())
        {
            setPosition(Vec2(0,-size.height));
        }
       
        addChild(rootNode);
        
    }
}

void PixThemeLayer::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    
    if(getShowAction())
    {
        showLayerAction();
    }
    
    // 강제로 테마 셋팅 하는 테스트 코드
//    int nCount = SMARTALBUM_MANAGER->getGroupCount();
//    
//    for(int i = 0; i < nCount; ++i)
//    {
//        PixDictionary *group = SMARTALBUM_MANAGER->getGroupDictionary(i);
//        
//        if(group)
//        {
//            PixArray *children = group->arrayForKey("children");
//            
//            if(children)
//            {
//                PixDictionary *pDic = children->getDictionaryAtIndex(0);
//                
//                if(pDic)
//                {
//                    PIXAPP->userInfo->setSelectThemeInfo(pDic);
//                    break;
//                }
//            }
//        }
//    }
//    
//    this->schedule(schedule_selector(PixThemeLayer::goGallery));
    
}

void PixThemeLayer::update(float dt)
{
    PixModalLayer::update(dt);
    
    if(themePageView)
    {
        themePageView->setEnabled(!mbln_IsAction);
    }
    
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

void PixThemeLayer::createThemePageView()
{
    int nCount = SMARTALBUM_MANAGER->getGroupCount();
    
    for(int i = 0; i < nCount; ++i)
    {
        PixDictionary *group = SMARTALBUM_MANAGER->getGroupDictionary(i);
        
        if(group)
        {
            PixThemeGroupCell *cell = PixThemeGroupCell::create(group, ThemeTopLayer, ThemeBottomLayer);
            cell->setDelegate(this);
            cell->setThemePreviewLayerDelegate(this);
            cell->createCustomItem();
            themePageView->addPage(cell);
        }
    }
}

void PixThemeLayer::createPageDot()
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

void PixThemeLayer::setPageDot(ssize_t index)
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


void PixThemeLayer::goGallery(float dt)
{
    if(PIXAPP->isLocalImageLoading == true)
    {
        PIXAPP->hideLoading();
        this->unschedule(schedule_selector(PixThemeLayer::goGallery));
        PIXAPP->goLevel(0, PT_GALLERY, TR_FADE);
    }
    else
    {
        //PIXAPP->showLoading();
    }
}

bool PixThemeLayer::checkCellSelected()
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

void PixThemeLayer::showAddphoto()
{
    btn_addphoto->stopAllActions();
    Vec2 movePos = getAddphotoPos();
    
    MoveTo *moveTo = MoveTo::create(0.2f,movePos);
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixThemeLayer::addphotoActionFinish, this));
    addphotoAction = Sequence::create(easeSineOut, callFuncN, NULL);
    
    btn_addphoto->runAction(addphotoAction);
}

void PixThemeLayer::hideAddphoto()
{
    btn_addphoto->stopAllActions();
    
    Vec2 pos = getAddphotoPos();
    Size size = btn_addphoto->getContentSize();
    Vec2 movePos = Vec2(pos.x, pos.y - size.height);
    
    MoveTo *moveTo = MoveTo::create(0.2f,movePos);
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_0(PixThemeLayer::addphotoActionFinish, this));
    addphotoAction = Sequence::create(easeSineOut, callFuncN, NULL);
    
    btn_addphoto->runAction(addphotoAction);
}

void PixThemeLayer::addphotoActionFinish()
{
    addphotoAction = nullptr;
}

void PixThemeLayer::setCategoryButton(int nIndex)
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

void PixThemeLayer::setThemeCategoryLine()
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

#pragma mark - Action

void PixThemeLayer::showLayerAction()
{
    mbln_IsAction = true;
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixThemeLayer::showLayerActionFinish, this));
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->showLayerActionStart(this);
    }
}

void PixThemeLayer::hideLayerAction()
{
    mbln_IsAction = true;
    
    Size size = rootNode->getContentSize();
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,-size.height));
    EaseSineOut *easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixThemeLayer::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    
    runAction(sequence);
    
    if(mDelegate)
    {
        mDelegate->hideLayerActionStart(this);
    }
}

void PixThemeLayer::showLayerActionFinish(Node* node)
{
    mbln_IsAction = false;
    
    if(mDelegate)
    {
        mDelegate->showLayerActionFinish(this);
    }
}

void PixThemeLayer::hideLayerActionFinish(Node* node)
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

void PixThemeLayer::btnBackPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
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

void PixThemeLayer::btnAddphotoPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction)
            return;
        
        if(addphotoAction != nullptr)
            return;
        
        if(checkCellSelected())
        {
            this->schedule(schedule_selector(PixThemeLayer::goGallery));
        }
        else
        {
            std::string msg = CODE_NAME("P37");
            PixAlert::show("Error", msg.c_str(), nullptr, ALERT_TYPE_OK);
        }    
    }
}

void PixThemeLayer::buttonTipPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction)
            return;
        
        Director::getInstance()->getRunningScene()->addChild(PixThemeTipLayer::create());
    }
}

void PixThemeLayer::buttonCategoryPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        if(mbln_IsAction)
            return;
        
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

void PixThemeLayer::myGalleryPressed()
{
    
}

void PixThemeLayer::facePressed()
{
    
}

void PixThemeLayer::googlePressed()
{
    
}

void PixThemeLayer::instaPressed()
{
    
}

void PixThemeLayer::kakaoPressed()
{
    
}

void PixThemeLayer::themeCellSelectedInitialize()
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

void PixThemeLayer::themePressed(bool isCellSelected)
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

void PixThemeLayer::themePreviewAddPhotoPressed()
{
    if(mbln_IsAction)
        return;
    
    if(addphotoAction != nullptr)
        return;
    
    this->schedule(schedule_selector(PixThemeLayer::goGallery));
}

#pragma mark - Android BackPressed

void PixThemeLayer::androidBackPressed()
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
