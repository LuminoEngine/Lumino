
#import <UIKit/UIKit.h>

@class GameViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property(nonatomic, readonly) GameViewController* viewController;
@property (strong, nonatomic) UIWindow *window;

@end
