//
//  GameViewController.m
//  LuminoApp.iOS
//
//  Created by lriki on 2018/09/02.
//  Copyright © 2018年 Lumino. All rights reserved.
//

#import "GameViewController.h"
#import "Renderer.h"

@implementation GameViewController
{
    //MTKView *_view;
	GLKView* _view;

    Renderer *_renderer;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

	
	// Created by storyboard
    _view = (GLKView *)self.view;

    //_view.device = MTLCreateSystemDefaultDevice();
    _view.backgroundColor = UIColor.clearColor;

	/*
    if(!_view.device)
    {
        NSLog(@"Metal is not supported on this device");
        self.view = [[UIView alloc] initWithFrame:self.view.frame];
        return;
    }
	 */

    _renderer = [[Renderer alloc] initWithOpenGLKitView:_view];

    //[_renderer mtkView:_view drawableSizeWillChange:_view.bounds.size];

	
    _view.delegate = _renderer;
	
	
}

@end
