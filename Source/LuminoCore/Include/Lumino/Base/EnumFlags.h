/**
	@file	EnumExtension.h
*/

#pragma once
#include "List.h"
#include "String.h"
#include "StringArray.h"

namespace ln
{
typedef uint32_t FlagsType;

template<typename EnumType, typename TBitsType = FlagsType>
class Flags
{
public:
	using BitsType = TBitsType;

	Flags()
		: m_bits(0)
	{
	}

	Flags(EnumType bit)
		: m_bits(static_cast<BitsType>(bit))
	{
	}

	Flags(const Flags<EnumType>& rhs)
		: m_bits(rhs.m_bits)
	{
	}

	explicit Flags(BitsType flags)
		: m_bits(flags)
	{
	}

	bool isSet(EnumType value) const
	{
		BitsType bitValue = static_cast<BitsType>(value);
		return (m_bits & bitValue) == bitValue && (bitValue != 0 || m_bits == bitValue);
	}

	Flags<EnumType>& set(EnumType value)
	{
		m_bits |= static_cast<BitsType>(value);
		return *this;
	}

	void unset(EnumType value)
	{
		m_bits &= ~static_cast<BitsType>(value);
	}

	EnumType get() const
	{
		return static_cast<EnumType>(m_bits);
	}

	Flags<EnumType> & operator=(const Flags<EnumType>& rhs)
	{
		m_bits = rhs.m_bits;
		return *this;
	}

	Flags<EnumType> & operator|=(const Flags<EnumType>& rhs)
	{
		m_bits |= rhs.m_bits;
		return *this;
	}

	Flags<EnumType> & operator|=(EnumType rhs)
	{
		m_bits |= static_cast<BitsType>(rhs);
		return *this;
	}

	Flags<EnumType> & operator&=(const Flags<EnumType>& rhs)
	{
		m_bits &= rhs.m_bits;
		return *this;
	}

	Flags<EnumType> & operator&=(EnumType rhs)
	{
		m_bits &= static_cast<BitsType>(rhs);
		return *this;
	}

	Flags<EnumType> & operator^=(const Flags<EnumType>& rhs)
	{
		m_bits ^= rhs.m_bits;
		return *this;
	}

	Flags<EnumType> & operator^=(EnumType rhs)
	{
		m_bits ^= static_cast<BitsType>(rhs);
		return *this;
	}

	bool operator!() const
	{
		return !m_bits;
	}

	Flags<EnumType> operator~() const
	{
		Flags<EnumType> result(*this);
		result.m_bits = (BitsType)~m_bits;
		return result;
	}

	explicit operator bool() const
	{
		return !!m_bits;
	}

	explicit operator BitsType() const
	{
		return m_bits;
	}

	operator EnumType() const
	{
		return static_cast<EnumType>(m_bits);
	}

public:
	BitsType  m_bits;
};

#define LN_FLAGS_OPERATORS(EnumType) \
	inline bool operator==(EnumType lhs, EnumType rhs) { return static_cast<ln::Flags<EnumType>::BitsType>(lhs) == static_cast<ln::Flags<EnumType>::BitsType>(rhs); } \
	inline bool operator==(const ln::Flags<EnumType>& lhs, EnumType rhs) { return static_cast<ln::Flags<EnumType>::BitsType>(lhs.get()) == static_cast<ln::Flags<EnumType>::BitsType>(rhs); } \
	inline bool operator==(EnumType lhs, const ln::Flags<EnumType>& rhs) { return static_cast<ln::Flags<EnumType>::BitsType>(lhs) == static_cast<ln::Flags<EnumType>::BitsType>(rhs.get()); } \
	inline bool operator==(const ln::Flags<EnumType>& lhs, const ln::Flags<EnumType>& rhs) { return static_cast<ln::Flags<EnumType>::BitsType>(lhs.get()) == static_cast<ln::Flags<EnumType>::BitsType>(rhs.get()); } \
	inline bool operator!=(EnumType lhs, EnumType rhs) { return !operator==(lhs, rhs); } \
	inline bool operator!=(const ln::Flags<EnumType>& lhs, EnumType rhs) { return !operator==(lhs, rhs); } \
	inline bool operator!=(EnumType lhs, const ln::Flags<EnumType>& rhs) { return !operator==(lhs, rhs); } \
	inline bool operator!=(const ln::Flags<EnumType>& lhs, const ln::Flags<EnumType>& rhs) { return !operator==(lhs, rhs); } \
	inline ln::Flags<EnumType> operator|(EnumType lhs, EnumType rhs) { ln::Flags<EnumType> r(static_cast<ln::Flags<EnumType>::BitsType>(lhs) | static_cast<ln::Flags<EnumType>::BitsType>(rhs)); return r; } \
	inline ln::Flags<EnumType> operator&(EnumType lhs, EnumType rhs) { ln::Flags<EnumType> r(static_cast<ln::Flags<EnumType>::BitsType>(lhs) & static_cast<ln::Flags<EnumType>::BitsType>(rhs)); return r; } \
	inline ln::Flags<EnumType> operator^(EnumType lhs, EnumType rhs) { ln::Flags<EnumType> r(static_cast<ln::Flags<EnumType>::BitsType>(lhs) ^ static_cast<ln::Flags<EnumType>::BitsType>(rhs)); return r; } \
	inline ln::Flags<EnumType> operator~(EnumType a) { return ~ln::Flags<EnumType>(a); } \
	inline bool testFlag(EnumType lhs, EnumType rhs) \
	{ \
		auto lv = static_cast<ln::Flags<EnumType>::BitsType>(lhs); \
		auto rv = static_cast<ln::Flags<EnumType>::BitsType>(rhs); \
		return (lv & rv) == rv && (rv != 0 || lv == rv); \
	}

} // namespace ln


#ifndef LN_DOXYGEN

#if 0
/**
	@brief	拡張 enum 型を定義します。
	@see	Doc_EnumExtension_1
*/
#define LN_ENUM(enumName) \
	class enumName : public Enum \
	{ \
	public: \
enum _##enumName

/**
	@brief	拡張 enum 型を定義します。
	@see	Doc_EnumExtension_1
*/
#define LN_ENUM_DECLARE(enumName) \
	protected: \
		_##enumName	m_value; \
	public: \
		inline operator int() const { return m_value; } \
		typedef _##enumName enum_type; \
		typedef _##enumName value_type; \
		enumName() { m_value = (enum_type)(0); } \
		enumName(enum_type v) { m_value = v; } \
		virtual void setValue(EnumValueType value) override { m_value = (enum_type)value; } \
		virtual EnumValueType getValue() const override { return m_value; } \
		inline bool operator==(enumName right) const { return m_value == right.m_value; } \
		inline bool operator==(enum_type right) const { return m_value == right; } \
		inline bool operator!=(enumName right) const { return !operator==(right); } \
		inline bool operator!=(enum_type right) const { return !operator==(right); } \
	};

/**
	@brief	拡張 enum 型の型情報にアクセスする機能を定義します。
	@see	Doc_EnumExtension_1
*/
#define LN_ENUM_REFLECTION(enumName, ...) \
private: \
	struct LocalEnumParser : public EnumParser <_##enumName> { LocalEnumParser() { _##enumName values[] = { __VA_ARGS__ };  init(values, LN_ARRAY_SIZE_OF(values), #__VA_ARGS__); } }; \
	static LocalEnumParser& GetEnumParser() { static LocalEnumParser parser; return parser; } \
public: \
	static int getMemberCount() { return GetEnumParser().GetMemberList().getCount(); } \
	String toString() const { return GetEnumParser().toString(m_value); } \
	static enumName parse(const Char* str) { return GetEnumParser().parse(str); }; \
	static bool TryParse(const Char* str, enumName* outValue) { return GetEnumParser().TryParse(str, (outValue) ? (int*)&outValue->m_value : NULL); }
#endif

/**
	@brief	ビットフィールドとしてフラグの組み合わせを表す 拡張 enum 型を定義します。
	@see	Doc_EnumExtension_1
*/
#define LN_ENUM_FLAGS(enumName) \
	class enumName : public Enum \
	{ \
	public: \
enum _##enumName

/**
	@brief	ビットフィールドとしてフラグの組み合わせを表す 拡張 enum 型を定義します。
	@see	Doc_EnumExtension_1
*/
#define LN_ENUM_FLAGS_DECLARE(enumName) \
	protected: \
		int	m_value; \
	public: \
		typedef _##enumName enum_type; \
		typedef _##enumName value_type; \
		enumName() { m_value = 0; } \
		enumName(enum_type v) { m_value = v; } \
		virtual void setValue(EnumValueType value) override { m_value = (enum_type)value; } \
		virtual EnumValueType getValue() const override { return m_value; } \
		inline bool TestFlag(enum_type f) const throw() { return (m_value & f) == f && (f != 0 || m_value == f); } \
        inline explicit operator int() const { return m_value; } \
		inline bool operator==(enumName right) const { return m_value == right.m_value; } \
		inline bool operator==(_##enumName right) const { return m_value == right; } \
		inline bool operator!=(enumName right) const { return !operator==(right); } \
		inline bool operator!=(_##enumName right) const { return !operator==(right); } \
		inline enumName& operator &= (const enumName& v) { m_value &= v.m_value; return *this; } \
		inline enumName& operator &= (enum_type v) { m_value &= v; return *this; } \
		inline enumName& operator |= (const enumName& v) { m_value |= v.m_value; return *this; } \
		inline enumName& operator |= (enum_type v) { m_value |= v; return *this; } \
		inline enumName operator~() const throw() { enumName value; value.m_value = ~m_value; return value; } \
		friend inline bool operator==(const enum_type& left, const enumName& right) throw(); \
		friend inline bool operator!=(const _##enumName& left, const enumName& right) throw(); \
		friend inline enumName operator&(const enumName& left, const enumName& right) throw(); \
		friend inline enumName operator&(const enumName& left, enum_type right) throw(); \
		friend inline enumName operator&(enum_type left, const enumName& right) throw(); \
		friend inline enumName operator&(enum_type left, enum_type right) throw(); \
		friend inline enumName operator|(const enumName& left, const enumName& right) throw(); \
		friend inline enumName operator|(const enumName& left, enum_type right) throw(); \
		friend inline enumName operator|(enum_type left, const enumName& right) throw(); \
		friend inline enumName operator|(enum_type left, enum_type right) throw(); \
		friend inline enumName operator~(enumName::enum_type v) throw(); \
	}; \
	inline bool operator==(const enumName::enum_type& left, const enumName& right) throw()		{ return left == right.m_value; } \
	inline bool operator!=(const enumName::enum_type& left, const enumName& right) throw()		{ return left != right.m_value; } \
	inline enumName operator&(const enumName& left, const enumName& right) throw()		{ enumName value; value.m_value = left.m_value & right.m_value; return value; } \
	inline enumName operator&(const enumName& left, enumName::enum_type right) throw()	{ enumName value; value.m_value = left.m_value & right; return value; } \
	inline enumName operator&(enumName::enum_type left, const enumName& right) throw()	{ enumName value; value.m_value = right.m_value & left;  return value; } \
	inline enumName operator&(enumName::enum_type left, enumName::enum_type right) throw() { return enumName(left) & right; } \
	inline enumName operator|(const enumName& left, const enumName& right) throw()		{ enumName value; value.m_value = left.m_value | right.m_value; return value; } \
	inline enumName operator|(const enumName& left, enumName::enum_type right) throw()	{ enumName value; value.m_value = left.m_value | right; return value; } \
	inline enumName operator|(enumName::enum_type left, const enumName& right) throw()	{ enumName value; value.m_value = right.m_value | left;  return value; } \
	inline enumName operator|(enumName::enum_type left, enumName::enum_type right) throw() { return enumName(left) | right; } \
	inline enumName operator~(enumName::enum_type v) throw() { enumName value; value.m_value = (~((int)v)); return value; }

/**
	@brief	拡張 enum 型の型情報にアクセスする機能を定義します。LN_ENUM_FLAGS で定義した enum に対して使用します。
	@see	Doc_EnumExtension_1
*/
#define LN_ENUM_FLAGS_REFLECTION(enumName, ...) \
private: \
	struct LocalEnumParser : public EnumFlagsParser <_##enumName> { LocalEnumParser() { _##enumName values[] = { __VA_ARGS__ };  init(values, LN_ARRAY_SIZE_OF(values), #__VA_ARGS__); } }; \
	static LocalEnumParser& GetEnumParser() { static LocalEnumParser parser; return parser; } \
public: \
	String toString(const Char* separator = _TT("|")) const { return GetEnumParser().toString(m_value, separator); } \
	static enumName parse(const Char* str, TCHAR separator = '|') { return GetEnumParser().parse(str, separator); }; \
	static bool TryParse(const Char* str, enumName* outValue, TCHAR separator = '|') { return GetEnumParser().TryParse(str, (outValue) ? &outValue->m_value : NULL, separator); }


LN_NAMESPACE_BEGIN

typedef int EnumValueType;
	
/**
	@brief	拡張 enum 型のベースクラスです。
*/
class Enum
{
protected:
	//int	m_value;

public:
	//inline operator int() const { return m_value; }

	virtual void setValue(EnumValueType value) {}
	virtual EnumValueType getValue() const { return 0; }

	// C++ operators
	// https://en.wikipedia.org/wiki/Operators_in_C_and_C++

	template<typename TEnum>
	struct EnumParser
	{
	// LN_ENUM_DECLARE マクロが非常に長くなるのを避けるため、部分的にクラス化した

		struct Pair { String name; TEnum value; };
		typedef typename ::ln::List<Pair> PairList;
		typedef typename ::ln::List<Pair>& PairListReference;

		static PairListReference GetMemberList()
		{
			static PairList members; return members;	// ヘッダ include だけで済ますため、static 変数は関数内に閉じ込めておく
		}
		void init(const TEnum* values, int valuesCount, const char* argNames)
		{
			PairList& members = GetMemberList();
			String names = String::fromCString(argNames);
			List<String> tokens = names.split(_TT(","));
			for (int i = 0; i < valuesCount; ++i)
			{
				Pair p;
				p.name = tokens[i].trim();
				p.value = values[i];
				members.add(p);
			}
		}
		static String toString(int value)
		{
			PairList& members = GetMemberList();
			for (int i = 0; i < members.getCount(); ++i)
			{
				if (members[i].value == value) {
					return members[i].name;
				}
			}
			LN_ASSERT(0);
			return String();
		}
		static TEnum parse(const Char* str)
		{
			int value;
			if (TryParse(str, &value)) {
				return (TEnum)value;
			}
			LN_ENSURE(0);
			return TEnum();
		}
		static bool TryParse(const Char* str, int* outValue)
		{
			PairList& members = GetMemberList();
			for (int i = 0; i < members.getCount(); ++i)
			{
				if (members[i].name == str)
				{
					*outValue = members[i].value;
					return true;
				}
			}
			return false;
		}
	};

	template<typename TEnum>
	struct EnumFlagsParser : public EnumParser<TEnum>
	{
	public:
		typedef typename EnumParser<TEnum>::Pair Pair;
		typedef typename EnumParser<TEnum>::PairListReference PairListReference;

		static String toString(int value, const Char* separator)
		{
			PairListReference members = EnumParser<TEnum>::GetMemberList();
			// 先に完全一致を探す (White=Red|Green|Blue のようなパターン用)
			for (int i = 0; i < members.getCount(); ++i)
			{
				if (members[i].value == value) {
					return members[i].name;
				}
			}
			// 完全一致が無ければ複数のフラグを結合する
			String out;
			for (int i = 0; i < members.getCount(); ++i)
			{
				TEnum f = members[i].value;
				if ((value & f) == f && (f != 0 || value == f))
				{
					if (!out.isEmpty()) { out += separator; }
					out += members[i].name;
				}
			}
			LN_THROW(!out.isEmpty(), LogicException);
			return out;
		}
		static TEnum parse(const Char* str, Char separator)
		{
			int value;
			if (TryParse(str, &value, separator)) {
				return (TEnum)value;
			}
			LN_REQUIRE(0);
			return TEnum();
		}
		static bool TryParse(const Char* str, int* outValue, Char separator)
		{
			int state = 0;
			const Char* pos = str;
			const Char* tokenBegin = NULL;
			*outValue = 0;

			while (*pos)
			{
				switch (state)
				{
				case 0:	// 識別子を検索中
					if (isalnum(*pos)) {
						tokenBegin = pos;
						state = 1;		// 識別子の解析へ
					}
					else if (isspace(*pos)) {
						// 継続する
					}
					else {
						return false;	// 不正な文字が見つかった
					}
					break;

				case 1:	// 識別子を解析中
					if (isalnum(*pos)) {
						// 継続する
					}
					else {
						// 識別子確定
						int value;
						if (TryParseInternal(tokenBegin, (pos - tokenBegin), &value)) {
							(*outValue) |= value;
						}
						else {
							// メンバではない識別値が見つかった。とりあえず無視。
						}
						tokenBegin = NULL;
						state = 2;		// セパレータの検索へ
						--pos;			// この時点で | を指しているかもしれないので戻しておく
					}
					break;

				case 2:	// セパレータを検索中
					if (*pos == separator) {
						state = 0;		// 識別子の検索へ
					}
					else if (isspace(*pos)) {
						// 継続する
					}
					else {
						return false;	// 不正な文字が見つかった
					}
					break;
				}

				++pos;
			}

			// 終端が識別子で終わっていた場合はその分を読み取る
			if (tokenBegin != NULL)
			{
				int value;
				if (TryParseInternal(tokenBegin, (pos - tokenBegin), &value)) {
					(*outValue) |= value;
				}
				else {
					// メンバではない識別値が見つかった。とりあえず無視。
				}
			}

			return true;
		}

		static bool TryParseInternal(const Char* str, int len, int* outValue)
		{
			PairListReference members = EnumParser<TEnum>::GetMemberList();
			for (int i = 0; i < members.getCount(); ++i)
			{
				if (members[i].name == StringRef(str, len))
				{
					*outValue = members[i].value;
					return true;
				}
			}
			return false;
		}

	};
};
	
#else	// LN_DOXYGEN
#define LN_ENUM(enumName)		enum class enumName
#define LN_ENUM_DECLARE(enumName)
#define LN_ENUM_REFLECTION(enumName, ...)
#define LN_ENUM_FLAGS(enumName)	enum class enumName
#define LN_ENUM_FLAGS_DECLARE(enumName)
#define LN_ENUM_FLAGS_REFLECTION(enumName, ...)
#endif	// LN_DOXYGEN



/**
	@brief	scoped enum で簡単なビット演算をサポートするユーティリティです。
*/
template<typename TEnum, typename TStorage = uint32_t>
class EnumFlags
{
public:
	EnumFlags()
		: m_value(0)
	{}
					
	EnumFlags(TEnum value)
	{
		m_value = static_cast<TStorage>(value);
	}

	EnumFlags(const EnumFlags<TEnum, TStorage>& value)
	{
		m_value = value.m_value;
	}

	explicit EnumFlags(TStorage value)
	{
		m_value = value;
	}

	/** 指定したビットフィールドが設定されているかどうかを判断します。 */
	bool HasFlag(TEnum value) const
	{
		return (m_value & static_cast<TStorage>(value)) == static_cast<TStorage>(value);
	}

	/** 指定したビットフィールドを追加します。*/
	void SetFlag(TEnum value)
	{
		m_value = static_cast<TStorage>(value);
	}

	/** 指定したビットフィールドを除外します。*/
	void ClearFlag(TEnum value)
	{
		m_value &= ~static_cast<TStorage>(value);
	}

	bool operator==(TEnum rhs) const
	{
		return m_value == static_cast<TStorage>(rhs);
	}

	bool operator==(const EnumFlags<TEnum, TStorage>& rhs) const
	{
		return m_value == rhs.m_value;
	}

	bool operator==(bool rhs) const
	{
		return ((bool)*this) == rhs;
	}

	bool operator!=(TEnum rhs) const
	{
		return m_value != static_cast<TStorage>(rhs);
	}

	bool operator!=(const EnumFlags<TEnum, TStorage>& rhs) const
	{
		return m_value != rhs.m_value;
	}

	EnumFlags<TEnum, TStorage>& operator= (TEnum rhs)
	{
		m_value = static_cast<TStorage>(rhs);

		return *this;
	}

	EnumFlags<TEnum, TStorage>& operator= (const EnumFlags<TEnum, TStorage>& rhs)
	{
		m_value = rhs.m_value;

		return *this;
	}

	EnumFlags<TEnum, TStorage>& operator|= (TEnum rhs)
	{
		m_value |= static_cast<TStorage>(rhs);

		return *this;
	}

	EnumFlags<TEnum, TStorage>& operator|= (const EnumFlags<TEnum, TStorage>& rhs)
	{
		m_value |= rhs.m_value;

		return *this;
	}

	EnumFlags<TEnum, TStorage> operator| (TEnum rhs) const
	{
		EnumFlags<TEnum, TStorage> out(*this);
		out |= rhs;

		return out;
	}

	EnumFlags<TEnum, TStorage> operator| (const EnumFlags<TEnum, TStorage>& rhs) const
	{
		EnumFlags<TEnum, TStorage> out(*this);
		out |= rhs;

		return out;
	}

	EnumFlags<TEnum, TStorage>& operator&= (TEnum rhs)
	{
		m_value &= static_cast<TStorage>(rhs);

		return *this;
	}

	EnumFlags<TEnum, TStorage>& operator&= (const EnumFlags<TEnum, TStorage>& rhs)
	{
		m_value &= rhs.m_value;

		return *this;
	}

	EnumFlags<TEnum, TStorage> operator& (TEnum rhs) const
	{
		EnumFlags<TEnum, TStorage> out = *this;
		out.m_value &= static_cast<TStorage>(rhs);

		return out;
	}

	EnumFlags<TEnum, TStorage> operator& (const EnumFlags<TEnum, TStorage>& rhs) const
	{
		EnumFlags<TEnum, TStorage> out = *this;
		out.m_value &= rhs.m_value;

		return out;
	}

	EnumFlags<TEnum, TStorage>& operator^= (TEnum rhs)
	{
		m_value ^= static_cast<TStorage>(rhs);

		return *this;
	}

	EnumFlags<TEnum, TStorage>& operator^= (const EnumFlags<TEnum, TStorage>& rhs)
	{
		m_value ^= rhs.m_value;

		return *this;
	}

	EnumFlags<TEnum, TStorage> operator^ (TEnum rhs) const
	{
		EnumFlags<TEnum, TStorage> out = *this;
		out.m_value ^= static_cast<TStorage>(rhs);

		return out;
	}

	EnumFlags<TEnum, TStorage> operator^ (const EnumFlags<TEnum, TStorage>& rhs) const
	{
		EnumFlags<TEnum, TStorage> out = *this;
		out.m_value ^= rhs.m_value;

		return out;
	}

	EnumFlags<TEnum, TStorage> operator~ () const
	{
		EnumFlags<TEnum, TStorage> out;
		out.m_value = (TStorage)~m_value;

		return out;
	}

	operator bool() const
	{
		return m_value ? true : false;
	}

	explicit operator uint8_t() const
	{
		return static_cast<uint8_t>(m_value);
	}

	explicit operator uint16_t() const
	{
		return static_cast<uint16_t>(m_value);
	}

	explicit operator uint32_t() const
	{
		return static_cast<uint32_t>(m_value);
	}

	friend EnumFlags<TEnum, TStorage> operator&(TEnum a, EnumFlags<TEnum, TStorage> &b) LN_NOEXCEPT
	{
		EnumFlags<TEnum, TStorage> out;
		out.m_value = static_cast<TStorage>(a)& b.m_value;
		return out;
	}

	friend bool operator == (TEnum a, const EnumFlags<TEnum, TStorage>& b) LN_NOEXCEPT
	{
		return static_cast<TStorage>(a) == b.m_value;
	}

private:
	TStorage m_value;
};

/** EnumFlags クラスのための演算子を定義します。*/
#define LN_ENUM_FLAGS_OPERATORS(TEnum) \
	LN_ENUM_FLAGS_OPERATORS_EXT(TEnum, uint32_t)

/** EnumFlags クラスのための演算子を定義します。*/
#define LN_ENUM_FLAGS_OPERATORS_EXT(TEnum, TStorage) \
	inline EnumFlags<TEnum, TStorage> operator|(TEnum a, TEnum b) { EnumFlags<TEnum, TStorage> r(a); r |= b; return r; } \
	inline EnumFlags<TEnum, TStorage> operator&(TEnum a, TEnum b) { EnumFlags<TEnum, TStorage> r(a); r &= b; return r; } \
	inline EnumFlags<TEnum, TStorage> operator~(TEnum a) { return ~EnumFlags<TEnum, TStorage>(a); }

LN_NAMESPACE_END
