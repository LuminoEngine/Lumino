
#pragma once
#include "List.h"
#include "String.h"

LN_NAMESPACE_BEGIN

/**
	@brief	文字列の配列です。
*/
template<typename TChar>
class GenericStringArray
	: public List< GenericString<TChar> >
{
public:
	typedef List< GenericString<TChar> >	ArrayType;
	typedef GenericString<TChar>			StringType;


public:
	GenericStringArray() {}

	/** 初期化子リストから作成します。*/
	GenericStringArray(std::initializer_list<const TChar*> list)
	{
		ArrayType::Reserve(list.size());
		for (const TChar* str : list) {
			ArrayType::Add(String(str));
		}
	}

	virtual ~GenericStringArray() {}

public:

	/** 指定した文字列がこの配列内に存在するかどうかを判断します。*/
	bool Contains(const TChar* str) const
	{
		return std::find(ArrayType::begin(), ArrayType::end(), str) != ArrayType::end();
	}
	bool Contains(const StringType& str) const		/**< @overload EndsWith */
	{
		return Contains(str.c_str());
	}


	int IndexOf(const TChar* str, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const
	{
		int len = StringTraits::tcslen(str);
		for (int i = startIndex; i < ArrayType::GetCount(); ++i)
		{
			const StringType& item = ArrayType::GetAt(i);
			if (StringTraits::Compare(item.c_str(), item.GetLength(), str, len, -1, cs) == 0)
			{
				return i;
			}
		}
		return -1;
	}

};

typedef GenericStringArray<TCHAR>	StringArray;
typedef GenericStringArray<char>	StringArrayA;
typedef GenericStringArray<wchar_t>	StringArrayW;

LN_NAMESPACE_END
