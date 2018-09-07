
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

#import <objc/runtime.h>
#include <string>
#include <Lumino/Base/Environment.hpp>
#include "Environment_Mac.hpp"

//std::string NSStringToStdString(NSString* nsstring) {
 //   return [nsstring UTF8String];
//}

namespace ln {

void PlatformEnvironment::getSpecialFolderPath(SpecialFolder specialFolder, StringType* outPath)
{
	if (specialFolder == SpecialFolder::Temporary) {
		if (NSString* path = NSTemporaryDirectory()) {
			*outPath = [path UTF8String];
			return;
		}
	}
	LN_NOTIMPLEMENTED();
}
} // namespace ln

