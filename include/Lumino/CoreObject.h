
#pragma once
#include <Lumino/Base/Collection.h>
#include "Variant.h"	// TODO

LN_NAMESPACE_BEGIN
	
enum class GenericInstanceType
{
	Unknown = 0,
	Bool,
	Int,
	Float,
	String,
	Enum,
	Object,
	List,
	Struct,
};
	
/**
	@brief		
*/
class CoreList
	: public CoreObject
{
public:
	static const int IsArrayObject = 1;	// TODO

	virtual void AddVariant(const Variant& value) = 0;

	//virtual GenericInstanceType GetGenericInstanceType() const = 0;
};

/**
	@brief		
	@details	RefObject またはその派生クラスのポインタでインスタンス化した場合、
				このコンテナへの追加/削除にあわせて参照カウントを操作します。
*/
template<typename TValue>
class GenericCoreList
	: public CoreList
	, public Collection<TValue>
{
public:
	typedef TValue value_type;
	//typedef typename Collection<TValue>::difference_type;
	//typedef typename Collection<TValue>::distance_type;
	typedef typename Collection<TValue>::pointer pointer;
	typedef typename Collection<TValue>::const_pointer const_pointer;
	typedef typename Collection<TValue>::reference reference;
	typedef typename Collection<TValue>::const_reference const_reference;

	//template<typename T, typename TIsEnum, typename TIsObject, typename TIsList, typename TIsClass> struct TypeTraits
	//{
	//	static GenericInstanceType GetGenericInstanceType() { return GenericInstanceType::Unknown; }
	//};
	//template<> struct TypeTraits<bool, std::false_type, std::false_type, std::false_type, std::false_type>
	//{ 
	//	static GenericInstanceType GetGenericInstanceType() { return GenericInstanceType::Bool; }
	//};
	//template<> struct TypeTraits<int, std::false_type, std::false_type, std::false_type, std::false_type>
	//{
	//	static GenericInstanceType GetGenericInstanceType() { return GenericInstanceType::Int; }
	//};
	//template<> struct TypeTraits<float, std::false_type, std::false_type, std::false_type, std::false_type>
	//{
	//	static GenericInstanceType GetGenericInstanceType() { return GenericInstanceType::Float; }
	//};
	//template<> struct TypeTraits<String, std::false_type, std::false_type, std::false_type, std::false_type>
	//{
	//	static GenericInstanceType GetGenericInstanceType() { return GenericInstanceType::String; }
	//};
	//template<> struct TypeTraits<T, std::true_type, std::false_type, std::false_type, std::true_type>
	//{
	//	static GenericInstanceType GetGenericInstanceType() { return GenericInstanceType::Enum; }
	//}
	//template<> struct TypeTraits<T, std::false_type, std::true_type, std::false_type, std::true_type>
	//{
	//	static GenericInstanceType GetGenericInstanceType() { return GenericInstanceType::Object; }
	//};
	//template<> struct TypeTraits<T, std::false_type, std::true_type, std::true_type>
	//{
	//	static GenericInstanceType GetGenericInstanceType() { return GenericInstanceType::List; }
	//};
	//template<> struct TypeTraits<T, std::false_type, std::false_type, std::false_type>
	//{
	//	static GenericInstanceType GetGenericInstanceType() { return GenericInstanceType::List; }
	//};



	virtual ~GenericCoreList()
	{
		Clear();
	}

	virtual void AddVariant(const Variant& value)
	{
		Add(Variant::Cast<TValue>(value));
	}

protected:
	virtual void InsertItem(int index, const value_type& item)
	{
		CheckAddRef(item);
		Collection<TValue>::InsertItem(index, item);
	}

	//virtual void InsertItemConst(int index, const value_type& item)
	//{
	//	CheckAddRef(item);
	//	Collection<TValue>::InsertItemConst(index, item);
	//}

	virtual void ClearItems()
	{
		for (int i = 0; i < GetCount(); ++i) {
			CheckRelease(GetAt(i));
		}
		Collection<TValue>::ClearItems();
	}

	virtual void RemoveItem(int index)
	{
		CheckRelease(GetAt(index));
		Collection<TValue>::RemoveItem(index);
	}

	virtual void SetItem(int index, const value_type& item)
	{
		CheckAddRef(item);
		CheckRelease(GetAt(index));
		Collection<TValue>::SetItem(index, item);
	}

	//virtual void SetItemConst(int index, const value_type& item)
	//{
	//	CheckAddRef(item);
	//	CheckRelease(GetAt(index));
	//	Collection<TValue>::SetItemConst(index, item);
	//}

private:
	template<typename TValue, typename TIsRefObjectPtr = void>
	struct RefOperator
	{ 
		static void AddRef(TValue& v) {}
		static void AddRef(const TValue& v) {}
		static void Release(TValue& v) {}
		static void Release(const TValue& v) {}
	};

	template<typename TValue>
	struct RefOperator<TValue*, typename std::enable_if<std::is_base_of<RefObject, TValue>::value>::type>
	{
		static void AddRef(TValue* v) { v->AddRef(); }
		static void AddRef(const TValue* v) { LN_THROW(0, InvalidOperationException); }
		static void Release(TValue* v) { v->Release(); }
		static void Release(const TValue* v) { LN_THROW(0, InvalidOperationException); }
	};

	void CheckAddRef(const value_type& v)
	{
		RefOperator<TValue>::AddRef(v);
	}
	//void CheckAddRef(const_reference v)
	//{
	//	RefOperator<TValue>::AddRef(v);
	//}
	void CheckRelease(const value_type& v)
	{
		RefOperator<TValue>::Release(v);
	}
	//void CheckRelease(const_reference v)
	//{
	//	RefOperator<TValue>::Release(v);
	//}
};


LN_NAMESPACE_END
