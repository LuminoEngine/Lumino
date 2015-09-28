
#pragma once
#include <iterator>
#include <type_traits>
#include <functional>
#include <Lumino/Base/Collection.h>
#include <Lumino/Base/SortedArray.h>
#include <Lumino/Base/Size.h>
#include <Lumino/Base/Rect.h>
#include <Lumino/Base/Thickness.h>
#include <Lumino/Base/Delegate.h>
//#include "RoutedEvent.h"

namespace Lumino
{
class CoreObject;
class CoreList;
class Variant;
class Property;
class PropertyChangedEventArgs;
class RoutedEvent;
class RoutedEventHandler;
class RoutedEventArgs;
class ApplicationContext;
template<typename T> class TypedProperty;

enum VariantType
{
	VariantType_Unknown = 0,
	VariantType_Bool,
	VariantType_Int,
	VariantType_Float,
	VariantType_String,
	VariantType_List,
	VariantType_Object,

	VariantType_SizeF,
	VariantType_Rect,
	VariantType_ThicknessF,

	VariantType_Max,			///< (Terminator)
};

class TypeInfo
{
public:
	typedef uint32_t* (*HasLocalValueFlagsGetter)(CoreObject* _this);

	typedef uint8_t RevisionCount;
	static const int MaxProperties = 32;

public:
	TypeInfo();

	TypeInfo(const TCHAR* fullName, TypeInfo* baseClass, HasLocalValueFlagsGetter getter)
		: m_fullName(fullName)
		, m_baseClass(baseClass)
		, m_hasLocalValueFlagsGetter(getter)
	{ }

	void RegisterProperty(Property* prop);
	Property* FindProperty(const String& name) const;

	// childObjProp が継承できるプロパティをこの TypeInfo から探す。見つからなければ NULL を返す。
	// childObj : childObjProp を持つオブジェクト
	Property* FindInheritanceProperty(const Property* childObjProp, CoreObject* childObj) const;

	void RegisterRoutedEvent(RoutedEvent* ev);
	RoutedEvent* FindRoutedEvent(const String& name) const;	// TODO: いらないかも
	void InvokeRoutedEvent(CoreObject* owner, const RoutedEvent* ev, RoutedEventArgs* e);

	/// RoutedEventHandler は、ユーザーが動的に追加できるハンドラよりも前に呼び出される。
	/// WPF では「静的ハンドラ」と呼ばれている。動的イベントに登録するのに比べ、メモリを使用しない。
	void RegisterRoutedEventHandler(const RoutedEvent* ev, RoutedEventHandler* handler);
	RoutedEventHandler* FindRoutedEventHandler(const RoutedEvent* ev) const;

	TypeInfo* GetBaseClass() const { return m_baseClass; }
	uint32_t* GetHasLocalValueFlags(CoreObject* obj) { return m_hasLocalValueFlagsGetter(obj); }

	/// ベースクラスも含めた全てのプロパティを列挙する
	static void ForEachAllProperty(const TypeInfo* typeInfo, const std::function<void(Property*)>& callback);


	bool operator == (const TypeInfo& info) const { return m_fullName == info.m_fullName; }
	bool operator < (const TypeInfo& info) const { return m_fullName < info.m_fullName; }

protected:

private:
	typedef SortedArray<const RoutedEvent*, RoutedEventHandler*>	RoutedEventHandlerList;

	String					m_fullName;		///< 完全修飾名
	TypeInfo*				m_baseClass;
	Array<Property*>		m_propertyList;
	Array<RoutedEvent*>		m_routedEventList;
	RoutedEventHandlerList	m_routedEventHandlerList;
	HasLocalValueFlagsGetter	m_hasLocalValueFlagsGetter;
};



#if 0

template<class TDerived, class TChildrenIterator, class TValue>
class hierarchical_iterator_adaptor
	: public std::iterator<std::input_iterator_tag, TValue>
{
public:
	hierarchical_iterator_adaptor() : m_internalItr() {}
	hierarchical_iterator_adaptor(const iterator& obj) : m_internalItr(obj.m_internalItr) {}
	iterator& operator = (const iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }

	bool operator==(const const_iterator& right) const	{ return m_internalItr == right.m_internalItr; }
	bool operator!=(const const_iterator& right) const	{ return m_internalItr != right.m_internalItr; }

	reference operator*() const		{ return *m_internalItr; }
	pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }

	const_iterator& operator++()	{ ++m_internalItr; return (*this); }
	const_iterator operator++(int)	{ const_iterator tmp = *this; ++(*this); return tmp; }

private:
	void increment()
	{

	}

private:
	struct Range
	{
		TChildrenIterator	m_itr;
		TChildrenIterator	m_end;
	};
	Stack<Range>	m_stack;

};

class CoreObjectHierarchicalIterator
	: public hierarchical_iterator_adaptor<CoreObjectHierarchicalIterator, CoreObjectCollection::const_iterator, CoreObject*>
{
public:
	typedef typename CoreObjectCollection::const_iterator TChildrenIterator;

public:
	CoreObjectHierarchicalIterator(CoreObject* obj, TChildrenIterator itr)
	{}
};
#endif

class EventArgs;

/**
	@brief		
*/
class EventSlotBase
{
protected:
	EventSlotBase() {}
	virtual ~EventSlotBase() {}

private:
	friend class CoreObject;
	virtual void Raise(EventArgs* e) = 0;
};


/**
	@brief		
	@details	RoutedEvent は UIElement 及びそのサブクラス内部からのみ発生させることが出来ます。
*/
template<class TArgs>
class EventSlot
	: public EventSlotBase
{
public:
	EventSlot() {}
	virtual ~EventSlot() {}

public:

	/**
		@brief	ルーティングイベントのハンドラを追加します。
	*/
	void AddHandler(const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Add(handler);
	}
	
	/**
		@brief	指定したハンドラに一致するハンドラを、このスロットから削除します。
	*/
	void RemoveHandler(const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Remove(handler);
	}
	
	/**
		@brief	ルーティングイベントのハンドラを追加します。
	*/
	void operator += (const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Add(handler);
	}
	
	/**
		@brief	指定したハンドラに一致するハンドラを、このスロットから削除します。
	*/
	void operator -= (const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Remove(handler);
	}

private:
	Array< Delegate01<TArgs*> > m_handlerList;

	virtual void Raise(EventArgs* e)
	{
		for (Delegate01<TArgs*>& d : m_handlerList)
		{
			d.Call(static_cast<TArgs*>(e));
		}
	}

	// Lumino::EventXX のような使い方と同時に、
	// Property にたいする メンバ変数のような意味も持つ。
	// 即ち、ハンドラのリスト。
	// RoutedEventSlot は、いわゆる「動的なハンドラ」
};


class CoreObjectCollection : public Collection < CoreObject* >
{
public:
	typedef Collection<CoreObject*>::value_type value_type;

public:
	CoreObjectCollection(CoreObject* owner);
	virtual ~CoreObjectCollection() {}

protected:
	virtual void InsertItem(int index, const value_type& item);
	virtual void ClearItems();
	virtual void RemoveItem(int index);
	virtual void SetItem(int index, const value_type& item);

private:
	CoreObject*	m_owner;
};

/**
	@brief		
*/
class CoreObject
	: public RefObject
{
public:
	static const int IsArrayObject = 0;

public:
	CoreObject();
	virtual ~CoreObject();

public:

	/**
		@brief		プロパティの値を設定します。
	*/
	// TODO: TypeInfo に移動したい。インテリセンスの汚れが結構気になる。
	void SetPropertyValue(const Property* prop, const Variant& value);
	
	/**
		@brief		プロパティの値を取得します。
	*/
	Variant GetPropertyValue(const Property* prop) const;

	/**
		@brief		プロパティの値を設定します。あらかじめ型が分かっている場合、SetPropertyValue() よりも少ないオーバーヘッドで設定できます。
	*/
	template<typename TValue>
	void SetTypedPropertyValue(const Property* prop, const TValue& value);

	/**
		@brief		プロパティの値を取得します。あらかじめ型が分かっている場合、GetPropertyValue() よりも少ないオーバーヘッドで設定できます。
	*/
	template<typename TValue>
	TValue GetTypedPropertyValue(const Property* prop) const;


	String ToString();

	virtual void SetUserData(void* data) { m_userData = data; }

	virtual void* GetUserData() const { return m_userData; }

	/// 各種言語バインダから設定される型情報 ID を取得します。
	virtual void* GetBindingTypeData() const { return NULL; }

	/**
		@brief	このオブジェクトに指定したプロパティのローカル値が設定されているかを確認します。
	*/
	bool HasLocalPropertyValue(const Property* prop);



	// internal
	void NotifyPropertyChange(const Property* prop, const Variant& newValue, const Variant& oldValue/*PropertyChangedEventArgs* e*/);
	void SetInheritanceParent(CoreObject* obj);
	CoreObject* GetInheritanceParent() const { return m_inheritanceParent; }
	CoreObjectCollection& GetInheritanceChildren() { return m_inheritanceChildren; }
	static void ForEachInheritanceChildrenHierarchical(CoreObject* parent, const std::function<void(CoreObject*)>& callback);

	//CoreObjectHierarchicalIterator beginHierarchical() { return CoreObjectHierarchicalIterator(this, m_inheritanceChildren.begin()); }
	//CoreObjectHierarchicalIterator endHierarchical() { return CoreObjectHierarchicalIterator(this, m_inheritanceChildren.end()); }

protected:
	void RaiseEvent(EventSlotBase& eventSlot, EventArgs* e);	// TODO: UIElement の関数名と被っている

	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	virtual void RaiseEventInternal(const RoutedEvent* ev, RoutedEventArgs* e);

	virtual void OnPropertyChanged(PropertyChangedEventArgs* e);
	//bool HasLocalValueInternal(const Property* prop);

	class LocalValueFlags
	{
	public:
		uint32_t	Flags;
		LocalValueFlags()
			: Flags(0)
		{}
	};

private:
	friend TypeInfo* GetTypeInfo(CoreObject* obj);
	//virtual TypeInfo* GetThisTypeInfo() const { return NULL; };	// TODO: 純粋仮想関数にしてマクロ定義を強制する
	static uint32_t* GetHasLocalValueFlags(CoreObject* _this) { return NULL; }

	//Property* FindInheritanceProperty();
	static void UpdateInheritanceProperty(CoreObject* obj, const Property* prop);

	void SetPropertyValueInternal(const Property* prop, const Variant& value, bool reset);

private:
	typedef SortedArray<String, Property*>	PropertyList;
	typedef SortedArray<const Property*, Variant>	PropertyDataStore;

	CoreObject*			m_inheritanceParent;
	CoreObjectCollection	m_inheritanceChildren;

	void*				m_userData;
	//PropertyList		m_propertyList;
	PropertyDataStore*	m_propertyDataStore;

	//typedef SortedArray<const RoutedEvent*, Property*>	RoutedEventList;

private:
	static TypeInfo m_typeInfo;
	virtual TypeInfo* GetThisTypeInfo() const;
public:
	static TypeInfo* GetClassTypeInfo();
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
template<typename TValue>
void CoreObject::SetTypedPropertyValue(const Property* prop, const TValue& value)
{
	LN_THROW(prop != NULL, ArgumentException);
	auto t = static_cast<const TypedProperty<TValue>*>(prop);
	t->SetValueDirect(this, value);
	PropertyInstanceData* data = prop->GetPropertyInstanceData(this);
	if (data != NULL)
	{
		if (data->IsDefault == true)
		{
			// 新しく設定される瞬間、これまで継承元として参照していたプロパティと this に対して
			// プロパティ参照更新値を1つ進める。子は Get しようとしたとき、継承元を再検索する。
			if (data->InheritanceParent != NULL) {
				data->InheritanceTarget->GetPropertyInstanceData(data->InheritanceParent)->PathRevisionCount++;
			}
			data->PathRevisionCount++;
		}
		data->IsDefault = false;
		data->RevisionCount++;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
template<typename TValue>
TValue CoreObject::GetTypedPropertyValue(const Property* prop) const
{
	LN_THROW(prop != NULL, ArgumentException);

	UpdateInheritanceProperty(const_cast<CoreObject*>(this), prop);

	auto t = static_cast<const TypedProperty<TValue>*>(prop);
	return t->GetValueDirect(this);
}


#define LN_CORE_OBJECT_TYPE_INFO_DECL() \
	private: \
		friend class ApplicationContext; \
		static TypeInfo m_typeInfo; \
		static void* m_coreObjectBindingTypeInfo; \
		virtual TypeInfo* GetThisTypeInfo() const; \
		LocalValueFlags m_hasLocalValueFlags; \
		static uint32_t* GetHasLocalValueFlags(CoreObject* _this); \
	public: \
		static TypeInfo* GetClassTypeInfo(); \
		virtual void* GetBindingTypeInfo() const { return m_coreObjectBindingTypeInfo; } \
		static void SetBindingTypeInfo(void* data) { m_coreObjectBindingTypeInfo = data; }

#define LN_CORE_OBJECT_TYPE_INFO_IMPL(subClassFullName, baseClass) \
	TypeInfo subClassFullName::m_typeInfo(_T(#subClassFullName), baseClass::GetClassTypeInfo(), &subClassFullName::GetHasLocalValueFlags); \
	void* subClassFullName::m_coreObjectBindingTypeInfo = NULL; \
	TypeInfo* subClassFullName::GetThisTypeInfo() const { return &m_typeInfo; } \
	TypeInfo* subClassFullName::GetClassTypeInfo() { return &m_typeInfo; } \
	uint32_t* subClassFullName::GetHasLocalValueFlags(CoreObject* _this) { return &static_cast<subClassFullName*>(_this)->m_hasLocalValueFlags.Flags; }



/// 指定されたオブジェクトの型情報を取得する
inline TypeInfo* GetTypeInfo(CoreObject* obj)
{
	return obj->GetThisTypeInfo();
}

/**
	@brief		
*/
class Variant
{
public:
	static const Variant Null;

public:
	Variant();
	Variant(const Variant& value);
	Variant(bool value);
	Variant(int value);
	Variant(float value);
	Variant(const String& value);
	Variant(const Enum& value);
	Variant(CoreList* value);
	Variant(const SizeF& value);
	Variant(const Rect& value);
	Variant(const ThicknessF& value);
	~Variant();
	Variant& operator = (const Variant& obj) { Copy(obj); return (*this); }

	Variant(CoreObject* obj);

	template<class T>
	Variant(RefPtr<T>& obj);

public:
	VariantType GetType() const { return m_type; }
	bool GetBool() const;

	void SetInt(int value);
	int GetInt() const;

	void SetFloat(float value);
	float GetFloat() const;

	void SetString(const String& value);
	String GetString() const;

	void SetList(CoreList* value);
	CoreList* GetList() const;

	void Set(CoreObject* obj);
	CoreObject* GetObject() const;

	void SetSizeF(const SizeF& value);
	const SizeF& GetSizeF() const;

	void SetRect(const Rect& value);
	const Rect& GetRect() const;

	void SetThicknessF(const ThicknessF& value);
	const ThicknessF& GetThicknessF() const;

	template<typename T>
	T& RefCast() const { return static_cast<T&>(GetObject()); }


public:
	
	/**
		@brief		指定した Variant の値を指定した型にキャストする。
		@code
					UIElement* item = Variant::Cast<UIElement*>(value);
		@endcode
	*/
	template<typename T>
	static T Cast(const Variant& value) { return CastSelector<T, typename std::is_base_of<Enum, T>::type, typename std::is_base_of<RefPtrCore, T>::type >::GetValue(value); }

private:

	template<typename T, typename TIsEnum, typename TIsRefPtr> struct CastSelector { static T GetValue(const Variant& v) { return static_cast<T>(v.GetObject()); } };
	//


#if 0
	template<typename T, typename TIsEnum> struct CastSelector { static T GetValue(const Variant& v) { return static_cast<T>(v.GetObject()); } };
	//template<typename TRefPtr, typename TIsEnum> struct CastSelector { static TRefPtr GetValue(const Variant& v) { return TRefPtr(static_cast<TRefPtr::PtrType>(v.GetObject())); } };

	template<> struct CastSelector < bool, std::false_type >		{ static bool GetValue(const Variant& v) { return v.GetBool(); } };
	template<> struct CastSelector < int, std::false_type >		{ static int GetValue(const Variant& v) { return v.GetInt(); } };
	template<> struct CastSelector < float, std::false_type >		{ static float GetValue(const Variant& v) { return v.GetFloat(); } };
	template<> struct CastSelector < String, std::false_type >		{ static String GetValue(const Variant& v) { return v.GetString(); } };
	template<> struct CastSelector < Rect, std::false_type >		{ static Rect GetValue(const Variant& v) { return v.GetRect(); } };
	template<> struct CastSelector < SizeF, std::false_type >		{ static SizeF GetValue(const Variant& v) { return v.GetSizeF(); } };
	template<> struct CastSelector < ThicknessF, std::false_type > { static const ThicknessF& GetValue(const Variant& v) { return v.GetThicknessF(); } };
	template<typename T> struct CastSelector < T, std::true_type > { static T GetValue(const Variant& v) { return *((T*)(&v.m_enum)); } };	// TODO: 型チェック
//	template<typename TRefPtr, typename U> struct CastSelector < TRefPtr<U>, std::true_type > { static TRefPtr<U> GetValue(const Variant& v) { return TRefPtr<U>(static_cast<U>(v.GetObject())); } };	// TODO: 型チェック
#endif

public:
	bool operator == (const Variant& right) const;

private:
	void Copy(const Variant& obj);
	void Release();

private:
	VariantType	m_type;
	union
	{
		uint64_t		m_uint;
		bool			m_bool;
		int				m_int;
		float			m_float;
		/*String*			m_string;*/
		CoreList*		m_valueList;
		CoreObject*		m_object;
		float			m_sizeF[2];
		int				m_rect[4];
		float			m_thicknessF[4];
		Enum			m_enum;
	};
	String			m_string;
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
template<class T>
Variant::Variant(RefPtr<T>& obj)
	: m_type(VariantType_Unknown)
	, m_uint(0)
{
	if (T::IsArrayObject == 0)
		Set(obj);
	else
	{
		CoreObject* t = obj.GetObjectPtr();
		SetList(static_cast<CoreList*>(t));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
template<> struct Variant::CastSelector < bool, std::false_type, std::false_type >			{ static bool GetValue(const Variant& v) { return v.GetBool(); } };
template<> struct Variant::CastSelector < int, std::false_type, std::false_type >			{ static int GetValue(const Variant& v) { return v.GetInt(); } };
template<> struct Variant::CastSelector < float, std::false_type, std::false_type >			{ static float GetValue(const Variant& v) { return v.GetFloat(); } };
template<> struct Variant::CastSelector < String, std::false_type, std::false_type >		{ static String GetValue(const Variant& v) { return v.GetString(); } };
template<> struct Variant::CastSelector < Rect, std::false_type, std::false_type >			{ static Rect GetValue(const Variant& v) { return v.GetRect(); } };
template<> struct Variant::CastSelector < SizeF, std::false_type, std::false_type >			{ static SizeF GetValue(const Variant& v) { return v.GetSizeF(); } };
template<> struct Variant::CastSelector < ThicknessF, std::false_type, std::false_type >	{ static const ThicknessF& GetValue(const Variant& v) { return v.GetThicknessF(); } };
template<typename T> struct Variant::CastSelector < T, std::true_type, std::false_type >	{ static T GetValue(const Variant& v) { return *((T*)(&v.m_enum)); } };	// TODO: 型チェック
template<typename T> struct Variant::CastSelector  < T, std::false_type, std::true_type >	{ static T GetValue(const Variant& v) { return T(static_cast<typename T::PtrType>(v.GetObject())); } };

#if 0

class VariantList
	: public Collection<Variant>
	, public CoreObject
{
public:
	static const int IsArrayObject = 1;
};


#define LN_VARIANT_OBJECT_LIST_DECL(itemType) \
	void SetAt(int index, itemType* item); \
	itemType* GetAt(int index) const;

#define LN__VARIANT_OBJECT_LIST_IMPL(listType, itemType) \
	void listType::SetAt(int index, itemType* item) { RefObjectList::SetAtBase(index, item); } \
	itemType* listType::GetAt(int index) const { return static_cast<itemType*>(RefObjectList::GetAtBase(index)); }


template<typename T>
struct TestTraits
{
	typedef T& DirectReference;
};

template<typename T>
struct TestTraits<T*>
{
	typedef T* DirectReference;
};

/**
	@brief
*/
template<typename TValue>
class GenericVariantList
	: public VariantList
{
public:
	typedef TValue value_type;
	typedef ptrdiff_t difference_type;
	typedef ptrdiff_t distance_type;	// retained
	//typedef typename std::vector<TValue>::pointer pointer;
	//typedef typename std::vector<TValue>::reference reference;
	typedef TValue* pointer;
	typedef TValue& reference;


public:
	/// 指定インデックスに要素を格納する
	void SetAt(int index, TValue& item) { VariantList::SetAt(index, item); }

	/// 指定インデックスの要素を取得する
	TValue GetAt(int index) const { return Variant::Cast<TValue>(VariantList::GetAt(index)); }	// TODO: できれば参照で返したいが…

	/// 要素を末尾に追加する
	void Add(TValue& item) { VariantList::Add(item); }

	/// 指定したインデックスの位置に要素を挿入する
	void Insert(int index, TValue& item) { VariantList::Insert(index, item); }

public:
	class const_iterator //: public std::iterator<std::forward_iterator_tag, const TValue>
	{
	public:
		typedef TValue value_type;
		typedef ptrdiff_t difference_type;
		typedef ptrdiff_t distance_type;
		typedef TValue* pointer;
		typedef TValue& reference;
		typedef std::random_access_iterator_tag iterator_category;

		const_iterator() : m_internalItr() {}
		const_iterator(const const_iterator& obj) : m_internalItr(obj.m_internalItr) {}
		const_iterator& operator = (const const_iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		reference operator*() const		{ return *m_internalItr; }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		const_iterator& operator++()	{ ++m_internalItr; return (*this); }
		const_iterator operator++(int)	{ const_iterator tmp = *this; ++(*this); return tmp; }
		const_iterator& operator--()	{ --m_internalItr; return (*this); }
		const_iterator operator--(int)	{ const_iterator tmp = *this; --(*this); return tmp; }
		//const_iterator& operator+=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator+(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//const_iterator& operator-=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator-(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//difference_type operator-(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return 0; }

		reference operator[](difference_type offset) const	{ return m_internalItr[offset]; }
		bool operator==(const const_iterator& right) const	{ return m_internalItr == right.m_internalItr; }
		bool operator!=(const const_iterator& right) const	{ return m_internalItr != right.m_internalItr; }
		bool operator<(const const_iterator& right) const	{ return m_internalItr < right.m_internalItr; }
		//bool operator>(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		//bool operator<=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		//bool operator>=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class GenericVariantList;
		const_iterator(const VariantList::const_iterator& itr) : m_internalItr(itr) {}
		VariantList::const_iterator m_internalItr;
	};

	class iterator// : public std::iterator<std::forward_iterator_tag, TValue>
	{
	public:
		typedef TValue value_type;
		typedef ptrdiff_t difference_type;
		typedef ptrdiff_t distance_type;
		typedef TValue* pointer;
		typedef TValue& reference;
		typedef std::random_access_iterator_tag iterator_category;

		iterator() : m_internalItr() {}
		iterator(const iterator& obj) : m_internalItr(obj.m_internalItr) {}
		iterator& operator = (const iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		typename TestTraits<TValue>::DirectReference operator*() const		{ return Variant::Cast<TValue>(*m_internalItr); }//{ return *static_cast<TValue**>(&(m_internalItr->Cast<TValue>())); }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		iterator& operator++()			{ ++m_internalItr; return (*this); }
		iterator operator++(int)		{ iterator tmp = *this; ++(*this); return tmp; }
		iterator& operator--()			{ --m_internalItr; return (*this); }
		iterator operator--(int)		{ iterator tmp = *this; --(*this); return tmp; }
		//const_iterator& operator+=(difference_type offset)			{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator+(difference_type offset) const		{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//const_iterator& operator-=(difference_type offset)			{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator-(difference_type offset) const		{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//difference_type operator-(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return 0; }

		reference operator[](difference_type offset) const	{ return static_cast<reference>(m_internalItr[offset]); }
		bool operator==(const iterator& right) const		{ return m_internalItr == right.m_internalItr; }
		bool operator!=(const iterator& right) const		{ return m_internalItr != right.m_internalItr; }
		bool operator<(const iterator& right) const			{ return m_internalItr < right.m_internalItr; }
		//bool operator>(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		//bool operator<=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }
		//bool operator>=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class GenericVariantList;
		iterator(const VariantList::iterator& itr) : m_internalItr(itr) {}
		VariantList::iterator m_internalItr;
	};

protected:
	// TODO: InsertItem とかオーバーライドして型チェックしたい

public:
	GenericVariantList() {}
	virtual ~GenericVariantList() {}

	iterator		begin()			{ return iterator(VariantList::begin()); }
	const_iterator	begin() const	{ return const_iterator(VariantList::begin()); }
	iterator		end()			{ return iterator(VariantList::end()); }
	const_iterator	end() const		{ return const_iterator(VariantList::end()); }
};
#endif

#if 0

/**
	@brief
*/
class VariantList
	: public CoreObject
{
public:
	static const int IsArrayObject = 1;

public:
	VariantList() {}
	virtual ~VariantList() { Clear(); }

public:

	bool IsOutOfRange(int index) const { return m_list.IsOutOfRange(index); }
	bool IsEmpty() const { return m_list.IsEmpty(); }

	/// 要素数を取得する
	int GetCount() const { return m_list.GetCount(); }

	/// 指定インデックスに要素を格納する
	void SetAtVariant(int index, const Variant& item)
	{
		//if (OnItemAdding(item))
		{
			m_list.SetAt(index, item);
			OnItemAdded(item);
		}
	}

	/// 指定インデックスの要素を取得する
	const Variant& GetAtVariant(int index) const
	{
		return m_list.GetAt(index);
	}

	/// 要素を末尾に追加する
	void AddVariant(const Variant& item)
	{
		//if (OnItemAdding(item))
		{
			m_list.Add(item);
			OnItemAdded(item);
		}
	}

	/// 全ての要素を削除する
	void Clear()
	{
		for (Variant& item : m_list) {
			OnItemRemoved(item);	// TODO: erase しながらひとつずつ呼ぶべきかも
		}
		m_list.Clear();
	}

	/// 指定したインデックスの位置に要素を挿入する
	void InsertVariant(int index, const Variant& item)
	{
		if (OnItemAdding(item))
		{
			m_list.Insert(index, item);
			OnItemAdded(item);
		}
	}

	/// item と一致する最初の要素を削除する
	void RemoveVariant(const Variant& item)
	{
		bool b = m_list.Remove(item);
		if (b) {
			OnItemRemoved(item);
		}
	}

	/// 指定したインデックスの要素を削除する
	void RemoveAt(int index)
	{
		Variant item = m_list.GetAt(index);
		m_list.RemoveAt(index);
		OnItemRemoved(item);
	}

public:
	class const_iterator : public std::iterator<std::forward_iterator_tag, const Variant>
	{
	public:
		const_iterator() : m_internalItr() {}
		const_iterator(const const_iterator& obj) : m_internalItr(obj.m_internalItr) {}
		const_iterator& operator = (const const_iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		reference operator*() const		{ return *m_internalItr; }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		const_iterator& operator++()	{ ++m_internalItr; return (*this); }
		const_iterator operator++(int)	{ const_iterator tmp = *this; ++(*this); return tmp; }
		const_iterator& operator--()	{ --m_internalItr; return (*this); }
		const_iterator operator--(int)	{ const_iterator tmp = *this; --(*this); return tmp; }
		//const_iterator& operator+=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator+(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//const_iterator& operator-=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator-(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//difference_type operator-(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return 0; }
		reference operator[](difference_type offset) const	{ return m_internalItr[offset]; }
		bool operator==(const const_iterator& right) const	{ return m_internalItr == right.m_internalItr; }
		bool operator!=(const const_iterator& right) const	{ return m_internalItr != right.m_internalItr; }
		bool operator<(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class VariantList;
		const_iterator(const Array<Variant>::const_iterator& itr) : m_internalItr(itr) {}
		Array<Variant>::const_iterator m_internalItr;
	};

	class iterator : public std::iterator<std::forward_iterator_tag, Variant>
	{
	public:
		iterator() : m_internalItr() {}
		iterator(const iterator& obj) : m_internalItr(obj.m_internalItr) {}
		iterator& operator = (const iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		reference operator*() const		{ return static_cast<reference>(*m_internalItr); }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		iterator& operator++()			{ ++m_internalItr; return (*this); }
		iterator operator++(int)		{ iterator tmp = *this; ++(*this); return tmp; }
		iterator& operator--()			{ --m_internalItr; return (*this); }
		iterator operator--(int)		{ iterator tmp = *this; --(*this); return tmp; }
		//const_iterator& operator+=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator+(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//const_iterator& operator-=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator-(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//difference_type operator-(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return 0; }
		reference operator[](difference_type offset) const	{ return static_cast<reference>(m_internalItr[offset]); }
		bool operator==(const iterator& right) const		{ return m_internalItr == right.m_internalItr; }
		bool operator!=(const iterator& right) const		{ return m_internalItr != right.m_internalItr; }
		bool operator<(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class VariantList;
		iterator(const Array<Variant>::iterator& itr) : m_internalItr(itr) {}	// begin() と end() で使用する
		Array<Variant>::iterator m_internalItr;
	};

	iterator		begin()			{ return iterator(m_list.begin()); }
	const_iterator	begin() const	{ return const_iterator(m_list.begin()); }
	iterator		end()			{ return iterator(m_list.end()); }
	const_iterator	end() const		{ return const_iterator(m_list.end()); }

protected:
	//virtual bool OnItemAdding(const Variant& item) { return true; }
	virtual void OnItemAdded(const Variant& item) {}
	virtual void OnItemRemoved(const Variant& item) {}

protected:
	Array<Variant>	m_list;
};

#define LN_VARIANT_OBJECT_LIST_DECL(itemType) \
	void SetAt(int index, itemType* item); \
	itemType* GetAt(int index) const;

#define LN__VARIANT_OBJECT_LIST_IMPL(listType, itemType) \
	void listType::SetAt(int index, itemType* item) { RefObjectList::SetAtBase(index, item); } \
	itemType* listType::GetAt(int index) const { return static_cast<itemType*>(RefObjectList::GetAtBase(index)); }


template<typename T>
struct TestTraits
{
	typedef T& DirectReference;
};

template<typename T>
struct TestTraits<T*>
{
	typedef T* DirectReference;
};

/**
	@brief
*/
template<typename TValue>
class GenericVariantList
	: public VariantList
{
public:
	typedef TValue value_type;
	typedef ptrdiff_t difference_type;
	typedef ptrdiff_t distance_type;	// retained
	//typedef typename std::vector<TValue>::pointer pointer;
	//typedef typename std::vector<TValue>::reference reference;
	typedef TValue* pointer;
	typedef TValue& reference;


public:
	/// 指定インデックスに要素を格納する
	void SetAt(int index, const TValue& item) { SetAtVariant(index, item); }

	/// 指定インデックスの要素を取得する
	TValue GetAt(int index) const { return Variant::Cast<TValue>(GetAtVariant(index)); }	// TODO: できれば参照で返したいが…

	/// 要素を末尾に追加する
	void Add(const TValue& item) { AddVariant(item); }

	/// 指定したインデックスの位置に要素を挿入する
	void Insert(int index, const TValue& item) { InsertVariant(index, item); }

public:
	class const_iterator //: public std::iterator<std::forward_iterator_tag, const TValue>
	{
	public:
		typedef TValue value_type;
		typedef ptrdiff_t difference_type;
		typedef ptrdiff_t distance_type;
		typedef TValue* pointer;
		typedef TValue& reference;
		typedef std::random_access_iterator_tag iterator_category;

		const_iterator() : m_internalItr() {}
		const_iterator(const const_iterator& obj) : m_internalItr(obj.m_internalItr) {}
		const_iterator& operator = (const const_iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		reference operator*() const		{ return *m_internalItr; }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		const_iterator& operator++()	{ ++m_internalItr; return (*this); }
		const_iterator operator++(int)	{ const_iterator tmp = *this; ++(*this); return tmp; }
		const_iterator& operator--()	{ --m_internalItr; return (*this); }
		const_iterator operator--(int)	{ const_iterator tmp = *this; --(*this); return tmp; }
		//const_iterator& operator+=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator+(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//const_iterator& operator-=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator-(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//difference_type operator-(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return 0; }

		reference operator[](difference_type offset) const	{ return m_internalItr[offset]; }
		bool operator==(const const_iterator& right) const	{ return m_internalItr == right.m_internalItr; }
		bool operator!=(const const_iterator& right) const	{ return m_internalItr != right.m_internalItr; }
		bool operator<(const const_iterator& right) const	{ return m_internalItr < right.m_internalItr; }
		bool operator>(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class GenericVariantList;
		const_iterator(const Array<Variant>::const_iterator& itr) : m_internalItr(itr) {}
		Array<Variant>::const_iterator m_internalItr;
	};

	class iterator// : public std::iterator<std::forward_iterator_tag, TValue>
	{
	public:
		typedef TValue value_type;
		typedef ptrdiff_t difference_type;
		typedef ptrdiff_t distance_type;
		typedef TValue* pointer;
		typedef TValue& reference;
		typedef std::random_access_iterator_tag iterator_category;

		iterator() : m_internalItr() {}
		iterator(const iterator& obj) : m_internalItr(obj.m_internalItr) {}
		iterator& operator = (const iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		typename TestTraits<TValue>::DirectReference operator*() const		{ return Variant::Cast<TValue>(*m_internalItr); }//{ return *static_cast<TValue**>(&(m_internalItr->Cast<TValue>())); }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		iterator& operator++()			{ ++m_internalItr; return (*this); }
		iterator operator++(int)		{ iterator tmp = *this; ++(*this); return tmp; }
		iterator& operator--()			{ --m_internalItr; return (*this); }
		iterator operator--(int)		{ iterator tmp = *this; --(*this); return tmp; }
		//const_iterator& operator+=(difference_type offset)			{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator+(difference_type offset) const		{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//const_iterator& operator-=(difference_type offset)			{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator-(difference_type offset) const		{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//difference_type operator-(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return 0; }

		reference operator[](difference_type offset) const	{ return static_cast<reference>(m_internalItr[offset]); }
		bool operator==(const iterator& right) const		{ return m_internalItr == right.m_internalItr; }
		bool operator!=(const iterator& right) const		{ return m_internalItr != right.m_internalItr; }
		bool operator<(const iterator& right) const			{ return m_internalItr < right.m_internalItr; }
		bool operator>(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class GenericVariantList;
		iterator(const Array<Variant>::iterator& itr) : m_internalItr(itr) {}
		Array<Variant>::iterator m_internalItr;
	};

public:
	GenericVariantList() {}
	virtual ~GenericVariantList() {}

	iterator		begin()			{ return iterator(m_list.begin()); }
	const_iterator	begin() const	{ return const_iterator(m_list.begin()); }
	iterator		end()			{ return iterator(m_list.end()); }
	const_iterator	end() const		{ return const_iterator(m_list.end()); }
};

#endif

} // namespace Lumino
