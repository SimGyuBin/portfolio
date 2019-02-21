//
//  PixToast.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 6..
//
//

#include "PixToast.h"
#include "../Scene/PixApp.h"

Vec2 PixToast::toastPos()
{
    Size ws = Director::getInstance()->getWinSize();
    Vec2 pos = Vec2::ZERO;
    
    if(PIXAPP->frameType == FRAME_VERTICALSIZE)
    {
        pos = Vec2(ws.width * 0.5f, 215);
    }
    else if(PIXAPP->frameType == FRAME_HORIZONTAL)
    {
        pos = Vec2(ws.width * 0.5f, 180);
    }
    
    return pos;
}

//void PixToast::appendCubicBezier(int startPoint, std::vector<Vec2>& verts, const Vec2& from, const Vec2& control1, const Vec2& control2, const Vec2& to, uint32_t segments)
//{
//    float t = 0;
//    for(int i = 0; i < segments; i++)
//    {
//        float x = powf(1 - t, 3) * from.x + 3.0f * powf(1 - t, 2) * t * control1.x + 3.0f * (1 - t) * t * t * control2.x + t * t * t * to.x;
//        float y = powf(1 - t, 3) * from.y + 3.0f * powf(1 - t, 2) * t * control1.y + 3.0f * (1 - t) * t * t * control2.y + t * t * t * to.y;
//        verts[startPoint + i] = Vec2(x,y);
//        t += 1.0f / segments;
//    }
//}
//
//Node* PixToast::createRoundedRectMaskNode(Size size, float radius, float borderWidth, int cornerSegments)
//{
//    const float kappa = 0.552228474;
//    float oneMinusKappa = (1.0f-kappa);
//    
//    // define corner control points
//    std::vector<Vec2> verts(16);
//    
//    verts[0] = Vec2(0, radius);
//    verts[1] = Vec2(0, radius * oneMinusKappa);
//    verts[2] = Vec2(radius * oneMinusKappa, 0);
//    verts[3] = Vec2(radius, 0);
//    
//    verts[4] = Vec2(size.width - radius, 0);
//    verts[5] = Vec2(size.width - radius * oneMinusKappa, 0);
//    verts[6] = Vec2(size.width, radius * oneMinusKappa);
//    verts[7] = Vec2(size.width, radius);
//    
//    verts[8] = Vec2(size.width, size.height - radius);
//    verts[9] = Vec2(size.width, size.height - radius * oneMinusKappa);
//    verts[10] = Vec2(size.width - radius * oneMinusKappa, size.height);
//    verts[11] = Vec2(size.width - radius, size.height);
//    
//    verts[12] = Vec2(radius, size.height);
//    verts[13] = Vec2(radius * oneMinusKappa, size.height);
//    verts[14] = Vec2(0, size.height - radius * oneMinusKappa);
//    verts[15] = Vec2(0, size.height - radius);
//    
//    // result
//    std::vector<Vec2> polyVerts(4 * cornerSegments + 1);
//    
//    // add corner arc segments
//    appendCubicBezier(0 * cornerSegments, polyVerts, verts[0], verts[1], verts[2], verts[3], cornerSegments);
//    appendCubicBezier(1 * cornerSegments, polyVerts, verts[4], verts[5], verts[6], verts[7], cornerSegments);
//    appendCubicBezier(2 * cornerSegments, polyVerts, verts[8], verts[9], verts[10], verts[11], cornerSegments);
//    appendCubicBezier(3 * cornerSegments, polyVerts, verts[12], verts[13], verts[14], verts[15], cornerSegments);
//    // close path
//    polyVerts[4 * cornerSegments] = verts[0];
//    
//    // draw final poly into mask
//    auto shapeMask = DrawNode::create();
//    Color4F color(1, 1, 1, 0);
//    shapeMask->drawPolygon(&polyVerts[0], (int)polyVerts.size(), color, 1.0f, color);
//    
//    // create clip node with draw node as stencil (mask)
//    return ClippingNode::create(shapeMask);
//}

PixToast* PixToast::create(const char *message, const Vec2 &position, float duration,
                           const char *fontName, int fontSize, Color4B textColor)
{
    PixToast *toast = new PixToast();
    toast->init(message, position, duration, fontName, fontSize, textColor);
    toast->autorelease();
    
    return toast;
}

PixToast* PixToast::showToast(const char *message, const Vec2 &position, float duration,
                            const char *fontName, int fontSize, Color4B textColor)
{
    Scene *scene = PIXAPP->isScene;
    Node *beforeToast = dynamic_cast<Node*>(scene->getChildByTag(TOAST_TAG));
    if (beforeToast) beforeToast->removeFromParentAndCleanup(true);
    
    PixToast *toast = new PixToast();
    toast->init(message, position, duration, fontName, fontSize, textColor);
    scene->addChild(toast, 300, TOAST_TAG);
    toast->release();
    
    return toast;
}

PixToast::~PixToast()
{
    CCLOG("%s", __FUNCTION__);
}

void PixToast::removeToast()
{
    PixToast *beforeToast = dynamic_cast<PixToast*>(Director::getInstance()->getRunningScene()->getChildByTag(TOAST_TAG));
    if (beforeToast) beforeToast->removeFromParentAndCleanup(true);
}

#pragma mark - Private

void PixToast::init(const char *message, const Vec2 &position, float duration,
                   const char *fontName, int fontSize, Color4B textColor)
{
    m_duration = duration;
    
    label = ui::Text::create(message, fontName, fontSize);
    
    Sprite::init();
    setTextureRect(Rect(0, 0, label->getContentSize().width + 50, label->getContentSize().height + 50));
    setPosition(position);
    
    Texture2D* texture = Director::getInstance()->getTextureCache()->getTextureForKey(TOAST_IMAGE);
    
    if(texture == nullptr)
    {
        Image* image = new (std::nothrow) Image();
        bool isOK = image->initWithRawData(pix_toast_image, sizeof(pix_toast_image), 2, 2, 8);
        CC_UNUSED_PARAM(isOK);
        CCASSERT(isOK, "The 2x2 empty texture was created unsuccessfully.");
        
        texture = Director::getInstance()->getTextureCache()->addImage(image, TOAST_IMAGE);
        CC_SAFE_RELEASE(image);
    }
    
    setTexture(texture);
    
    setOpacity(0);
    
    label->setPosition(Vec2(getContentSize().width * 0.5f,
                            getContentSize().height * 0.5f));
    
    Size size = getContentSize();
  
    left = Sprite::create("Common/toast_back_left.png");
    float fScale = ((float) size.height) / ((float) left->getContentSize().height);
    left->setScale(fScale);
    left->setPosition(Vec2(-((left->getContentSize().width * fScale) / 2),
                           ((left->getContentSize().height * fScale)/ 2)));
    left->setOpacity(0);
    addChild(left);
    
    
    right = Sprite::create("Common/toast_back_right.png");
    fScale = ((float) size.height) / ((float) right->getContentSize().height);
    right->setScale(fScale);
    right->setPosition(Vec2(size.width + ((left->getContentSize().width * fScale) / 2),
                           ((left->getContentSize().height * fScale)/ 2)));
    right->setOpacity(0);
    addChild(right);
    
    label->setTextColor(textColor);
    label->setOpacity(0);
    addChild(label);
}


void PixToast::onEnter()
{
    Sprite::onEnter();
    
    if (Director::getInstance()->isPaused())
    {
        setOpacity(255 * 0.75f);
        label->setOpacity(255);
        left->setOpacity(255 * 0.75f);
        right->setOpacity(255 * 0.75f);
    }
    else
    {
        runAction(FadeTo::create(TOAST_ANI_SPEED, 255 * 0.75f));
        label->runAction(FadeTo::create(TOAST_ANI_SPEED, 255));
        left->runAction(FadeTo::create(TOAST_ANI_SPEED, 255 * 0.75f));
        right->runAction(FadeTo::create(TOAST_ANI_SPEED, 255 * 0.75f));
    }
    
    scheduleUpdate();
}

void PixToast::update(float dt)
{
    m_duration -= dt;
    
    if (m_duration <= 0)
    {
        unscheduleUpdate();
        
        label->runAction(FadeTo::create(TOAST_ANI_SPEED, 0));
        left->runAction(FadeTo::create(TOAST_ANI_SPEED, 0));
        right->runAction(FadeTo::create(TOAST_ANI_SPEED, 0));

        FadeTo *fade = FadeTo::create(TOAST_ANI_SPEED, 0);
        CallFunc *func = CallFunc::create(CC_CALLBACK_0(PixToast::remove, this));
        
        runAction(Sequence::create(fade, func, NULL));
    }
}

void PixToast::remove()
{
    removeToast();
}

