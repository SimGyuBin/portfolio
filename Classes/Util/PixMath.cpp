//
//  PixMath.cpp
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 4. 8..
//
//

#include "PixMath.h"

#define PixMath_PI					3.14159

PixMath *pixMath = nullptr;

MATRIX2X2 PixMath::rotMatrixes[3600];

PixMath* PixMath::sharedObject()
{
    if (pixMath == nullptr)
    {
        pixMath = new PixMath();
    }
    return pixMath;
}

PixMath::PixMath()
{
    memset (cos_Predefined, 0x00, sizeof(float) * 3600);
    for (int i = 0; i < 360 * 10; ++i)
    {
        cos_Predefined[i] = cosf((i / 10.0f) * 3.141592 / 180);
    }
    cos_Predefined[0] = 1.0;
    cos_Predefined[900] = 0.0;
    cos_Predefined[1800] = -1.0;
    cos_Predefined[2700] = 0.0;
    
    memset (vertex_temp, 0x00, sizeof(GLVERTEX) * 4);
    
    for (int i = 0; i < 3600; ++i)
    {
        getMatrixRotation(&rotMatrixes[i], i / 10.0f);
    }
}

void PixMath::multiplyMatrix(int nCount, GLVERTEX *p, MATRIX2X2 *d)
{
    float x, y;
    for (int i = 0; i < nCount; ++i)
    {
        x = p[i].x;
        y = p[i].y;
        
        p[i].x = x * d->item[0][0] + y * d->item[0][1];
        p[i].y = x * d->item[1][0] + y * d->item[1][1];
    }
}

void PixMath::addMatrix(int nCount, GLVERTEX *p, MATRIX2X2 *d)
{
    for (int i = 0; i < nCount; ++i)
    {
        p[i].x += d->item[0][0];
        p[i].y += d->item[1][1];
    }
}

void PixMath::getMatrixRotation(MATRIX2X2 *matrix, float fAngle)
{
    matrix->item[0][0] = this->fast_cosf(fAngle);
    matrix->item[1][0] = this->fast_sinf(fAngle);
    matrix->item[0][1] = (-1) * matrix->item[1][0];
    matrix->item[1][1] = matrix->item[0][0];
}

void PixMath::getMatrixMove(MATRIX2X2 *matrix, float x, float y)
{
    matrix->item[0][0] = x;
    matrix->item[0][1] = 0;
    matrix->item[1][0] = 0;
    matrix->item[1][1] = y;
}

void PixMath::getMatrixZoom(MATRIX2X2 *matrix, float zoom)
{
    matrix->item[0][0] = zoom;
    matrix->item[0][1] = 0;
    matrix->item[1][0] = 0;
    matrix->item[1][1] = zoom;
}

Vec2 PixMath::Point_Rotation(Vec2 pt, int angle)
{
    Vec2 p;
    float cosangle = fast_cosf(angle);
    float sinangle = fast_sinf(angle);
    
    p.x = (pt.x * cosangle - pt.y * sinangle);
    p.y = (pt.x * sinangle + pt.y * cosangle);
    
    return p;
}

Vec2 PixMath::NormalVectorRot    (Vec2 pt_Start, Vec2 pt_End)
{
    Vec2 pt_Gap = pt_End - pt_Start;
    float length = getLength(pt_Gap);
    pt_Gap.x /= length;
    pt_Gap.y /= length;
    Vec2 pt_Rotation = Point_Rotation(pt_Gap, 90);
    return pt_Rotation;
}

Vec2 PixMath::NormalVector    (Vec2 pt_Start, Vec2 pt_End)
{
    Vec2 pt_Gap = pt_End - pt_Start;
    float length = getLength(pt_Gap);
    pt_Gap.x /= length;
    pt_Gap.y /= length;
    return pt_Gap;
}

float PixMath::fast_cosf(float angle)
{
    int nAngle = (int)(angle * 10);
    
    nAngle = nAngle % 3600;
    
    if (nAngle < 0)
    {
        nAngle += 3600;
    }
    return cos_Predefined[nAngle];
}

float PixMath::fast_sinf(float angle)
{
    angle -= 90;
    return fast_cosf(angle);
}

bool PixMath::getIsinRect(Rect rect, Vec2 pt)
{
    return (rect.origin.x < pt.x && rect.origin.y < pt.y && rect.origin.x + rect.size.width > pt.x && rect.origin.y + rect.size.height > pt.y);
}

Rect PixMath::getIntersectRect(const Rect &rect1, const Rect &rect2)
{
    //-----------------------------------------------------------------------
    // 1. rect1 가 rect2 영역에서 완전히 벗어날 경우
    //-----------------------------------------------------------------------
    if( (rect1.origin.x >= rect2.origin.x + rect2.size.width ) ||
       ( rect1.origin.y >= rect2.origin.y + rect2.size.height ) ||
       ( (rect1.origin.x + rect1.size.width) <= rect2.origin.x) ||
       ( rect1.origin.y + rect1.size.height <= rect2.origin.y) )
    {
        return Rect(0, 0, 0, 0);	// 전체가 클리핑 됨
    }
    
    double ClippedRectLeft		= 0;
    double ClippedRectTop		= 0;
    double ClippedRectRight	= 0;
    double ClippedRectBottom	= 0;
    
    //-----------------------------------------------------------------------
    // 2. rect1 가 부분적으로 rect2 에 걸쳐 있을시
    //-----------------------------------------------------------------------
    ClippedRectLeft		= rect2.origin.x;			//left
    ClippedRectTop		= rect2.origin.y;			//top
    ClippedRectRight	= rect2.origin.x + rect2.size.width - 1.0;		//right
    ClippedRectBottom	= rect2.origin.y + rect2.size.height - 1.0;	//bottom
    
    //2.Clipping 영역을 screen_size에 맞게 재 지정한다.
    if( ClippedRectLeft < rect1.origin.x )
    {	ClippedRectLeft = rect1.origin.x;	}
    
    if( ClippedRectTop < rect1.origin.y )
    {	ClippedRectTop = rect1.origin.y;		}
    
    if( ClippedRectRight > (rect1.origin.x + rect1.size.width - 1.0) )
    {	ClippedRectRight = (rect1.origin.x + rect1.size.width - 1.0);		}
    
    if( ClippedRectBottom > (rect1.origin.y + rect1.size.height - 1.0) )
    {	 ClippedRectBottom = (rect1.origin.y + rect1.size.height - 1.0);	}
    
    return Rect( ClippedRectLeft, ClippedRectTop, ClippedRectRight - ClippedRectLeft + 1, ClippedRectBottom - ClippedRectTop + 1);
}


float PixMath::getLength(Vec2 pt1, Vec2 pt2)
{
    return ((float)sqrtf(powf(pt1.x - pt2.x, 2) + powf(pt1.y - pt2.y, 2)));
}

float PixMath::getLength(Vec2 pt1)
{
    return (float)sqrtf(pt1.x * pt1.x + pt1.y * pt1.y);
}

float PixMath::getAngle(Vec2 pt1, Vec2 pt2)
{
    float r = atan2f(pt2.y - pt1.y, pt2.x - pt1.x) * 180.0 / 3.141592;
    if (r < 0)
        r += 360;
    return r;
}

float PixMath::getDecimal(float value)
{
    return value - (int)value;
}


float PixMath::getRandom(float fStart, float fEnd, int nMaxDot)
{
    if (fStart > fEnd)
    {
        float fValue = fStart;
        fStart = fEnd;
        fEnd = fValue;
    }
    
    int nStart = fStart * nMaxDot;
    int nEnd = fEnd * nMaxDot;
    
    
    if (nStart == nEnd)
        return fStart;
    int nGap = (nEnd - nStart + 1);
    if (nGap < 0)
        nGap = -nGap;
    int nResult = rand() % nGap;
    
    
    
    return fStart + ((float)nResult / (float)(nMaxDot));
}
float PixMath::getRandom(unsigned char nStart, unsigned char nEnd)
{
    return nStart + (rand() % (nEnd - nStart + 1));
}
Rect PixMath::getSumRect       (Rect r1, Rect r2)
{
    Vec2 p1[6];
    p1[0].x = r1.origin.x;
    p1[0].y = r1.origin.y;
    p1[1].x = r1.origin.x + r1.size.width;
    p1[1].y = r1.origin.y;
    p1[2].x = r1.origin.x;
    p1[2].y = r1.origin.y + r1.size.height;
    p1[3].x = r2.origin.x;
    p1[3].y = r2.origin.y;
    p1[4].x = r2.origin.x + r2.size.width;
    p1[4].y = r2.origin.y;
    p1[5].x = r2.origin.x;
    p1[5].y = r2.origin.y + r2.size.height;
    
    float fMinX = 99999999;
    float fMaxX = -99999999;
    float fMinY = 99999999;
    float fMaxY = -99999999;
    for (int i = 0; i < 6; ++i)
    {
        if (p1[i].x != -99999999)
        {
            if (p1[i].x < fMinX)
            {
                fMinX = p1[i].x;
            }
            if (p1[i].x > fMaxX)
            {
                fMaxX = p1[i].x;
            }
        }
        if (p1[i].y != -99999999)
        {
            if (p1[i].y < fMinY)
            {
                fMinY = p1[i].y;
            }
            if (p1[i].y > fMaxY)
            {
                fMaxY = p1[i].y;
            }
        }
    }
    return Rect(fMinX, fMinY, (fMaxX - fMinX), (fMaxY - fMinY));
    
}
std::vector<Vec2>* PixMath::getCardinalSplineCurve(Vec2 *p, int nCount, float rez, int div)
{
    if(p == NULL || nCount <= 0 || rez <= 0 || div <= 0)
    {
        CCLOG("인자값을 확인해주십시오.");
        return NULL;
    }
    
    float px, py;
    Vec2 m0, m1, m2, m3;
    div += 1;
    std::vector<Vec2> *totalPoints = new std::vector<Vec2>();
    
    for (int n = 0; n < nCount - 1; ++n)
    {
        m0 = (n < 1) ? p[nCount - 1] : p[n - 1];
        m1 = p[n];
        m2 = p[n + 1];
        m3 = (n < nCount - 2) ? p[n + 2] : p[0];
        
        std::vector<Vec2> points;
        std::vector<float> currentLengths;
        for (float t = 0; t < 1; t += rez)
        {
            if(n + t != 0)
            {
                float s = 1;
                
                px =    s * ( -t * t * t + 2 * t * t - t) * m0.x +
                s * ( -t * t * t + t * t) * m1.x +
                (2 * t * t * t -3 * t * t + 1) * m1.x +
                s * (t * t * t -2 * t * t + t) * m2.x +
                (-2 * t * t * t + 3 * t * t) * m2.x +
                s * (t * t * t - t * t) * m3.x;
                
                py =    s * ( -t * t * t + 2 * t * t - t) * m0.y +
                s * ( -t * t * t + t * t) * m1.y +
                (2 * t * t * t -3 * t * t + 1) * m1.y +
                s * (t * t * t -2 * t * t + t) * m2.y +
                (-2 * t * t * t + 3 * t * t) * m2.y +
                s * (t * t * t - t * t) * m3.y;
                
                points.push_back(Vec2(px, py));
            }
        }
        
        int nPointCount = (int)points.size();
        float fTotalLength = 0;
        currentLengths.push_back(0);
        if (nPointCount >= 2)
        {
            for (int i = 1; i < nPointCount; ++i)
            {
                float size = PixMath::sharedObject()->getLength(points[i - 1], points[i]);
                fTotalLength += size;
                currentLengths.push_back(fTotalLength);
            }
        }
        
        int nLoopCount = 0;
        float fMinLength = fTotalLength / div;
        for (int i = 0; i < nPointCount; ++i)
        {
            if (currentLengths[i] > fMinLength && nLoopCount < div - 1)
            {
                totalPoints->push_back(points[i]);
                fMinLength += (fTotalLength / div);
                ++nLoopCount;
            }
        }
    }
    
    std::vector<Vec2>::iterator itor_begin;
    std::vector<Vec2>::iterator itor_end;
    std::vector<Vec2>::iterator itor2_begin;
    std::vector<Vec2>::iterator itor2_end;
    
    itor_begin = totalPoints->begin();
    itor_end = itor_begin;
    
    for(int i = 0; i<div-1; ++i)
    {
        itor_end ++;
    }
    
    itor2_begin = totalPoints->begin();
    for (int i = 0; i < totalPoints->size() - (div-1); ++i)
    {
        itor2_begin ++;
    }
    itor2_end = itor2_begin;
    
    for(int i = 0; i<div-1; ++i)
    {
        itor2_end ++;
    }
    
    totalPoints->erase(itor2_begin, itor2_end);
    totalPoints->erase(itor_begin, itor_end);
    
    return totalPoints;
}

std::vector<Vec2>*        PixMath::getStraightLine(Vec2 *p, int nCount, float fGap)
{
    std::vector<Vec2> *totalPoints = new std::vector<Vec2>();
    
    for (int i = 0; i < nCount - 1; ++i)
    {
        Vec2 ptStart = *(p + i);
        Vec2 ptDest = *(p + i + 1);
        
        Vec2 ptDir = PixMath::sharedObject()->NormalVector(ptStart, ptDest);
        float fLength = PixMath::sharedObject()->getLength(ptStart, ptDest);
        int nCount = fLength / fGap;
        ptDir *= fGap;
        for (int j = 0; j < nCount; ++j)
        {
            totalPoints->push_back(ptStart);
            ptStart += ptDir;
        }
    }
    return totalPoints;
}

float   PixMath::tween_easeInOutSine(float start, float end, float percent)
{
    float change = end - start;
    
    percent = (percent > 1)? 1.0 : percent;
    
    return -change * 0.5f * (cosf(PixMath_PI * percent) - 1) + start;
}

float   PixMath::tween(float start, float end, float percent)
{
    percent = (percent > 1)? 1.0 : percent;
    
    return (1 - percent) * start + percent * end;
}

Vec2 PixMath::tween(Vec2 start, Vec2 end, float percent)
{
    percent = (percent > 1)? 1.0 : percent;
    
    //    if(percent < 0.5)
    //    {
    //        float easeInPercent = (percent * percent);
    //
    //        float fX = (1 - easeInPercent) * start.x + easeInPercent * end.x;
    //        float fY = (1 - easeInPercent) * start.y + easeInPercent * end.y;
    //        return BpPoint_Make(fX, fY);
    //    }
    //    else
    //    {
    //        float easeOutPercent = (2 * percent) - (percent * percent);
    //
    //        float fX = (1 - easeOutPercent) * start.x + easeOutPercent * end.x;
    //        float fY = (1 - easeOutPercent) * start.y + easeOutPercent * end.y;
    //        return Vec2(fX, fY);
    //    }
    
    float fX = (1 - percent) * start.x + percent * end.x;
    float fY = (1 - percent) * start.y + percent * end.y;
    return Vec2(fX, fY);
}

int   PixMath::getRoundUp        (float fValue)
{
    if (fValue - (int)fValue != 0)
    {
        return (int)fValue + 1;
    }
    return (int)fValue;
}
