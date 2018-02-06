#pragma once

LN_NAMESPACE_BEGIN

// Unix
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
	
#ifdef LN_OS_MAC
	void getSpecialFolderPath(SpecialFolder specialFolder, LocalStringConverter<CharType>* out)
	{
#if 0
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
#endif
	}
#endif
};

LN_NAMESPACE_END

