
#import <mach-o/dyld.h>
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
//#include <CoreFoundation/CoreFoundation.h>
//#include <Security/Security.h>

#import <objc/runtime.h>
#include <string>
#include <Lumino/Base/Environment.hpp>
#include "Environment_Mac.hpp"

//std::string NSStringToStdString(NSString* nsstring) {
 //   return [nsstring UTF8String];
//}

namespace ln {

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
	if (specialFolder == SpecialFolder::ApplicationData)
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
	}
	else if (specialFolder == SpecialFolder::Temporary) {
		if (NSString* path = NSTemporaryDirectory()) {
			*outPath = [path UTF8String];
			return;
		}
	}
	else {
		LN_UNREACHABLE();
	}
}
} // namespace ln

