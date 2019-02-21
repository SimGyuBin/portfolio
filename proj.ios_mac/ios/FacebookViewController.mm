//
//  CustomViewController.m
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 4..
//
//

#import "FacebookViewController.h"
#import "AppController.h"
#import <FBSDKShareKit/FBSDKShareKit.h>

@implementation FacebookViewController

- (id)init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void) displayView
{
    [[AppController appDelegate].navigationController popViewControllerAnimated:NO];
    [[AppController appDelegate].navigationController pushViewController:self animated:NO];
}

- (void) facebookShare:(std::string)strUrl
{
   [self displayView];
    
    NSString *url = [NSString stringWithUTF8String:strUrl.c_str()];
    
    FBSDKShareLinkContent *content = [[FBSDKShareLinkContent alloc] init];
    content.contentURL = [NSURL URLWithString:url];
    
    FBSDKShareDialog *dialog = [[FBSDKShareDialog alloc] init];
    
    dialog.mode = FBSDKShareDialogModeBrowser;
    dialog.shareContent = content;
    dialog.delegate = self;
    dialog.fromViewController = self;
    [dialog show];
    
}

- (void) facebookSharePhotoData:(cocos2d::Data*)pData
{
    [self displayView];
    
    NSData *nsData = [NSData dataWithBytes:pData->getBytes() length:pData->getSize()];
    UIImage *image = [UIImage imageWithData:nsData];
    
    FBSDKSharePhoto *photo = [[FBSDKSharePhoto alloc] init];
    photo.image = image;
    photo.userGenerated = YES;
    
    FBSDKSharePhotoContent *content = [[FBSDKSharePhotoContent alloc] init];
    content.photos = @[photo];
    
    FBSDKShareDialog *dialog = [[FBSDKShareDialog alloc] init];
    
    dialog.mode = FBSDKShareDialogModeAutomatic;
    dialog.shareContent = content;
    dialog.delegate = self;
    dialog.fromViewController = self;
    [dialog show];
}

- (void) facebookSharePhotoPath:(std::string)strPath
{
    [self displayView];
    
    NSString *path = [NSString stringWithUTF8String:strPath.c_str()];
    UIImage *image = [UIImage imageWithContentsOfFile:path];
    
    FBSDKSharePhoto *photo = [[FBSDKSharePhoto alloc] init];
    photo.image = image;
    photo.userGenerated = YES;
    
    FBSDKSharePhotoContent *content = [[FBSDKSharePhotoContent alloc] init];
    content.photos = @[photo];
    
    FBSDKShareDialog *dialog = [[FBSDKShareDialog alloc] init];
    
    dialog.mode = FBSDKShareDialogModeAutomatic;
    dialog.shareContent = content;
    dialog.delegate = self;
    dialog.fromViewController = self;
    [dialog show];
}

#pragma mark - FBSDKSharingDelegate

- (void)sharer:(id<FBSDKSharing>)sharer didCompleteWithResults:(NSDictionary *)results
{
    [[AppController appDelegate].navigationController popViewControllerAnimated:NO];
}

- (void)sharer:(id<FBSDKSharing>)sharer didFailWithError:(NSError *)error
{
    [[AppController appDelegate].navigationController popViewControllerAnimated:NO];
}

- (void)sharerDidCancel:(id<FBSDKSharing>)sharer
{
   [[AppController appDelegate].navigationController popViewControllerAnimated:NO];
}

@end
