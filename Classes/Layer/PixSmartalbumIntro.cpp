//
//  PixSmartalbumIntro.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 6..
//
//

#include "PixSmartalbumIntro.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Util/PixUserDefault.h"
#include "../Manager/PixMessageManager.h"
#include "../Common.h"

PixSmartalbumIntro* PixSmartalbumIntro::create()
{
    PixSmartalbumIntro *smartalbumIntro = new (std::nothrow) PixSmartalbumIntro();
    if (smartalbumIntro && smartalbumIntro->init())
    {
        smartalbumIntro->autorelease();
        return smartalbumIntro;
    }
    else
    {
        CC_SAFE_DELETE(smartalbumIntro);
        return nullptr;
    }
}

PixSmartalbumIntro::PixSmartalbumIntro()
: rootNode(nullptr)
, pageView(nullptr)
, layout(nullptr)
, bottom(nullptr)
, close(nullptr)
, checkBox(nullptr)
, Image_icon_next(nullptr)
, curPage(0)
, mbln_isLastPage(false)
{
    hideLayerFunction = std::bind(&PixSmartalbumIntro::hideLayerAction,this);
}

PixSmartalbumIntro::~PixSmartalbumIntro()
{
    
}

void PixSmartalbumIntro::initLayer()
{
    rootNode = CSLoader::createNode("05_AlbumView/smartalbumIntroLayer.csb");
    
    if(rootNode)
    {
        Node *Node_smartalbumIntroLayer = dynamic_cast<Node*>(rootNode->getChildByName("Node_smartalbumIntroLayer"));
        
        if(Node_smartalbumIntroLayer)
        {
            pageView = dynamic_cast<ui::PageView*>(Node_smartalbumIntroLayer->getChildByName("PageView"));
            
            if(pageView)
            {
                
            }
            
            layout = dynamic_cast<ui::Layout*>(Node_smartalbumIntroLayer->getChildByName("next"));
            
            if(layout)
            {
                
            }
            
            bottom = dynamic_cast<ui::Layout*>(Node_smartalbumIntroLayer->getChildByName("bottom"));
            
            if(bottom)
            {
                close = dynamic_cast<ui::Button*>(bottom->getChildByName("Button_close"));
                
                if(close)
                {
                    ui::Text *Text = dynamic_cast<ui::Text*>(close->getChildByName("Text"));
                    
                    if(Text)
                    {
                        Text->setString(CODE_NAME(Text->getString()));
                    }
                    
                    close->addTouchEventListener(CC_CALLBACK_2(PixSmartalbumIntro::buttonClosePressed, this));
//                    close->setEnabled(false);
                }
                
                checkBox = dynamic_cast<ui::CheckBox*>(bottom->getChildByName("checkBox"));
                
                if(checkBox)
                {
                    checkBox->addEventListener(CC_CALLBACK_2(PixSmartalbumIntro::checkBoxPressed, this));
                    
                    checkBox->setVisible(false);
                }
                
                ui::Text *label = dynamic_cast<ui::Text*>(bottom->getChildByName("Text"));
                
                if(label)
                {
                    label->setString(CODE_NAME(label->getString()));
                }
            }
            
            Image_icon_next = dynamic_cast<ui::ImageView*>(Node_smartalbumIntroLayer->getChildByName("Image_icon_next"));
            
            if(Image_icon_next)
            {
                Image_icon_next->setOpacity(0);
                Sequence *seq = Sequence::create(FadeIn::create(0.3f), DelayTime::create(0.3f), FadeOut::create(0.3f), NULL);
                Image_icon_next->runAction(RepeatForever::create(seq));
            }
        }
        
        setContentSize(rootNode->getContentSize());
        addChild(rootNode);    
    }
}

void PixSmartalbumIntro::onEnterTransitionDidFinish()
{
    PixModalLayer::onEnterTransitionDidFinish();
    
//    showCompletealbum();
}

void PixSmartalbumIntro::update(float delta)
{
    PixModalLayer::update(delta);
    
    if(pageView)
    {
        int nPageIndex = (int)pageView->getCurrentPageIndex();
        
        if(curPage != nPageIndex)
        {
            curPage = nPageIndex;
            setPageMarker(curPage);
        }
        
        if(mbln_isLastPage == false && curPage == 2)
        {
            mbln_isLastPage = true;
            
            if(checkBox)
            {
                checkBox->setVisible(true);
            }
            
            if(close)
            {
//                close->setEnabled(true);
            }
            
            if(Image_icon_next)
            {
                Image_icon_next->setVisible(false);
            }
        }
    }
}

void PixSmartalbumIntro::setPageMarker(int nPage)
{
    if(layout == nullptr)
        return;
    
    if(nPage == 0)
    {
        Sprite *sprite = dynamic_cast<Sprite*>(layout->getChildByName("intro_next_on_1"));
        
        if(sprite)
        {
            sprite->setVisible(true);
        }
        
        sprite = dynamic_cast<Sprite*>(layout->getChildByName("intro_next_on_2"));
        
        if(sprite)
        {
            sprite->setVisible(false);
        }
        
        sprite = dynamic_cast<Sprite*>(layout->getChildByName("intro_next_on_3"));
        
        if(sprite)
        {
            sprite->setVisible(false);
        }
    }
    else if(nPage == 1)
    {
        Sprite *sprite = dynamic_cast<Sprite*>(layout->getChildByName("intro_next_on_1"));
        
        if(sprite)
        {
            sprite->setVisible(false);
        }
        
        sprite = dynamic_cast<Sprite*>(layout->getChildByName("intro_next_on_2"));
        
        if(sprite)
        {
            sprite->setVisible(true);
        }
        
        sprite = dynamic_cast<Sprite*>(layout->getChildByName("intro_next_on_3"));
        
        if(sprite)
        {
            sprite->setVisible(false);
        }
    }
    else if(nPage >= 2)
    {
        Sprite *sprite = dynamic_cast<Sprite*>(layout->getChildByName("intro_next_on_1"));
        
        if(sprite)
        {
            sprite->setVisible(false);
        }
        
        sprite = dynamic_cast<Sprite*>(layout->getChildByName("intro_next_on_2"));
        
        if(sprite)
        {
            sprite->setVisible(false);
        }
        
        sprite = dynamic_cast<Sprite*>(layout->getChildByName("intro_next_on_3"));
        
        if(sprite)
        {
            sprite->setVisible(true);
        }
    }
}

void PixSmartalbumIntro::showCompletealbumFinish(Node* node)
{
    node->setVisible(false);
}

void PixSmartalbumIntro::hideLayerAction()
{
    if(getParent())
    {
        this->removeFromParentAndCleanup(true);
    }
}

#pragma mark - Button

void PixSmartalbumIntro::buttonClosePressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        PixUserDefault::SetSmartAlbumTutorial(false);
        
//        if(checkBox)
//        {
//            if(checkBox->isSelected())
//            {
//                PixUserDefault::SetSmartAlbumTutorial(false);
//            }
//        }
        
        hideLayerAction();
    }
}

void PixSmartalbumIntro::checkBoxPressed(cocos2d::Ref* pSender, ui::CheckBox::EventType type)
{
    
}

#pragma mark - Touch & Touches

void PixSmartalbumIntro::onTouchEnded(Touch* touch, Event* event)
{
    
}

#pragma mark - Android BackPressed

void PixSmartalbumIntro::androidBackPressed()
{
    PixModalLayer::androidBackPressed();
    
    if(getAndroidBackPressedAction() == false)
    {
        setAndroidBackPressedAction(true);
        
        hideLayerAction();
    }
}
