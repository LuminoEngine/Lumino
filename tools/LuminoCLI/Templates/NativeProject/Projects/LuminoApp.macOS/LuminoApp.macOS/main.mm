
#include <LuminoEngine.hpp>
#include <LuminoEngine/Platform/CocoaPlatformInterface.hpp>
#import <AppKit/AppKit.h>

int main(int argc, const char * argv[])
{
	ln::GlobalLogger::addStdErrAdapter();
	
	NSBundle* bundle = [NSBundle mainBundle];
	NSString* assetsPath = [bundle pathForResource:@"Assets" ofType:@"lca"];

	ln::CocoaPlatformInterface::addAssetArchive(
		ln::String::fromStdString(std::string([assetsPath UTF8String])));

	return ln::CocoaPlatformInterface::Main();
}
