
#import "RootGLView.h"
#import "GameViewController.h"
#import "Renderer.h"
#include <LuminoEngine/Platform/iOSPlatformInterface.hpp>

@implementation GameViewController
{
	RootGLView* _view;
    Renderer* _renderer;
}

// nibを使用せずにプログラムでビュー階層を作成する
- (void)loadView {
    _view = [RootGLView viewWithFrame: [UIScreen mainScreen].bounds];
    self.view = _view;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
#if 0
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
#endif

    _renderer = [[Renderer alloc] initWithOpenGLKitView:_view];

    //[_renderer mtkView:_view drawableSizeWillChange:_view.bounds.size];

    _view.delegate = _renderer;
}
	
// UI が回転した
- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
    CGSize s = CGSizeMake([_view getWidth], [_view getHeight]);
    ln::iOSPlatformInterface::applicationScreenSizeChanged((int) s.width, (int) s.height);
}

@end
