
#pragma once
#include "../Base/EnumExtension.h"
#include "../Base/List.h"
#include "../Base/String.h"

LN_NAMESPACE_BEGIN
namespace tr {
class JsonMember;

/** Json の値の型を示します。*/
LN_ENUM(JsonType)
{
	Null = 0,
	Bool,
	Double,
	String,
	Array,
	Object,
};
LN_ENUM_DECLARE(JsonType);

/**
	@brief	JSONの値を表します。
*/
class JsonValue
{
public:
	JsonValue();
	explicit JsonValue(bool value);
	JsonValue(double value);
	explicit JsonValue(const TCHAR* value);
	explicit JsonValue(const String& value);
	explicit JsonValue(const JsonValue& value);
	explicit JsonValue(JsonType::enum_type type);
	virtual ~JsonValue();
	JsonValue& operator=(const JsonValue& obj);

public:
	JsonType GetType()  const { return m_type; }
	bool IsNull()   const { return m_type == JsonType::Null; }
	bool IsBool()   const { return m_type == JsonType::Bool; }
	bool IsDouble() const { return m_type == JsonType::Double; }
	bool IsString() const { return m_type == JsonType::String; }
	bool IsArray()  const { return m_type == JsonType::Array; }
	bool IsObject() const { return m_type == JsonType::Object; }

	/*-----------------------------------------------------------------------*/
	/** @name Null */
	/** @{ */

	/** この値に Null を設定します。*/
	void SetNull();

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Bool */
	/** @{ */

	/** この値に Bool 値を設定します。*/
	void SetBool(bool value);

	/** この値の Bool 値を取得します。*/
	bool GetBool() const;

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Double */
	/** @{ */

	/** この値に Double 値を設定します。*/
	void SetDouble(double value);

	/** この値の Double 値を取得します。*/
	double GetDouble() const;

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name String */
	/** @{ */

	/** この値に文字列を設定します。*/
	void SetString(const String& str);

	/** この値の文字列を取得します。*/
	const String& GetString() const;

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Array */
	/** @{ */

	/** この値に空の配列を設定します。*/
	void SetArray();

	/** この配列に含まれている値の数を取得します。*/
	int GetItemCount() const;

	/** この配列の末尾に要素を追加します。*/
	void AddItem(const JsonValue& value);

	/** 指定したインデックスの要素を取得します。*/
	JsonValue& operator[](int index);
	const JsonValue& operator[](int index) const;		/**< @copydoc operator[] */

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Object */
	/** @{ */

	/** この値に空のオブジェクトを設定します。*/
	void SetObject();

	/** このオブジェクトに含まれているメンバの数を取得します。*/
	int GetMemberCount() const;

	/** このオブジェクトに新しいメンバを追加します。*/
	void AddMember(const String& name, const JsonValue& value);

	/** @} */

private:
	void Copy(const JsonValue& obj);
	void Detach();

	friend class JsonDOMHandler;
	void ResizeValueList(int count) { m_valueList->Resize(count); }
	JsonValue& GetAtValue(int index) { return m_valueList->GetAt(index); }
	void ResizeMemberList(int count) { m_memberList->Resize(count); }
	JsonMember& GetAtMember(int index) { return m_memberList->GetAt(index); }

private:
	typedef List<JsonValue>	ValueList;
	typedef List<JsonMember>	MemberList;

	JsonType	m_type;
	union
	{
		uint64_t	m_uint;
		bool		m_bool;
		double		m_double;
		String*		m_string;
		ValueList*	m_valueList;	// on array
		MemberList*	m_memberList;	// on object
	};
};

/**
	@brief	JSON の Object 型の値が持つメンバを表します。
*/
class JsonMember
{
public:
	String		Name;
	JsonValue	Value;
};

} // namespace tr
LN_NAMESPACE_END
