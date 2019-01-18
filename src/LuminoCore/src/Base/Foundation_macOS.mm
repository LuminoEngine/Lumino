
#import <mach-o/dyld.h>
//#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
//#include <CoreFoundation/CoreFoundation.h>
//#include <Security/Security.h>

#import <objc/runtime.h>
#include <string>
#include <LuminoCore/Base/Environment.hpp>
#include "Environment_Mac.hpp"

//std::string NSStringToStdString(NSString* nsstring) {
 //   return [nsstring UTF8String];
//}

namespace ln {

void writeNSLog(const char* str)
{
	NSLog([NSString stringWithCString: str encoding:NSUTF8StringEncoding]);
}

PlatformEnvironment::StringType PlatformEnvironment::getExecutablePath()
{
	char path[1024];
	uint32_t size = sizeof(path);
	if (_NSGetExecutablePath(path, &size) == 0) {
		return path;
	}
	else {
		LN_ENSURE(0);
		return StringType();
	}
}

void PlatformEnvironment::getSpecialFolderPath(SpecialFolder specialFolder, StringType* outPath)
{
	switch (specialFolder)
	{
		case SpecialFolder::ApplicationData:
		{
			NSFileManager* fileManager = [NSFileManager defaultManager];
			if (LN_ENSURE(fileManager)) {
				return;
			}
			
			NSArray* urls = [fileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];
			if (LN_ENSURE(urls)) {
				return;
			}
			if (LN_ENSURE(urls.count > 0)) {
				return;
			}
			
			NSURL* url = [urls objectAtIndex:0];
			NSString* path = url.path;
			*outPath = [path UTF8String];
			break;
		}
		case SpecialFolder::Temporary:
		{
			if (NSString* path = NSTemporaryDirectory()) {
				*outPath = [path UTF8String];
				return;
			}
			break;
		}
		case SpecialFolder::Home:
		{
			const char* path = ::getenv("HOME");
			if (path)
				*outPath = path;
			else
				*outPath = "/";
			break;
		}
		default:
			LN_UNREACHABLE();
			break;
	}
}

uint64_t PlatformEnvironment::getTickCount()
{
	auto time = [NSDate timeIntervalSinceReferenceDate];
	return (uint64_t)time;
}

} // namespace ln

