//
//  PixImagePicker.mm
//  Pzle
//
//  Created by 심규빈 on 2016. 11. 8..
//
//

#import "PixImagePicker.h"
#import "UIImagePickerBridge.h"

UIImagePickerBridge *imagePicker = nil;

PixImagePicker::PixImagePicker(PixImagePickerDelegate *delegate)
{
    mDelegate = delegate;
    imagePicker = [[UIImagePickerBridge alloc] init];
    [imagePicker setPixImagePicker:this];
}

PixImagePicker::~PixImagePicker()
{
    if(imagePicker)
    {
        [imagePicker release]; imagePicker = nil;
    }
}


void PixImagePicker::showImagePicker(const char *savePhotoName)
{
    if([imagePicker isShow] == NO)
    {
        NSString *imageName = [NSString stringWithUTF8String:savePhotoName];
        [imagePicker showImagePicker:imageName];
    }
    
}

void PixImagePicker::successImagePicker()
{
    if(mDelegate)
        mDelegate->successImagePicker();
}

void PixImagePicker::canceledImagePicker()
{
    if(mDelegate)
        mDelegate->canceledImagePicker();
}
