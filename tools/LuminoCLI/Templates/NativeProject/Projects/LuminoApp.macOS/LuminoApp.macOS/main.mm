
#include <LuminoEngine.hpp>
#include <LuminoEngine/Platform/CocoaPlatformInterface.hpp>
#import <AppKit/AppKit.h>

int main(int argc, const char * argv[])
{
    ln::GlobalLogger::addStdErrAdapter();

#ifdef LN_DEBUG
    auto projectRoot = ln::Path(ln::Environment::executablePath()).parent().parent().parent().parent().parent();
    ln::EngineSettings::addAssetDirectory(ln::Path::combine(projectRoot, u"Assets"));
    ln::EngineSettings::addAssetDirectory(ln::Path::combine(projectRoot, u".ln", u"Assets"));
#endif

    NSBundle* bundle = [NSBundle mainBundle];
    NSString* assetsPath = [bundle pathForResource:@"Assets" ofType:@"lca"];
    ln::Engine::addAssetArchive(
        ln::String::fromStdString(std::string([assetsPath UTF8String])));

    return ln::CocoaPlatformInterface::Main();
}
