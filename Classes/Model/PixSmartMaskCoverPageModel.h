//
//  PixSmartMaskCoverPageModel.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 2. 18..
//
//

#ifndef PixSmartMaskCoverPageModel_h
#define PixSmartMaskCoverPageModel_h

#include "cocos2d.h"
#include "PixSmartMaskPageModel.h"

USING_NS_CC;

class PixSmartMaskCoverPageModel : public PixSmartMaskPageModel
{

public:
    
    static PixSmartMaskCoverPageModel *create();
    
    PixSmartMaskCoverPageModel();
    virtual ~PixSmartMaskCoverPageModel();
    virtual PixSmartMaskCoverPageModel* clone();
    virtual void initRenderTexture();
    
    virtual void renderTextureH600(std::function<void (PixSmartMaskPageModel*)> callback);
    virtual void renderTextureH600Callback(RenderTexture* pRenderTexture, const std::string& filename);
    bool isH600Data();

    const Data& getH600Data()
    {
        return H600Data;
    }
    
    void clearH600Data();
        
private:
    
    Data H600Data;
    std::function<void (PixSmartMaskPageModel*)> _coverRenderCallBack;
    
    CC_SYNTHESIZE(bool, mbln_H600DataRender, H600DataRender)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _seneka_width, Seneka_width)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _seneka_text, Seneka_text)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _seneka_color, Seneka_color)
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _H600filename, H600filename)
};

#endif /* PixSmartMaskCoverPageModel_h */
