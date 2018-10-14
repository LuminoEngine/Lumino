
#import <GLKit/GLKit.h>

@interface Renderer : NSObject<GLKViewDelegate>

-(nonnull instancetype)initWithOpenGLKitView:(nonnull GLKView *)view;

@end

