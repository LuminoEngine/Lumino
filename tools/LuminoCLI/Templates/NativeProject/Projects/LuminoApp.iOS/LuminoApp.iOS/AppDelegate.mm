//
//  AppDelegate.m
//  LuminoApp.iOS
//
//  Created by lriki on 2018/09/02.
//  Copyright © 2018年 Lumino. All rights reserved.
//

#import "AppDelegate.h"
#include "GameViewController.h"
#include <LuminoEngine/Platform/iOSPlatformInterface.hpp>

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	// Override point for customization after application launch.
	
	// Create lumino app instance
	
	// Initialize GLView (initialize engine)
	
	// Create UIWindow
	//_window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
	
	
	
	// Create view controller
	//_viewController = [[GameViewController alloc]init];
	
	// Set RootViewController to window
	//[_window addSubview: _viewController.view];
	
	//[_window makeKeyAndVisible];
	
	

	// Window 作成
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];

	// initWithNibName:nil で、nib(storyboard) からの読み込みを行わないようにする
    _viewController = [[GameViewController alloc] initWithNibName:nil bundle:nil];
    _viewController.wantsFullScreenLayout = YES;
    [window setRootViewController:_viewController];
    //self.window.rootViewController = _viewController;

	// Windowを表示する
    [self.window makeKeyAndVisible];

	// Hide status bar
    [[UIApplication sharedApplication] setStatusBarHidden:true];

	return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
	// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
	// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
	ln::iOSPlatformInterface::applicationDidEnterBackground();
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
	// Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
	ln::iOSPlatformInterface::applicationWillEnterForeground();
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}


- (void)applicationWillTerminate:(UIApplication *)application {
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
	ln::iOSPlatformInterface::nativeFinalize();
}


@end
