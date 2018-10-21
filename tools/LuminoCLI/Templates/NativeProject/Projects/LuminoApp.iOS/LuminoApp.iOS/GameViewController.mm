
#import "RootGLKView.h"
#import "GameViewController.h"
#import "Renderer.h"

@implementation GameViewController
{
	RootGLKView* _view;
    Renderer* _renderer;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	
	_view = (RootGLKView*)self.view;

    _renderer = [[Renderer alloc] initWithOpenGLKitView:_view];
    _view.delegate = _renderer;
}

@end
