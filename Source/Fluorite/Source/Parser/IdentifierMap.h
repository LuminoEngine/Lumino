
#pragma once
#include <map>
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace parser
{

template<typename TValue>
class IdentifierMap
{
public:
	IdentifierMap()
		: m_table1(nullptr)
	{
		m_table1 = LN_NEW Item[TotalTableSize];
	}

	virtual ~IdentifierMap()
	{
		LN_SAFE_DELETE_ARRAY(m_table1);
	}

	void Insert(const TokenChar* keyBegin, const TokenChar* keyEnd, TValue value)
	{
		assert(keyBegin);
		assert(keyEnd);
		assert(keyBegin < keyEnd);
		int len = keyEnd - keyBegin;
		if (len < MaxKeyLength)
		{
			Item* item = GetItem(*keyBegin, *(keyEnd - 1), len);
			if (item != nullptr)
			{
				memcpy(item->key.data(), keyBegin, len);
				item->item = value;
				return;
			}
		}

		TokenString key(keyBegin, len);
		m_table2[key] = value;
	}

	bool Find(const TokenChar* keyBegin, const TokenChar* keyEnd, TValue* outValue, CaseSensitivity cs) const
	{
		int len = keyEnd - keyBegin;
		if (len < MaxKeyLength)
		{
			Item* item = GetItem(*keyBegin, *(keyEnd - 1), len);
			if (item != nullptr && item->key[0] != '\0')
			{
				if (StringTraits::Compare(keyBegin, item->key.data(), len, cs) == 0)
				{
					*outValue = item->item;
					return true;
				}
			}
		}

		auto itr = m_table2.find(TokenString(keyBegin, keyEnd - keyBegin));	// TODO: StringRef で検索できるように
		if (itr != m_table2.end())
		{
			*outValue = itr->second;
			return true;
		}
		return false;
	}

	void Copy(const IdentifierMap& srcMap)
	{
		memcpy(m_table1, srcMap.m_table1, TotalTableSize);
		m_table2 = srcMap.m_table2;
	}

public:

	/*
		a b c d e f g h i j k l m
		n o p q r s t u v w x y z
		A B C D E F G H I J K L M
		N O P Q R S T U V W X Y Z _

		0 1 2 3 4 5 6 7 8 9

		universal-character-name:
			\u hex-quad
			\U hex-quad hex-quad
	*/

	static const int MaxKeyFirstCount = 64;
	static const int MaxKeyLastCount = 64;
	static const int MaxKeyLength = 64;
	static const int TotalTableSize = MaxKeyFirstCount * MaxKeyLastCount * MaxKeyLength;

	struct Item
	{
		std::array<TokenChar, MaxKeyLength>	key = {};	// [0] が '\0' ならInsertされていないということ
		TValue								item;
	};

	//typedef Array<Item>	ItemArray;

	Item*					m_table1;	// [先頭文字][終端文字][文字数]
	//std::unordered_map<StringA, Item>	m_table2;
	std::map<TokenString, TValue>	m_table2;

	// 先頭文字、終端文字、長さから Item を取得する。名前が一致するかは改めてチェックすること。
	Item* GetItem(TokenChar first, TokenChar last, int len) const
	{
		static int charIndexTable[128] =
		{
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,			// '0'~'9'
			-1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,	// 'A'~'O'
			25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, 36,	// 'P'~'Z', '_'
			-1, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,	// 'a'~'o'
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, -1, -1, -1, -1, -1,
		};
		assert(first < 128);
		assert(last < 128);
		int i1 = charIndexTable[first];
		int i2 = charIndexTable[last];
		if (i1 == -1) return nullptr;
		if (i2 == -1) return nullptr;
		return &m_table1[(i1 * MaxKeyLastCount * MaxKeyLength) + (i2 * MaxKeyLength) + len];
	}
};

} // namespace Parser
LN_NAMESPACE_END

