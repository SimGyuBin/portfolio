//
//  CustomNavigationController.m
//  coco2dx_pixholic
//
//  Created by 심규빈 on 2016. 7. 5..
//
//

#import "CustomNavigationController.h"
#include "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"

#define degreesToRadian(x) (M_PI * (x) / 180.0)

@implementation CustomNavigationController

- (BOOL)shouldAutorotate
{
    return NO;
//    return self.topViewController.shouldAutorotate;
}

- (NSUInteger)supportedInterfaceOrientations
{
    return self.topViewController.supportedInterfaceOrientations;
}

- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
    return self.topViewController.preferredInterfaceOrientationForPresentation;
}

- (BOOL)prefersStatusBarHidden
{
    return self.topViewController.prefersStatusBarHidden;
}

//- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
//{
//        [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
//    
//        // Code here will execute before the rotation begins.
//        // Equivalent to placing it in the deprecated method -[willRotateToInterfaceOrientation:duration:]
//    
//        [coordinator animateAlongsideTransition:^(id<UIViewControllerTransitionCoordinatorContext> context) {
//    
//            // Place code here to perform animations during the rotation.
//            // You can pass nil or leave this block empty if not necessary.
//    
//        } completion:^(id<UIViewControllerTransitionCoordinatorContext> context) {
//    
//            // Code here will execute after the rotation has finished.
//            // Equivalent to placing it in the deprecated method -[didRotateFromInterfaceOrientation:]
//            
//        }];
//}

- (void) changeOrientation:(int)flag
{
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    double x = screenBounds.origin.x;
    double y = screenBounds.origin.y;
    double width = screenBounds.size.width;
    double height = screenBounds.size.height;
    
    cocos2d::Director *director = cocos2d::Director::getInstance();
    CCEAGLView *eaglView = (CCEAGLView*)director->getOpenGLView()->getEAGLView();
    
    if(flag == 0)
    {
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait animated:YES];
        eaglView.transform = CGAffineTransformMakeRotation(degreesToRadian(0));
        
        CGRect convertSize;
        
        if(width > height)
        {
            convertSize = CGRectMake(x,y,height,width);
        }
        else
        {
            convertSize = CGRectMake(x,y,width,height);
        }
        
        eaglView.frame = convertSize;
        eaglView.bounds = convertSize;
    }
    else if(flag == 1)
    {
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:YES];
        
        eaglView.transform = CGAffineTransformMakeRotation(degreesToRadian(90));
        eaglView.frame =  CGRectMake(x,y, width , height);
        eaglView.bounds = CGRectMake(x,y, height, width);
    }
}

@end
