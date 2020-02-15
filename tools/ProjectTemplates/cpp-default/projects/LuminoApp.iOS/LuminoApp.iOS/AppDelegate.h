
#import <UIKit/UIKit.h>

@class GameViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property(readwrite) GameViewController* viewController;
@property (strong, nonatomic) UIWindow *window;

@end
