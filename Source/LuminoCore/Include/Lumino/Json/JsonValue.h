
#pragma once
#include "../Base/EnumFlags.h"
#include "../Base/List.h"
#include "../Base/String.h"

LN_NAMESPACE_BEGIN
namespace tr {
class JsonMember;

/** Json の値の型を示します。*/
enum class JsonType
{
	Null = 0,
	Bool,
	Double,
	String,
	Array,
	Object,
};

/**
	@brief	JSONの値を表します。
*/
class JsonValue
{
public:
	JsonValue();
	explicit JsonValue(bool value);
	JsonValue(double value);
	explicit JsonValue(const Char* value);
	explicit JsonValue(const String& value);
	explicit JsonValue(const JsonValue& value);
	explicit JsonValue(JsonType type);
	virtual ~JsonValue();
	JsonValue& operator=(const JsonValue& obj);

public:
	JsonType getType()  const { return m_type; }
	bool isNull()   const { return m_type == JsonType::Null; }
	bool IsBool()   const { return m_type == JsonType::Bool; }
	bool IsDouble() const { return m_type == JsonType::Double; }
	bool IsString() const { return m_type == JsonType::String; }
	bool IsArray()  const { return m_type == JsonType::Array; }
	bool IsObject() const { return m_type == JsonType::Object; }

	/*-----------------------------------------------------------------------*/
	/** @name Null */
	/** @{ */

	/** この値に Null を設定します。*/
	void setNull();

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Bool */
	/** @{ */

	/** この値に Bool 値を設定します。*/
	void setBool(bool value);

	/** この値の Bool 値を取得します。*/
	bool getBool() const;

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Double */
	/** @{ */

	/** この値に Double 値を設定します。*/
	void setDouble(double value);

	/** この値の Double 値を取得します。*/
	double getDouble() const;

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name String */
	/** @{ */

	/** この値に文字列を設定します。*/
	void setString(const String& str);

	/** この値の文字列を取得します。*/
	const String& getString() const;

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Array */
	/** @{ */

	/** この値に空の配列を設定します。*/
	void setArray();

	/** この配列に含まれている値の数を取得します。*/
	int getItemCount() const;

	/** この配列の末尾に要素を追加します。*/
	void addItem(const JsonValue& value);

	/** 指定したインデックスの要素を取得します。*/
	JsonValue& operator[](int index);
	const JsonValue& operator[](int index) const;		/**< @copydoc operator[] */

	/** @} */
	/*-----------------------------------------------------------------------*/
	/** @name Object */
	/** @{ */

	/** この値に空のオブジェクトを設定します。*/
	void setObject();

	/** このオブジェクトに含まれているメンバの数を取得します。*/
	int getMemberCount() const;

	/** このオブジェクトに新しいメンバを追加します。*/
	void addMember(const String& name, const JsonValue& value);

	/** @} */

private:
	void copy(const JsonValue& obj);
	void detach();

	friend class JsonDOMHandler;
	void resizeValueList(int count) { m_valueList->resize(count); }
	JsonValue& getAtValue(int index) { return m_valueList->getAt(index); }
	void resizeMemberList(int count) { m_memberList->resize(count); }
	JsonMember& getAtMember(int index) { return m_memberList->getAt(index); }

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
