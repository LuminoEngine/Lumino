
#pragma once
#include <map>
#include "String.h"

LN_NAMESPACE_BEGIN

namespace InternalResource
{
	extern const String UnknownError;
	extern const String VerifyError;
	extern const String ArgumentError;
	extern const String InvalidOperationError;
	extern const String NotImplementedError;
	extern const String OutOfMemoryError;
	extern const String OutOfRangeError;
	extern const String KeyNotFoundError;
	extern const String OverflowError;
	extern const String IOError;
	extern const String FileNotFoundError;
	extern const String DirectoryNotFoundError;
	extern const String InvalidFormatError;
	extern const String EndOfStreamError;
	extern const String EncodingError;
	extern const String RuntimeError;
	extern const String Win32Error;
	extern const String ComError;
	extern const String OpenGLError;

	extern const String XmlException;
	extern const String Xml_TagMismatch;


	extern const String& GetString(const String& name);
	extern void SetString(const String& name, const String& value);
};

LN_NAMESPACE_END
