//
//  PixToast.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 6..
//
//

#ifndef PixToast_h
#define PixToast_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

#define TOAST_TAG           592
#define TOAST_ANI_SPEED     0.5f    // 등장 & 사라짐 스피드

static unsigned char pix_toast_image[] = {
    // RGBA8888
    0x11, 0x11, 0x11, 0xFF,
    0x11, 0x11, 0x11, 0xFF,
    0x11, 0x11, 0x11, 0xFF,
    0x11, 0x11, 0x11, 0xFF
};

#define TOAST_IMAGE  "/pix_toast_image"
#define TOAST_FONT_NAME "Common/KoPubDotumMedium.ttf"

class PixToast : public Sprite {
    
public:
    
    
    static Vec2 toastPos();
    
//    static void appendCubicBezier(int startPoint, std::vector<Vec2>& verts, const Vec2& from, const Vec2& control1, const Vec2& control2, const Vec2& to, uint32_t segments);
//    static Node* createRoundedRectMaskNode(Size size, float radius, float borderWidth, int cornerSegments);
    
    
    static PixToast* create(const char *message, const Vec2 &position = PixToast::toastPos(), float duration = 3.0f,
                            const char *fontName = TOAST_FONT_NAME, int fontSize = 26, Color4B textColor = Color4B::WHITE);
    
    static PixToast* showToast(const char *message, const Vec2 &position = PixToast::toastPos(), float duration = 3.0f,
                               const char *fontName = TOAST_FONT_NAME, int fontSize = 26, Color4B textColor = Color4B::WHITE);
    virtual ~PixToast();
    
    static void removeToast();
    
private:
    void init(const char *message, const Vec2 &position, float duration,
              const char *fontName, int fontSize, Color4B textColor);
    
    virtual void onEnter();
    virtual void update(float dt);
    
    void remove();
    
private:
    float m_duration;
    ui::Text *label;
    Sprite *left;
    Sprite *right;
};


#endif /* PixToast_h */
