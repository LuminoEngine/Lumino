#pragma once

LN_NAMESPACE_BEGIN

class PlatformEnvironment
{
public:
	using CharType = wchar_t;

	static void getCurrentDirectory(LocalStringConverter<CharType>* out)
	{
		DWORD size = ::GetCurrentDirectoryW(0, NULL);
		out->alloc(size);
		::GetCurrentDirectoryW(size, out->data());
	}
};

LN_NAMESPACE_END

