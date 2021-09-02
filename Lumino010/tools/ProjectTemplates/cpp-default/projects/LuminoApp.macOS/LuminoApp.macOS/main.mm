
#include <LuminoEngine.hpp>
#include <LuminoEngine/Platform/CocoaPlatformInterface.hpp>
#import <AppKit/AppKit.h>

int main(int argc, const char * argv[])
{
    ln::GlobalLogger::addStdErrAdapter();

#if defined(LN_DEBUG) && defined(LN_XCODE_PROJECT_DIR)
	auto projectRoot = ln::Path(ln::String::fromCString(LN_XCODE_PROJECT_DIR)).parent().parent();
    ln::EngineSettings::addAssetDirectory(ln::Path::combine(projectRoot, u"Assets"));
    ln::EngineSettings::addAssetDirectory(ln::Path::combine(projectRoot, u".ln", u"Assets"));
#endif

    NSBundle* bundle = [NSBundle mainBundle];
    NSString* assetsPath = [bundle pathForResource:@"Assets" ofType:@"lca"];
    ln::EngineSettings::addAssetArchive(
        ln::String::fromStdString(std::string([assetsPath UTF8String])));

    return ln::CocoaPlatformInterface::Main();
}
