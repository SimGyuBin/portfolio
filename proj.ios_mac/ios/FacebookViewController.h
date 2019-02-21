//
//  CustomViewController.h
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 4..
//
//

#import <UIKit/UIKit.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import <FBSDKShareKit/FBSDKShareKit.h>

@interface FacebookViewController : UIViewController <FBSDKSharingDelegate> {
    
}

- (void) facebookShare:(std::string)strUrl;
- (void) facebookSharePhotoData:(cocos2d::Data*)pData;
- (void) facebookSharePhotoPath:(std::string)strPath;
@end
