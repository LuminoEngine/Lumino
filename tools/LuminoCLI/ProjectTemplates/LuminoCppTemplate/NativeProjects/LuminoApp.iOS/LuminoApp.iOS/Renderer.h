//
//  Renderer.h
//  LuminoApp.iOS
//
//  Created by lriki on 2018/09/02.
//  Copyright © 2018年 Lumino. All rights reserved.
//

//#import <MetalKit/MetalKit.h>
#import <GLKit/GLKit.h>

// Our platform independent renderer class.   Implements the MTKViewDelegate protocol which
//   allows it to accept per-frame update and drawable resize callbacks.
@interface Renderer : NSObject<GLKViewDelegate>// <MTKViewDelegate>

-(nonnull instancetype)initWithOpenGLKitView:(nonnull GLKView *)view;

@end

