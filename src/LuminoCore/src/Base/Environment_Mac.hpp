#pragma once
#include <unistd.h>
#import <Foundation/Foundation.h>
//#import <Foundation/NSString.h>
//class NSString;

//typedef struct objc_object NSString;
extern "C" NSString* NSTemporaryDirectory();

extern std::string NSStringToStdString(NSString* nsstring);

namespace ln {

class PlatformEnvironment
{
public:
	using CharType = char;
	using StringType = std::string;
	
	static void getCurrentDirectory(StringType* outPath)
	{
		char* p = getcwd(NULL, 0);
		size_t len = strlen(p);
		outPath->resize(len);
		strncpy(&((*outPath)[0]), p, len);
		free(p);
	}

	static StringType getExecutablePath()
	{
		LN_NOTIMPLEMENTED();
		return StringType();
	}

	static void setEnvironmentVariable(const StringRef& variableName, const StringRef& value)
	{
		putenv(ln::String::format(u"{0}={1}", variableName, value).toStdString().c_str());
	}

	static void getSpecialFolderPath(SpecialFolder specialFolder, StringType* outPath)
	{
		if (specialFolder == SpecialFolder::Temporary) {
			if (NSString* path = NSTemporaryDirectory()) {
				*outPath = [path UTF8String];
				return;
			}
		}
		LN_NOTIMPLEMENTED();
	}
	
#if 0
	void getSpecialFolderPath(SpecialFolder specialFolder, LocalStringConverter<CharType>* out)
	{
		short domain = kOnAppropriateDisk;
		
		OSType type = kDesktopFolderType;
		switch (specialFolder)
		{
			case SpecialFolder::ApplicationData:
				type = kApplicationSupportFolderType;
				break;
			case SpecialFolder::Temporary:
				type = kTemporaryFolderType;
				break;
			default:
				LN_THROW(0, ArgumentException);
				break;
		}
		
		FSRef ref;
		if (FSFindFolder(domain, type, false, &ref) != 0) {
			LN_THROW(0, RuntimeException);
			return;
		}
		
		
		
		ByteBuffer buf(2048);
		if (FSRefMakePath(&ref, reinterpret_cast<UInt8 *>(buf.getData()), buf.getSize()) != noErr) {
			LN_THROW(0, RuntimeException);
			return;
		}
		
		String path = String::fromCString((const char*)buf.getConstData(), buf.getSize());
		StringTraits::tstrcpy(outPath, LN_MAX_PATH, path.c_str());
	}
#endif
};

} // namespace ln

