
#pragma once
#include "Common.h"
#include "../Base/RefObject.h"
#include "../Base/String.h"

LN_NAMESPACE_BEGIN
class Object;

namespace tr
{
class TypeInfo;
class PropertyInfo;
typedef uint32_t LocalValueHavingFlags;

namespace detail
{
	using ObjectFactory = Ref<Object>(*)();
}

	/**
	@brief		
*/
class TypeInfo
{
public:
	typedef LocalValueHavingFlags* (*HasLocalValueFlagsGetter)(Object* _this);
	typedef void (*BindingTypeInfoSetter)(void* data);
	typedef void* (*BindingTypeInfoGetter)();
	typedef uint8_t RevisionCount;
	static const int MaxProperties = sizeof(LocalValueHavingFlags) * 8;

	struct ConstructArg
	{
		virtual void doSet(TypeInfo* typeInfo) = 0;
	};
	struct ConstructArgHolder
	{
		ConstructArgHolder(ConstructArg& arg)
			: m_arg(arg)
		{}
		ConstructArg& m_arg;
	};

public:
	
	/**
		@brief	指定したオブジェクトの型情報を取得します。
	*/
	static TypeInfo* getTypeInfo(const Object* obj);

	/**
		@brief	型引数に指定したクラス型の型情報を取得します。
	*/
	template<class T>
	static TypeInfo* getTypeInfo();

	static TypeInfo* findTypeInfo(const StringRef& name);

public:
	TypeInfo(
		const Char* className,
		TypeInfo* baseClass,
		HasLocalValueFlagsGetter getter,
		BindingTypeInfoSetter bindingTypeInfoSetter,
		BindingTypeInfoGetter bindingTypeInfoGetter,
		//detail::ObjectFactory factory,
		std::initializer_list<ConstructArgHolder> args);

	virtual ~TypeInfo() = default;
	
	/**
		@brief	クラス名を取得します。
	*/
	const String& getName() const;

	void registerProperty(PropertyInfo* prop);
	PropertyInfo* findProperty(const String& name) const;
	PropertyInfo* findProperty(size_t memberOffset) const;

	//// childObjProp が継承できるプロパティをこの TypeInfo から探す。見つからなければ NULL を返す。
	//// childObj : childObjProp を持つオブジェクト
	//Property* FindInheritanceProperty(const Property* childObjProp, CoreObject* childObj) const;

	//void RegisterRoutedEvent(RoutedEvent* ev);
	//RoutedEvent* FindRoutedEvent(const String& name) const;	// TODO: いらないかも
	//void InvokeRoutedEvent(CoreObject* owner, const RoutedEvent* ev, RoutedEventArgs* e);

	///// RoutedEventHandler は、ユーザーが動的に追加できるハンドラよりも前に呼び出される。
	///// WPF では「静的ハンドラ」と呼ばれている。動的イベントに登録するのに比べ、メモリを使用しない。
	//void RegisterRoutedEventHandler(const RoutedEvent* ev, RoutedEventHandler* handler);
	//RoutedEventHandler* FindRoutedEventHandler(const RoutedEvent* ev) const;

	TypeInfo* getBaseClass() const { return m_baseClass; }

	///// ベースクラスも含めた全てのプロパティを列挙する
	//static void ForEachAllProperty(const TypeInfo* typeInfo, const std::function<void(Property*)>& callback);


	void setBindingTypeInfo(void* data);
	static void* getBindingTypeInfo(const Object* obj);

	bool operator == (const TypeInfo& info) const { return m_name == info.m_name; }
	bool operator < (const TypeInfo& info) const { return m_name < info.m_name; }

	intptr_t getInternalGroup() const { return m_internalGroup; }

	void initializeProperties(Object* obj);


	Ref<Object> createInstance();
	int getSerializeClassVersion() const { return m_serializeClassVersion; }

protected:
	void setInternalGroup(intptr_t group) { m_internalGroup = group; }

private:
	//typedef SortedArray<const RoutedEvent*, RoutedEventHandler*>	RoutedEventHandlerList;

	String						m_name;						// クラス名
	TypeInfo*					m_baseClass;				// 継承元クラスを示す TypeInfo
	std::vector<PropertyInfo*>	m_propertyList;
	BindingTypeInfoSetter		m_bindingTypeInfoSetter;
	BindingTypeInfoGetter		m_bindingTypeInfoGetter;
	detail::ObjectFactory		m_factory;

	int							m_serializeClassVersion;

	intptr_t					m_internalGroup;
};


} // namespace tr
LN_NAMESPACE_END
