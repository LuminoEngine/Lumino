//
//  AppDelegate.h
//  LuminoApp.iOS
//
//  Created by lriki on 2018/09/02.
//  Copyright © 2018年 Lumino. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GameViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property(nonatomic, readonly) GameViewController* viewController;
@property (strong, nonatomic) UIWindow *window;


@end

