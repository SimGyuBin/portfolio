//
//  PixMath.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 8..
//
//

#ifndef PixMath_h
#define PixMath_h

#include "cocos2d.h"

USING_NS_CC;

typedef struct __GLVERTEX
{
    GLfloat x;
    GLfloat y;
} GLVERTEX;

typedef struct __MATRIX2X2
{
    float item[2][2];
} MATRIX2X2;

typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
} GLVERTEXxyz;

class PixMath
{
private:
    float cos_Predefined[360 * 10];
    GLVERTEX vertex_temp[4];

public:
    static MATRIX2X2 rotMatrixes[3600];
    
    static PixMath* sharedObject();
    PixMath();
    
    void getMatrixRotation  (MATRIX2X2 *matrix, float fAngle);
    void getMatrixMove      (MATRIX2X2 *matrix, float x, float y);
    void getMatrixZoom      (MATRIX2X2 *matrix, float zoom);
    
    void multiplyMatrix     (int nCount, GLVERTEX *p, MATRIX2X2 *d);
    void addMatrix          (int nCount, GLVERTEX *p, MATRIX2X2 *d);
    
    Vec2 Point_Rotation     (Vec2 pt, int angle);
    Vec2 NormalVectorRot    (Vec2 pt_Start, Vec2 pt_End);
    Vec2 NormalVector       (Vec2 pt_Start, Vec2 pt_End);
    float fast_cosf         (float angle);
    float fast_sinf         (float angle);
    
    bool getIsinRect        (Rect rect, Vec2 pt);
    Rect getIntersectRect   (const Rect &rect1, const Rect &rect2);
    float getLength         (Vec2 pt1, Vec2 pt2);
    float getLength         (Vec2 pt1);
    float getAngle          (Vec2 pt1, Vec2 pt2);
    
    float getDecimal        (float value);
    
    float getRandom         (float fStart, float fEnd, int nMaxDot);
    float getRandom         (unsigned char nStart, unsigned char nEnd);
    
    Rect getSumRect         (Rect r1, Rect r2);
    std::vector<Vec2>*      getCardinalSplineCurve(Vec2 *p, int nCount, float rez, int div);
    std::vector<Vec2>*      getStraightLine(Vec2 *p, int nCount, float fGap);
    float   tween_easeInOutSine(float start, float end, float percent);
    float   tween(float start, float end, float percent);
    Vec2 tween(Vec2 start, Vec2 end, float percent);
    
    int     getRoundUp        (float fValue);
    
};

static inline void Vertex_Move2       (GLVERTEX* vertex, float moveX, float moveY)
{
    vertex[0].x += moveX;
    vertex[0].y += moveY;
    vertex[1].x += moveX;
    vertex[1].y += moveY;
    vertex[2].x += moveX;
    vertex[2].y += moveY;
    vertex[3].x += moveX;
    vertex[3].y += moveY;
};

static inline void Vertex_Move3        (GLVERTEXxyz* vertex, float moveX, float moveY)
{
    vertex[0].x += moveX;
    vertex[0].y += moveY;
    vertex[1].x += moveX;
    vertex[1].y += moveY;
    vertex[2].x += moveX;
    vertex[2].y += moveY;
    vertex[3].x += moveX;
    vertex[3].y += moveY;
}

static inline void Vertex_Zoom        (GLVERTEX* vertex, float zoomwidth, float zoomheight)
{
    vertex[0].x *= zoomwidth;
    vertex[0].y *= zoomheight;
    vertex[1].x *= zoomwidth;
    vertex[1].y *= zoomheight;
    vertex[2].x *= zoomwidth;
    vertex[2].y *= zoomheight;
    vertex[3].x *= zoomwidth;
    vertex[3].y *= zoomheight;
}

static inline void Vertex_Rotation    (GLVERTEX* vertex, float angle  )
{
    GLVERTEX vertex_temp[4];
    MATRIX2X2 *rotation;
    int nAngle = (int)(angle * 10);
    nAngle = nAngle % 3600;
    if (nAngle < 0)
    {
        nAngle += 3600;
    }
    rotation = &(PixMath::rotMatrixes[nAngle]);
    
    vertex_temp[0].x = rotation->item[0][0] * vertex[0].x + rotation->item[1][0] * vertex[0].y;
    vertex_temp[0].y = rotation->item[0][1] * vertex[0].x + rotation->item[1][1] * vertex[0].y;
    
    vertex_temp[1].x = rotation->item[0][0] * vertex[1].x + rotation->item[1][0] * vertex[1].y;
    vertex_temp[1].y = rotation->item[0][1] * vertex[1].x + rotation->item[1][1] * vertex[1].y;
    
    vertex_temp[2].x = rotation->item[0][0] * vertex[2].x + rotation->item[1][0] * vertex[2].y;
    vertex_temp[2].y = rotation->item[0][1] * vertex[2].x + rotation->item[1][1] * vertex[2].y;
    
    vertex_temp[3].x = rotation->item[0][0] * vertex[3].x + rotation->item[1][0] * vertex[3].y;
    vertex_temp[3].y = rotation->item[0][1] * vertex[3].x + rotation->item[1][1] * vertex[3].y;
    memcpy (vertex, vertex_temp, sizeof(GLVERTEX) * 4);
}


#endif /* PixMath_h */
