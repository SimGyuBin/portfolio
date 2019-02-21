//
//  PixSwitchButton.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 27..
//
//

#include "PixSwitchButton.h"
#include "../Util/PixUtils.h"

PixSwitchButton* PixSwitchButton::create(const char *leftImageName, const char *rightImageName,
                                         const char *maskImageName, const char *thumbImageName,
                                         float space, bool isActive)
{
    PixSwitchButton *button = new PixSwitchButton();
    button->init(leftImageName, rightImageName, maskImageName, thumbImageName, space, isActive);
    button->autorelease();
    
    return button;
}

PixSwitchButton::PixSwitchButton()
: mListener(nullptr)
, m_isActive(false)
, mask(nullptr)
, leftImage(nullptr)
, rightImage(nullptr)
, thumbImage(nullptr)
, maskSize(Size::ZERO)
, thumbHalfWidth(0.0f)
, mf_MoveX(0.0f)
, mf_Sspace(0.0f)
, isTab(false)
{
    arr_Delegate.clear();
}

PixSwitchButton::~PixSwitchButton()
{
    arr_Delegate.clear();
    
    unschedule(schedule_selector(PixSwitchButton::autoMove));
    
    if(mListener)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(mListener);
    }
}

#pragma mark - Public Method

void PixSwitchButton::setDelegate(PixSwitchDelegate *delegate)
{
    // 이미 등록되어있는 delegate라면 추가하지 않는다
    for (int i = 0; i < arr_Delegate.size(); i++)
    {
        if (arr_Delegate[i] == delegate)
        {
            return;
        }
    }
    
    arr_Delegate.push_back(delegate);
}

void PixSwitchButton::removeDelegate(PixSwitchDelegate *delegate)
{
    // 등록되지 않은 delegate라면 제거하지 않는다
    std::vector<PixSwitchDelegate*>::iterator it;
    
    for (it = arr_Delegate.begin(); it != arr_Delegate.end(); it++)
    {
        PixSwitchDelegate *inDelegate = (PixSwitchDelegate *)(*it);
        if (inDelegate == delegate)
        {
            arr_Delegate.erase(it);
            return;
        }
    }
}

void PixSwitchButton::setValue(bool value, bool isAnimate)
{
    m_isActive = value;
    
    goFixedPosition(isAnimate);
}


bool PixSwitchButton::getValue()
{
    return m_isActive;
}

#pragma mark - Private

void PixSwitchButton::init(const char *leftImageName, const char *rightImageName,
                           const char *maskImageName, const char *thumbImageName,
                           float space, bool isActive)
{
    ClippingNode::init();
    
    m_isActive = isActive;
    mf_Sspace = space;
    mf_MoveX = 0;
    
    mask = Sprite::create(maskImageName);
    mask->setAnchorPoint(Vec2::ZERO);
    mask->getTexture()->setAntiAliasTexParameters();
    setStencil(mask);
    addChild(mask);
    setAlphaThreshold(0.05f);
    maskSize = mask->getContentSize();
    
    thumbImage = Sprite::create(thumbImageName);
    thumbHalfWidth = thumbImage->getContentSize().width * 0.5f + mf_Sspace;
    thumbImage->setAnchorPoint( Vec2(0.5f, 0.5f) );
    thumbImage->setPosition( Vec2(m_isActive ? maskSize.width - thumbHalfWidth : thumbHalfWidth,
                                  maskSize.height * 0.5f) );
    addChild(thumbImage, 1);
    
    leftImage = Sprite::create(leftImageName);
    leftImage->setAnchorPoint( Vec2(1.0f, 0.5f) );
    leftImage->setPosition( Vec2(getImagePositionX(), maskSize.height * 0.5f) );
    addChild(leftImage);
    
    rightImage = Sprite::create(rightImageName);
    rightImage->setAnchorPoint( Vec2(0.0f, 0.5f) );
    rightImage->setPosition( Vec2(getImagePositionX(), maskSize.height * 0.5f) );
    addChild(rightImage);
    
    setContentSize(maskSize);
    setAnchorPoint(Vec2::ZERO);
}

// 바닥이미지 위치변화
void PixSwitchButton::moveImage(Vec2 position)
{
    thumbImage->setPosition( position );
    
    if (thumbImage->getPositionX() < thumbHalfWidth)
    {
        thumbImage->setPosition( Vec2(thumbHalfWidth, maskSize.height * 0.5f) );
    }
    else if (thumbImage->getPositionX() > maskSize.width - thumbHalfWidth)
    {
        thumbImage->setPosition( Vec2(maskSize.width - thumbHalfWidth, maskSize.height * 0.5f) );
    }
    
    leftImage->setPosition( Vec2(getImagePositionX(), maskSize.height * 0.5f) );
    rightImage->setPosition( Vec2(getImagePositionX(), maskSize.height * 0.5f) );
}

// 양쪽 끝으로 자동으로 이동한다
void PixSwitchButton::goFixedPosition(bool isAnimate)
{
    if (isAnimate && !Director::getInstance()->isPaused())
    {
        schedule(schedule_selector(PixSwitchButton::autoMove));
    }
    else
    {
        unschedule(schedule_selector(PixSwitchButton::autoMove));
        
        float to_X;
        
        if (m_isActive)
        {
            to_X = maskSize.width - thumbHalfWidth;
        }
        else
        {
            to_X = thumbHalfWidth;
        }
        
        thumbImage->setPosition( Vec2(to_X, maskSize.height * 0.5f) );
        leftImage->setPosition( Vec2(getImagePositionX(), maskSize.height * 0.5f) );
        rightImage->setPosition( Vec2(getImagePositionX(), maskSize.height * 0.5f) );
    }
}

// 자동으로 양끝으로 이동하는 애니메이션
void PixSwitchButton::autoMove(float dt)
{
    float to_X;
   
    if (m_isActive)
    {
        to_X = maskSize.width - thumbHalfWidth;
    }
    else
    {
        to_X = thumbHalfWidth;
    }
    
    
    mf_MoveX = to_X - thumbImage->getPositionX();
    
    if (fabs(mf_MoveX) < 1)
    {
        unschedule(schedule_selector(PixSwitchButton::autoMove));
        
        thumbImage->setPosition( Vec2(to_X, maskSize.height * 0.5f) );
        
        leftImage->setPosition( Vec2(getImagePositionX(), maskSize.height * 0.5f) );
        rightImage->setPosition( Vec2(getImagePositionX(), maskSize.height * 0.5f) );
    }
    else
    {
        thumbImage->setPosition( Vec2(thumbImage->getPositionX() + mf_MoveX * PIXSWITCH_ANI_SPEED, maskSize.height * 0.5f) );
        
        leftImage->setPosition( Vec2(getImagePositionX(), maskSize.height * 0.5f) );
        rightImage->setPosition( Vec2(getImagePositionX(), maskSize.height * 0.5f) );
    }
}

float PixSwitchButton::getImagePositionX()
{
    float fPosX = 0.0f;
    
    float fHalfSize = maskSize.width * 0.5f;
    
    if((thumbImage->getPositionX() > fHalfSize))
    {
        float fPercent = (thumbImage->getPositionX() + thumbHalfWidth - fHalfSize) / fHalfSize;
        fPosX = thumbImage->getPositionX() + (thumbHalfWidth * fPercent);
    }
    else
    {
        float fPercent = (fHalfSize - thumbImage->getPositionX() + thumbHalfWidth) / fHalfSize;
        fPosX = thumbImage->getPositionX() - (thumbHalfWidth * fPercent);
    }
    
    return fPosX;
}

#pragma mark - Touch

void PixSwitchButton::onEnter()
{
    ClippingNode::onEnter();
    
    mListener = EventListenerTouchOneByOne::create();
    mListener->setSwallowTouches(true);
    
    mListener->onTouchBegan = CC_CALLBACK_2(PixSwitchButton::onTouchBegan, this);
    mListener->onTouchMoved = CC_CALLBACK_2(PixSwitchButton::onTouchMoved, this);
    mListener->onTouchEnded = CC_CALLBACK_2(PixSwitchButton::onTouchEnded, this);
    mListener->onTouchCancelled = CC_CALLBACK_2(PixSwitchButton::onTouchCancelled, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mListener, this);
}

bool PixSwitchButton::onTouchBegan(Touch* touch, Event* event)
{
    if(NodeContainsPoint(this, touch->getLocation()))
    {
        isTab = true;
        goFixedPosition(false);
        return true;
    }
    
    return false;
}

void PixSwitchButton::onTouchMoved(Touch* touch, Event* event)
{
    if(NodeContainsPoint(this, touch->getLocation()))
    {
        isTab = false;
        
        Vec2 point = this->convertToNodeSpace(touch->getLocation());
        
        if (point.x >= 0 && point.x <= maskSize.width)
        {
            Vec2 bef = touch->getPreviousLocation();
            bef = this->convertToNodeSpace(bef);
            
            moveImage( Vec2(thumbImage->getPositionX() + (point.x - bef.x), maskSize.height * 0.5f) );
        }
    }
}

void PixSwitchButton::onTouchEnded(Touch* touch, Event* event)
{
    if (isTab)
    {
        m_isActive = !m_isActive;
    }
    
    else
    {
        m_isActive = (thumbImage->getPositionX() > maskSize.width * 0.5f);
    }
    
    goFixedPosition(true);

    // send CallBack
    for (int i = 0; i < arr_Delegate.size(); i++)
    {
        arr_Delegate[i]->changeSwitch(this, m_isActive);
    }
}

void PixSwitchButton::onTouchCancelled(Touch* touch, Event* event)
{
    
}
