//
//  PixSlideLeftMenu.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 5. 11..
//
//

#include "PixSlideLeftMenu.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "../Manager/PixMessageManager.h"
#include "../Scene/PixApp.h"
#include "../Util/PixUtils.h"
#include "../Util/PixNativeUtils.h"
#include "../UIControl/PixGalleryListView.h"
#include "../UIControl/PixToast.h"
#include "PixMenuMemberLayer.h"
#include "PixMenuSNSLayer.h"
#include "PixMenuNoticeLayer.h"
#include "PixMenuCustomerLayer.h"
#include "PixMenuCartLayer.h"
#include "PixMenuCouponLayer.h"
#include "PixMenuOrderLayer.h"
#include "PixMenuEventLayer.h"
#include "PixMenuProductLayer.h"
#include "PixMenuServiceLayer.h"
#include "../Common.h"

PixSlideLeftMenu *PixSlideLeftMenu::create()
{
    PixSlideLeftMenu *layer = new (std::nothrow) PixSlideLeftMenu();
    if (layer && layer->init())
    {
        layer->autorelease();
        layer->initLayer();
        return layer;
    }
    else
    {
        CC_SAFE_DELETE(layer);
        return nullptr;
    }
}

PixSlideLeftMenu::PixSlideLeftMenu()
: rootNode(nullptr)
, mListener(nullptr)
, Layout(nullptr)
, MenuListLayer(nullptr)
, blockout(nullptr)
, profliePhoto(nullptr)
, Text_name(nullptr)
, Text_email(nullptr)
, CartCount(nullptr)
, eventNew(nullptr)
, noticeNew(nullptr)
, mbln_isShow(false)
, mbln_showLayerActionFinish(false)
, mbln_hideLayerActionFinish(true)
, _touchesStart(Vec2::ZERO)
, mDelegate(nullptr)
, mAddMenuLayer(nullptr)
{
   
}

PixSlideLeftMenu::~PixSlideLeftMenu()
{
    if(mListener)
        Director::getInstance()->getEventDispatcher()->removeEventListener(mListener);
    
    CC_SAFE_RELEASE(mAddMenuLayer);
}

void PixSlideLeftMenu::initLayer()
{
    rootNode = CSLoader::createNode("00_Common/SlideLeftMenu.csb");
    
    if(rootNode)
    {
        MenuListLayer = dynamic_cast< ui::Layout*>(rootNode->getChildByName("MenuListLayer"));
        
        if(MenuListLayer)
        {
            Layout = dynamic_cast<Node*>(MenuListLayer->getChildByName("Layout"));
            
            if(Layout)
            {
                profliePhoto = dynamic_cast<Sprite*>(Layout->getChildByName("profliePhoto"));
                Text_name = dynamic_cast<ui::Text*>(Layout->getChildByName("Text_name"));
                Text_email = dynamic_cast<ui::Text*>(Layout->getChildByName("Text_email"));
                
                ui::Button *Button_Setting = dynamic_cast<ui::Button*>(Layout->getChildByName("Button_Setting"));
                
                if(Button_Setting)
                {
                    std::string normalImg = Button_Setting->getNormalFile().file;
                    std::string pressedImg = Button_Setting->getPressedFile().file;
                    std::string disabledImg = Button_Setting->getDisabledFile().file;
                    
                    Vec2 pos = Button_Setting->getPosition();
                    
                    auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                          CC_CALLBACK_1(PixSlideLeftMenu::ButtonSettingPressed, this));
                    
                    PixScrollMenu *menu = PixScrollMenu::create(menuItem, nullptr);
                    menu->setPosition(pos);
                    
                    menu->setContentSize(Button_Setting->getContentSize());
                    MenuListLayer->addChild(menu);
                    
                    Button_Setting->removeFromParentAndCleanup(true);
                    
                }
                
                ui::Button *Button_Cart = dynamic_cast<ui::Button*>(Layout->getChildByName("Button_Cart"));
                
                if(Button_Cart)
                {
                    std::string normalImg = Button_Cart->getNormalFile().file;
                    std::string pressedImg = Button_Cart->getPressedFile().file;
                    std::string disabledImg = Button_Cart->getDisabledFile().file;
                    
                    Vec2 pos = Button_Cart->getPosition();
                    
                    auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                          CC_CALLBACK_1(PixSlideLeftMenu::ButtonCartPressed, this));
                    
                    PixScrollMenu *menu = PixScrollMenu::create(menuItem, nullptr);
                    menu->setPosition(pos);
                    
                    menu->setContentSize(Button_Cart->getContentSize());
                    MenuListLayer->addChild(menu);
                    
                    ui::Text *Cart = dynamic_cast<ui::Text*>(Button_Cart->getChildByName("Cart"));
                    
                    if(Cart)
                    {
                        Cart->setString(CODE_NAME(Cart->getString()));
                        
                        ui::Text *clone =  dynamic_cast<ui::Text*>(Cart->clone());
                        
                        if(clone)
                        {
                            clone->setFontSize(Cart->getFontSize());
                            clone->setTextColor(Cart->getTextColor());
                            clone->setString(Cart->getString());
                            menuItem->addChild(clone);
                        }
                    }
                    
                    ui::Text *number = dynamic_cast<ui::Text*>(Button_Cart->getChildByName("number"));
                    
                    if(number)
                    {
                        CartCount = dynamic_cast<ui::Text*>(number->clone());
                        
                        if(CartCount)
                        {
                            CartCount->setFontSize(number->getFontSize());
                            CartCount->setTextColor(number->getTextColor());
                            CartCount->setString(number->getString());
                            menuItem->addChild(CartCount);
                        }
                    }
                    
                    Button_Cart->removeFromParentAndCleanup(true);
                }
                
                ui::Button *Button_Oder = dynamic_cast<ui::Button*>(Layout->getChildByName("Button_Oder"));
                
                if(Button_Oder)
                {
                    std::string normalImg = Button_Oder->getNormalFile().file;
                    std::string pressedImg = Button_Oder->getPressedFile().file;
                    std::string disabledImg = Button_Oder->getDisabledFile().file;
                    
                    Vec2 pos = Button_Oder->getPosition();
                    
                    auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                          CC_CALLBACK_1(PixSlideLeftMenu::ButtonOrderHistoryPressed, this));
                    
                    PixScrollMenu *menu = PixScrollMenu::create(menuItem, nullptr);
                    menu->setPosition(pos);
                    
                    menu->setContentSize(Button_Oder->getContentSize());
                    MenuListLayer->addChild(menu);
                    
                    ui::Text *Oder = dynamic_cast<ui::Text*>(Button_Oder->getChildByName("Oder"));
                    
                    if(Oder)
                    {
                        Oder->setString(CODE_NAME(Oder->getString()));
                        
                        ui::Text *clone =  dynamic_cast<ui::Text*>(Oder->clone());
                        
                        if(clone)
                        {
                            clone->setFontSize(Oder->getFontSize());
                            clone->setTextColor(Oder->getTextColor());
                            clone->setString(Oder->getString());
                            menuItem->addChild(clone);
                        }
                    }
                    
                    Button_Oder->removeFromParentAndCleanup(true);
                }
                
                ui::Button *Button_Coupon = dynamic_cast<ui::Button*>(Layout->getChildByName("Button_Coupon"));
                
                if(Button_Coupon)
                {
                    std::string normalImg = Button_Coupon->getNormalFile().file;
                    std::string pressedImg = Button_Coupon->getPressedFile().file;
                    std::string disabledImg = Button_Coupon->getDisabledFile().file;
                    
                    Vec2 pos = Button_Coupon->getPosition();
                    
                    auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                          CC_CALLBACK_1(PixSlideLeftMenu::ButtonCouponPressed, this));
                    
                    PixScrollMenu *menu = PixScrollMenu::create(menuItem, nullptr);
                    menu->setPosition(pos);
                    
                    menu->setContentSize(Button_Coupon->getContentSize());
                    MenuListLayer->addChild(menu);
                    
                    ui::Text *Coupon = dynamic_cast<ui::Text*>(Button_Coupon->getChildByName("Coupon"));
                    
                    if(Coupon)
                    {
                        Coupon->setString(CODE_NAME(Coupon->getString()));
                        
                        ui::Text *clone =  dynamic_cast<ui::Text*>(Coupon->clone());
                        
                        if(clone)
                        {
                            clone->setFontSize(Coupon->getFontSize());
                            clone->setTextColor(Coupon->getTextColor());
                            clone->setString(Coupon->getString());
                            menuItem->addChild(clone);
                        }
                    }
                    
                    Button_Coupon->removeFromParentAndCleanup(true);
                }
                
                ui::Button *Button_Service = dynamic_cast<ui::Button*>(Layout->getChildByName("Button_Service"));
                
                if(Button_Service)
                {
                    std::string normalImg = Button_Service->getNormalFile().file;
                    std::string pressedImg = Button_Service->getPressedFile().file;
                    std::string disabledImg = Button_Service->getDisabledFile().file;
                    
                    Vec2 pos = Button_Service->getPosition();
                    
                    auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                          CC_CALLBACK_1(PixSlideLeftMenu::ButtonServiceInforPressed, this));
                    
                    PixScrollMenu *menu = PixScrollMenu::create(menuItem, nullptr);
                    menu->setPosition(pos);
                    
                    menu->setContentSize(Button_Service->getContentSize());
                    MenuListLayer->addChild(menu);
                    
                    ui::Text *Service = dynamic_cast<ui::Text*>(Button_Service->getChildByName("Service"));
                    
                    if(Service)
                    {
                        Service->setString(CODE_NAME(Service->getString().c_str()));
                        
                        ui::Text *clone =  dynamic_cast<ui::Text*>(Service->clone());
                        
                        if(clone)
                        {
                            clone->setFontSize(Service->getFontSize());
                            clone->setTextColor(Service->getTextColor());
                            clone->setString(Service->getString());
                            menuItem->addChild(clone);
                        }
                    }
                    
                    Button_Service->removeFromParentAndCleanup(true);
                }
                
                ui::Button *Button_ProductInfor = dynamic_cast<ui::Button*>(Layout->getChildByName("Button_ProductInfor"));
                
                if(Button_ProductInfor)
                {
                    std::string normalImg = Button_ProductInfor->getNormalFile().file;
                    std::string pressedImg = Button_ProductInfor->getPressedFile().file;
                    std::string disabledImg = Button_ProductInfor->getDisabledFile().file;
                    
                    Vec2 pos = Button_ProductInfor->getPosition();
                    
                    auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                          CC_CALLBACK_1(PixSlideLeftMenu::ButtonProductInforPressed, this));
                    
                    PixScrollMenu *menu = PixScrollMenu::create(menuItem, nullptr);
                    menu->setPosition(pos);
                    
                    menu->setContentSize(Button_ProductInfor->getContentSize());
                    MenuListLayer->addChild(menu);
                    
                    ui::Text *ProductInfor = dynamic_cast<ui::Text*>(Button_ProductInfor->getChildByName("ProductInfor"));
                    
                    if(ProductInfor)
                    {
                        ProductInfor->setString(CODE_NAME(ProductInfor->getString().c_str()));
                        
                        ui::Text *clone =  dynamic_cast<ui::Text*>(ProductInfor->clone());
                        
                        if(clone)
                        {
                            clone->setFontSize(ProductInfor->getFontSize());
                            clone->setTextColor(ProductInfor->getTextColor());
                            clone->setString(ProductInfor->getString());
                            menuItem->addChild(clone);
                        }
                    }
                    
                    Button_ProductInfor->removeFromParentAndCleanup(true);
                }
                
                ui::Button *Button_Event = dynamic_cast<ui::Button*>(Layout->getChildByName("Button_Event"));
                
                if(Button_Event)
                {
                    std::string normalImg = Button_Event->getNormalFile().file;
                    std::string pressedImg = Button_Event->getPressedFile().file;
                    std::string disabledImg = Button_Event->getDisabledFile().file;
                    
                    Vec2 pos = Button_Event->getPosition();
                    
                    auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                          CC_CALLBACK_1(PixSlideLeftMenu::ButtonEventPressed, this));
                    
                    PixScrollMenu *menu = PixScrollMenu::create(menuItem, nullptr);
                    menu->setPosition(pos);
                    
                    menu->setContentSize(Button_Event->getContentSize());
                    MenuListLayer->addChild(menu);
                    
                    ui::Text *Event = dynamic_cast<ui::Text*>(Button_Event->getChildByName("Event"));
                    
                    if(Event)
                    {
                        Event->setString(CODE_NAME(Event->getString().c_str()));
                        
                        ui::Text *clone =  dynamic_cast<ui::Text*>(Event->clone());
                        
                        if(clone)
                        {
                            clone->setFontSize(Event->getFontSize());
                            clone->setTextColor(Event->getTextColor());
                            clone->setString(Event->getString());
                            menuItem->addChild(clone);
                        }
                    }
                    
                    ui::ImageView *imageView = dynamic_cast<ui::ImageView*>(Button_Event->getChildByName("Image_New"));
                    
                    if(imageView)
                    {
                        eventNew = dynamic_cast<ui::ImageView*>(imageView->clone());
                        
                        if(eventNew)
                        {
                            menuItem->addChild(eventNew);
                        }
                        
                        if(Event)
                        {
                            Size size = Event->getContentSize();
                            Vec2 pos = Event->getPosition();
                            
                            pos = Vec2((pos.x + size.width + 30.0f), pos.y);
                            
                            eventNew->setPosition(pos);
                        }
                    }
                    
                    Button_Event->removeFromParentAndCleanup(true);
                }
                
                ui::Button *Button_notice = dynamic_cast<ui::Button*>(Layout->getChildByName("Button_notice"));
                
                if(Button_notice)
                {
                    std::string normalImg = Button_notice->getNormalFile().file;
                    std::string pressedImg = Button_notice->getPressedFile().file;
                    std::string disabledImg = Button_notice->getDisabledFile().file;
                    
                    Vec2 pos = Button_notice->getPosition();
                    
                    auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                          CC_CALLBACK_1(PixSlideLeftMenu::ButtonNoticePressed, this));
                    
                    PixScrollMenu *menu = PixScrollMenu::create(menuItem, nullptr);
                    menu->setPosition(pos);
                    
                    menu->setContentSize(Button_notice->getContentSize());
                    MenuListLayer->addChild(menu);
                    
                    ui::Text *notice = dynamic_cast<ui::Text*>(Button_notice->getChildByName("notice"));
                    
                    if(notice)
                    {
                        notice->setString(CODE_NAME(notice->getString()));
                        
                        ui::Text *clone =  dynamic_cast<ui::Text*>(notice->clone());
                        
                        if(clone)
                        {
                            clone->setFontSize(notice->getFontSize());
                            clone->setTextColor(notice->getTextColor());
                            clone->setString(notice->getString());
                            menuItem->addChild(clone);
                        }
                    }
                    
                    ui::ImageView *imageView = dynamic_cast<ui::ImageView*>(Button_notice->getChildByName("Image_New"));
                    
                    if(imageView)
                    {
                        noticeNew = dynamic_cast<ui::ImageView*>(imageView->clone());
                        
                        if(noticeNew)
                        {
                            menuItem->addChild(noticeNew);
                        }
                        
                        if(notice)
                        {
                            Size size = notice->getContentSize();
                            Vec2 pos = notice->getPosition();
                            
                            pos = Vec2((pos.x + size.width + 30.0f), pos.y);
                            
                            noticeNew->setPosition(pos);
                        }
                    }

                    Button_notice->removeFromParentAndCleanup(true);
                }
                
                ui::Button *Button_Customer = dynamic_cast<ui::Button*>(Layout->getChildByName("Button_Customer"));
                
                if(Button_Customer)
                {
                    std::string normalImg = Button_Customer->getNormalFile().file;
                    std::string pressedImg = Button_Customer->getPressedFile().file;
                    std::string disabledImg = Button_Customer->getDisabledFile().file;
                    
                    Vec2 pos = Button_Customer->getPosition();
                    
                    auto menuItem = MenuItemImage::create(normalImg, pressedImg, disabledImg,
                                                          CC_CALLBACK_1(PixSlideLeftMenu::ButtonHelpPressed, this));
                    
                    PixScrollMenu *menu = PixScrollMenu::create(menuItem, nullptr);
                    menu->setPosition(pos);
                    
                    menu->setContentSize(Button_Customer->getContentSize());
                    MenuListLayer->addChild(menu);
                    
                    ui::Text *Customer = dynamic_cast<ui::Text*>(Button_Customer->getChildByName("Customer"));
                    
                    if(Customer)
                    {
                        Customer->setString(CODE_NAME(Customer->getString()));
                        
                        ui::Text *clone =  dynamic_cast<ui::Text*>(Customer->clone());
                        
                        if(clone)
                        {
                            clone->setFontSize(Customer->getFontSize());
                            clone->setTextColor(Customer->getTextColor());
                            clone->setString(Customer->getString());
                            menuItem->addChild(clone);
                        }
                    }
                    
                    Button_Customer->removeFromParentAndCleanup(true);
                }
            }
            
            blockout = dynamic_cast<Sprite*>(rootNode->getChildByName("blockout"));
            
            if(blockout)
            {
                blockout->setOpacity(0);
            }
            
            addChild(rootNode);
        }
    }
}

void PixSlideLeftMenu::onEnter()
{
    Layer::onEnter();
    scheduleUpdate();
}

void PixSlideLeftMenu::onExit()
{
    unscheduleUpdate();
    Layer::onExit();
}

void PixSlideLeftMenu::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
//    initEventListener();
}

void PixSlideLeftMenu::update(float dt)
{
    Layer::update(dt);
    
    float fPosX = MenuListLayer->getPositionX();
    float fWidth = MenuListLayer->getContentSize().width;
     
    if(-fWidth < fPosX || fPosX >=  0)
    {
        mbln_isShow = true;
        MenuListLayer->setVisible(true);
        blockout->setVisible(true);
    }
    else
    {
        mbln_isShow = false;
        MenuListLayer->setVisible(false);
        blockout->setVisible(false);
    }
    
    if(fPosX == 0)
    {
        if(getShowLayerActionFinish() == false)
        {
            showLayerActionFinish(nullptr);
        }
    }
    
    if(-fWidth == fPosX)
    {
        if(getHideLayerActionFinish() == false)
        {
            hideLayerActionFinish(nullptr);
        }
    }
    
    if(Text_name)
    {
        Text_name->setString(PIXAPP->userInfo->getName());
    }
    
    if(Text_email)
    {
        Text_email->setString(PIXAPP->userInfo->getEmail());
    }
    
    if(CartCount)
    {
        int nCount = PIXAPP->userInfo->getCartCount();
        
        if(nCount > 0)
        {
            std::string strCount = StringUtils::format("%d",nCount);
            CartCount->setString(strCount);
            CartCount->setVisible(true);
        }
        else
        {
            CartCount->setVisible(false);
        }
    }
    
    if(eventNew)
    {
        eventNew->setVisible(PIXAPP->userInfo->getEventNew());
    }
    
    if(noticeNew)
    {
        noticeNew->setVisible(PIXAPP->userInfo->getNoticeNew());
    }
}

void PixSlideLeftMenu::initEventListener()
{
    mListener = EventListenerTouchOneByOne::create();
    mListener->setSwallowTouches(true);
    
    mListener->onTouchBegan = CC_CALLBACK_2(PixSlideLeftMenu::onTouchBegan, this);
    mListener->onTouchMoved = CC_CALLBACK_2(PixSlideLeftMenu::onTouchMoved, this);
    mListener->onTouchEnded = CC_CALLBACK_2(PixSlideLeftMenu::onTouchEnded, this);
    mListener->onTouchCancelled = CC_CALLBACK_2(PixSlideLeftMenu::onTouchCancelled, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mListener, this);
}

void PixSlideLeftMenu::setAddMenuLayer(Node *pLayer)
{
    CC_SAFE_RELEASE_NULL(mAddMenuLayer);
    
    if(pLayer)
    {
        mAddMenuLayer = pLayer;
        mAddMenuLayer->retain();
    }
}

void PixSlideLeftMenu::addMenuLayer()
{
    if(mAddMenuLayer)
    {
        Director::getInstance()->getRunningScene()->addChild(mAddMenuLayer);
        CC_SAFE_RELEASE_NULL(mAddMenuLayer);
    }
}

void PixSlideLeftMenu::showSlideLeftMenu()
{
    showLayerAction();
}

void PixSlideLeftMenu::hideSlideLeftMenu()
{
    hideLayerAction();
}

#pragma mark - Action

void PixSlideLeftMenu::showLayerAction()
{
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    MenuListLayer->setVisible(true);
    blockout->setVisible(true);
    
    FadeIn *fadeIn = FadeIn::create(0.3f);
    EaseSineOut *easeSineOut = EaseSineOut::create(fadeIn);
    blockout->runAction(easeSineOut);
    
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(0,0));
    easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSlideLeftMenu::showLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    sequence->setTag(SLIDE_LEFT_ACTION_TAG);
    
    MenuListLayer->runAction(sequence);
}

void PixSlideLeftMenu::hideLayerAction()
{
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    FadeOut *fadeOut = FadeOut::create(0.3f);
    EaseSineOut *easeSineOut = EaseSineOut::create(fadeOut);
    blockout->runAction(easeSineOut);
    
    float fWidth = MenuListLayer->getContentSize().width;
    MoveTo *moveTo = MoveTo::create(0.3f,Vec2(-fWidth,0));
    easeSineOut = EaseSineOut::create(moveTo);
    CallFuncN *callFuncN = CallFuncN::create(CC_CALLBACK_1(PixSlideLeftMenu::hideLayerActionFinish, this));
    
    Sequence *sequence = Sequence::create(easeSineOut, callFuncN, NULL);
    sequence->setTag(SLIDE_LEFT_ACTION_TAG);
    
    MenuListLayer->runAction(sequence);
}

void PixSlideLeftMenu::showLayerActionFinish(Node* node)
{
    setShowLayerActionFinish(true);
    setHideLayerActionFinish(false);
}

void PixSlideLeftMenu::hideLayerActionFinish(Node* node)
{
    setShowLayerActionFinish(false);
    setHideLayerActionFinish(true);
    
    addMenuLayer();
 }

#pragma mark - Button

void PixSlideLeftMenu::ButtonSettingPressed(cocos2d::Ref *pSender)
{
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    PixMenuMemberLayer *menuMemberLayer =  PixMenuMemberLayer::create();
    menuMemberLayer->setDelegate(mDelegate);
    setAddMenuLayer(menuMemberLayer);
    hideLayerAction();
}

void PixSlideLeftMenu::ButtonCartPressed(cocos2d::Ref *pSender)
{
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    PixMenuCartLayer *menuCartLayer = PixMenuCartLayer::create();
    menuCartLayer->setDelegate(mDelegate);
    setAddMenuLayer(menuCartLayer);
    hideLayerAction();
}

void PixSlideLeftMenu::ButtonCouponPressed(cocos2d::Ref *pSender)
{
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    PixMenuCouponLayer *menuCouponLayer = PixMenuCouponLayer::create();
    menuCouponLayer->setDelegate(mDelegate);
    setAddMenuLayer(menuCouponLayer);
    hideLayerAction();
}

void PixSlideLeftMenu::ButtonOrderHistoryPressed(cocos2d::Ref *pSender)
{
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    PixMenuOrderLayer *menuOrderLayer = PixMenuOrderLayer::create();
    menuOrderLayer->setDelegate(mDelegate);
    setAddMenuLayer(menuOrderLayer);
    hideLayerAction();
}

void PixSlideLeftMenu::ButtonServiceInforPressed(cocos2d::Ref *pSender)
{
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    PixMenuServiceLayer *menuServiceLayer = PixMenuServiceLayer::create();
    menuServiceLayer->setDelegate(mDelegate);
    setAddMenuLayer(menuServiceLayer);
    hideLayerAction();
}

void PixSlideLeftMenu::ButtonProductInforPressed(cocos2d::Ref *pSender)
{
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    PixMenuProductLayer *menuProductLayer = PixMenuProductLayer::create();
    menuProductLayer->setDelegate(mDelegate);
    setAddMenuLayer(menuProductLayer);
    hideLayerAction();
}

void PixSlideLeftMenu::ButtonEventPressed(cocos2d::Ref *pSender)
{
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    PixMenuEventLayer *menuEventLayer = PixMenuEventLayer::create();
    menuEventLayer->setDelegate(mDelegate);
    setAddMenuLayer(menuEventLayer);
    hideLayerAction();
}

void PixSlideLeftMenu::ButtonNoticePressed(cocos2d::Ref *pSender)
{
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    PixMenuNoticeLayer *menuNoticeLayer = PixMenuNoticeLayer::create();
    menuNoticeLayer->setDelegate(mDelegate);
    setAddMenuLayer(menuNoticeLayer);
    hideLayerAction();
}

void PixSlideLeftMenu::ButtonHelpPressed(cocos2d::Ref *pSender)
{
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    PixMenuCustomerLayer *MenuCustomerLayer = PixMenuCustomerLayer::create();
    MenuCustomerLayer->setDelegate(mDelegate);
    setAddMenuLayer(MenuCustomerLayer);
    hideLayerAction();
}

#pragma mark - Touch & Touches

bool PixSlideLeftMenu::onTouchBegan(Touch* touch, Event* event)
{
    _touchesStart = touch->getLocation();
    
    return false;
}

void PixSlideLeftMenu::onTouchMoved(Touch* touch, Event* event)
{
    Layer::onTouchMoved(touch,event);
    
    if(MenuListLayer->getActionByTag(SLIDE_LEFT_ACTION_TAG))
        return;
    
    if(NodeContainsPoint(this, touch->getLocation()))
    {
        Vec2 directionVec = touch->getLocation() - _touchesStart;
        
        Vec2 origin(0, 1);
        float angle = directionVec.getAngle(origin);
        angle = CC_RADIANS_TO_DEGREES(angle);
        
        if ((90.0f * 3 / 4 <= angle && angle < 90.0f * 5 / 4) ||
            (-90.0f * 5 / 4 <= angle && angle < -90.0f * 3 / 4))
        {
            float fPosX = MenuListLayer->getPositionX();
            float fWidth = MenuListLayer->getContentSize().width;
            
            if(-fWidth <= fPosX || 0 >= fPosX)
            {
                Vec2 point = this->convertToNodeSpace(touch->getLocation());
                Vec2 bef = touch->getPreviousLocation();
                bef = this->convertToNodeSpace(bef);
                
                fPosX = fPosX + (point.x - bef.x);
                
                if(fPosX >= 0)
                {
                    fPosX = 0;
                }
                
                // 메뉴 포지션
                MenuListLayer->setPositionX(fPosX);
                
                // 백그라운드 처리
                fPosX = MenuListLayer->getPositionX();
                float fWidth = MenuListLayer->getContentSize().width;
                
                float fOpacity = 255 * ((fWidth - fabs(fPosX)) / fWidth);
                blockout->setOpacity(fOpacity);
            }
        }
    }
}

void PixSlideLeftMenu::onTouchEnded(Touch* touch, Event* event)
{
    _touchesEnd = touch->getLocation();
    
    float fPosX = MenuListLayer->getPositionX();
    float fWidth = MenuListLayer->getContentSize().width;
    
    if(fPosX == 0)
    {
        if(NodeContainsPoint(this, touch->getLocation()))
        {
            if(!NodeContainsPoint(MenuListLayer, touch->getLocation()))
            {
                hideLayerAction();
            }
        }
    }
    else
    {
        if(getShowLayerActionFinish())
        {
            float fDistance = fabs(fPosX);
            
            if(fDistance >= (fWidth / 5))
            {
                hideLayerAction();
            }
            else
            {
                showLayerAction();
            }
        }
        
        if(getHideLayerActionFinish())
        {
            float fDistance = (fWidth - fabs(fPosX));
            
            if(fDistance >= (fWidth / 5))
            {
                showLayerAction();
            }
            else
            {
                hideLayerAction();
            }
        }
    }
    
    _touchesStart = Vec2::ZERO;
    
    Layer::onTouchEnded(touch,event);
    
}

void PixSlideLeftMenu::onTouchCancelled(Touch* touch, Event* event)
{
    _touchesStart = Vec2::ZERO;
    
    Layer::onTouchCancelled(touch,event);
}
