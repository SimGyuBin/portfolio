//
//  PixImagePicker.h
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 8..
//
//

#ifndef PixImagePicker_h
#define PixImagePicker_h

#include "cocos2d.h"

class PixImagePickerDelegate;

class PixImagePicker : public cocos2d::Layer
{
private:
    
    PixImagePickerDelegate *mDelegate;
    
public:
    
    PixImagePicker(PixImagePickerDelegate *delegate);
    ~PixImagePicker();
    
    void showImagePicker(const char *savePhotoName);
    
    void successImagePicker();
    void canceledImagePicker();
    
};


class PixImagePickerDelegate {
    
public:
    
    virtual void successImagePicker(){};
    virtual void canceledImagePicker(){};
    
};


#endif /* PixImagePicker_h */
