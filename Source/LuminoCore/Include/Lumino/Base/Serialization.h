
#pragma once
#include <unordered_map>
#include "String.h"
#include "../Reflection/ReflectionObject.h"

LN_NAMESPACE_BEGIN
//template <class T> class RefPtr;

namespace tr {
//class ReflectionObject;
//class TypeInfo;
class Archive;

class ScVariant;
class ISerializeElement;

enum class ScVariantType
{
	Unknown,	// TODO: Null
	Bool,
	Int,
	Float,
	String,
	List,
	Map,
};

class ScVariantCore : public ln::Object
{
public:
	ScVariantCore()
		: m_type(ScVariantType::Unknown)
	{}

	virtual ~ScVariantCore();

	void SetInt(int value);
	void SetString(const StringRef& value);
	void SetList();
	void SetMap();


private:
	void ResetType(ScVariantType type);
	void ReleaseValue();
	//void Serialize(Archive& ar, int version);

	ScVariantType	m_type;
	union
	{
		bool										m_bool;
		int											m_int;
		float										m_float;
		ln::String*									m_string;
		ln::List<ScVariant>*						m_list;
		std::unordered_map<ln::String, ScVariant>*	m_map;
	};

	friend class ScVariant;
	friend class Archive;
};

class ScVariant
{
public:
	ScVariantType GetType() const;

	void SetInt(int value);
	int GetInt() const;

	void SetString(const StringRef& value);
	const String& GetString() const;

	void SaveInternal(ISerializeElement* value);
	void LoadInternal(ISerializeElement* value);

private:
	friend class Archive;
	ln::RefPtr<ScVariantCore>	m_core;
};

enum class ArchiveMode
{
	Save,
	Load,
};

/**
	@brief
*/
template<typename TRef>
class NameValuePair
{
public:
	const TCHAR* name;
	TRef& value;

	NameValuePair(const TCHAR* n, TRef& v) : name(n), value(v) {}

private:
	NameValuePair & operator=(NameValuePair const &) = delete;
};

template<typename TRef>
NameValuePair<TRef> MakeNVP(const TCHAR* name, TRef& valueRef)
{
	return NameValuePair<TRef>(name, valueRef);
}



template<typename TThis>
class NameValuePairBaseObject
{
public:
	const TCHAR* name;
	TThis* value;

	NameValuePairBaseObject(const TCHAR* n, TThis* v) : name(n), value(v) {}

private:
	NameValuePairBaseObject & operator=(NameValuePairBaseObject const &) = delete;
};

template<typename TThis>
NameValuePairBaseObject<TThis> MakeNVPBaseObject(TThis* value)
{
	return NameValuePairBaseObject<TThis>(Archive::ClassBaseDefaultNameKey, value);
}

///**
//	@brief
//*/
//class ISerializeArrayElement
//	: public ISerializeElement
//{
//public:
//	virtual int GetSerializeElementCount() const = 0;
//	virtual JsonElement2* GetSerializeElement(int index) const = 0;
//};


enum class SerializationElementType
{
	Value,
	Array,
	Object,
};

enum class SerializationValueType
{
	Null,
	Bool,
	Int8,
	Int16,
	Int32,
	Int64,
	UInt8,
	UInt16,
	UInt32,
	UInt64,
	Float,
	Double,
	String,
};


/**
	@brief
*/
class ISerializeElement
{
public:
	virtual SerializationElementType GetSerializationElementType() const = 0;

	virtual void SetValueInt32(const StringRef& name, int32_t value) = 0;
	//virtual void SetValueDouble(const StringRef& name, int32_t value) = 0;
	virtual void SetValueString(const StringRef& name, const String& value) = 0;

	virtual ISerializeElement* AddObject(const StringRef& name) = 0;


	virtual bool TryGetValueInt32(const StringRef& name, int32_t* outValue) = 0;
	virtual bool TryGetValueString(const StringRef& name, String* outValue) = 0;

	virtual bool TryGetObject(const StringRef& name, ISerializeElement** outValue) = 0;

	virtual bool TryGetArray(const StringRef& name, ISerializeElement** outValue) = 0;



	// Value
	virtual SerializationValueType GetSerializationValueType() const = 0;
	virtual bool GetSerializeValueBool() const = 0;
	virtual int8_t GetSerializeValueInt8() const = 0;
	virtual int16_t GetSerializeValueInt16() const = 0;
	virtual int32_t GetSerializeValueInt32() const = 0;
	virtual int64_t GetSerializeValueInt64() const = 0;
	virtual uint8_t GetSerializeValueUInt8() const = 0;
	virtual uint16_t GetSerializeValueUInt16() const = 0;
	virtual uint32_t GetSerializeValueUInt32() const = 0;
	virtual uint64_t GetSerializeValueUInt64() const = 0;
	virtual float GetSerializeValueFloat() const = 0;
	virtual double GetSerializeValueDouble() const = 0;
	virtual String GetSerializeValueString() const = 0;
	//virtual bool SetSerializeValueBool() const = 0;
	//virtual int8_t SetSerializeValueInt8() const = 0;
	//virtual int16_t SetSerializeValueInt16() const = 0;
	//virtual int32_t SetSerializeValueInt32() const = 0;
	//virtual int64_t SetSerializeValueInt64() const = 0;
	//virtual uint8_t SetSerializeValueUInt8() const = 0;
	//virtual uint16_t SetSerializeValueUInt16() const = 0;
	//virtual uint32_t SetSerializeValueUInt32() const = 0;
	//virtual uint64_t SetSerializeValueUInt64() const = 0;
	//virtual float SetSerializeValueFloat() const = 0;
	//virtual double SetSerializeValueDouble() const = 0;
	//virtual String SetSerializeValueString() const = 0;
	
	// Array and Object
	virtual int GetSerializeElementCount() const = 0;
	virtual ISerializeElement* GetSerializeElement(int index) const = 0;
	virtual const String& GetSerializeElementName(int index) const = 0;

	// Array
	virtual void AddSerializeItemValue(SerializationValueType type, const void* value) = 0;
	virtual ISerializeElement* AddSerializeItemNewArray() = 0;
	virtual ISerializeElement* AddSerializeItemNewObject() = 0;

	// Object
	virtual ISerializeElement* FindSerializeElement(const StringRef& name) const = 0;
	virtual void AddSerializeMemberValue(const StringRef& name, SerializationValueType type, const void* value) = 0;
	virtual ISerializeElement* AddSerializeMemberNewArray(const StringRef& name) = 0;
	virtual ISerializeElement* AddSerializeMemberNewObject(const StringRef& name) = 0;


//#define LN_DEFINE_ValueAccessor(type, cppType) \
//	virtual cppType GetSerializeValue##type() const = 0; \
//	virtual cppType AddSerializeValueItem##type(cppType value) = 0; \
//	virtual cppType AddSerializeValueMember##type(const StringRef& name, cppType value) = 0;
//
//	LN_DEFINE_ValueAccessor(Bool, bool);
//	LN_DEFINE_ValueAccessor(Int8, int8_t);
//	LN_DEFINE_ValueAccessor(Int16, int16_t);
//	LN_DEFINE_ValueAccessor(Int32, int32_t);
//	LN_DEFINE_ValueAccessor(Int64, int64_t);
//	LN_DEFINE_ValueAccessor(UInt8, uint8_t);
//	LN_DEFINE_ValueAccessor(UInt16, uint16_t);
//	LN_DEFINE_ValueAccessor(UInt32, uint32_t);
//	LN_DEFINE_ValueAccessor(UInt64, uint64_t);
//	LN_DEFINE_ValueAccessor(Float, float);
//	LN_DEFINE_ValueAccessor(Double, double);
//	LN_DEFINE_ValueAccessor(String, String);
//
//#undef LN_DEFINE_ValueAccessor
};

/**
	@brief
*/
class ISerializationeStore
{
public:
	virtual ISerializeElement* GetRootObject() = 0;
};


/**
	@brief
*/
class Archive
{

	struct KeyInfo
	{
		StringRef name;
		bool callBase;
	};

public:
	static const TCHAR* ClassNameKey;
	static const TCHAR* ClassVersionKey;
	static const TCHAR* ClassBaseDefaultNameKey;
	//Archive(const PathName& filePath, ArchiveMode mode);

	//template<typename T> Archive& operator&(T && arg)
	//{
	//	Process(std::forward<T>(arg));
	//	return *self;
	//}

	Archive(ISerializationeStore* stream, ArchiveMode mode, bool refrectionSupported)
		: m_mode(mode)
		, m_stream(stream)
		, m_refrectionSupported(refrectionSupported)
	{
		m_currentObject = m_stream->GetRootObject();
	}

	template<class TRef>
	Archive& operator & (const NameValuePair<TRef>& nvp)
	{
		Process(KeyInfo{ nvp.name, false }, nvp.value);
		return *this;
	}

	template<class TThis>
	Archive& operator & (const NameValuePairBaseObject<TThis>& nvp)
	{
		Process(KeyInfo{ nvp.name, true }, RefPtr<TThis>(nvp.value));
		return *this;
	}

	//template<class T>
	//void Serialize(const )
	//{

	//}

	ISerializeElement* GetCurrentSerializeElement() const
	{
		return m_currentObject;
	}

	bool IsSaving() const { return m_mode == ArchiveMode::Save; }

	bool IsLoading() const { return m_mode == ArchiveMode::Load; }

	template<typename T>
	void Save(T& value)
	{
		m_currentObject = SaveHeaderElement(m_currentObject);
		DoSaveObjectType(value, false);
	}
	void Save(ScVariant& value)
	{
		if (LN_CHECK_ARG(value.GetType() == ScVariantType::Map)) return;
		m_currentObject = SaveHeaderElement(m_currentObject);
		DoSaveObjectType(*value.m_core->m_map, false);
	}


	template<typename T>
	void Load(T& value)
	{
		m_currentObject = LoadHeaderElement(m_currentObject);
		TryGetValue(m_currentObject, &value, false);
	}
	void Load(ScVariant& value)
	{
		m_currentObject = LoadHeaderElement(m_currentObject);
		TryGetValue(m_currentObject, &value, false);
	}


protected:
	virtual RefPtr<ReflectionObject> CreateObject(const String& className, TypeInfo* requestedType);

private:

	ISerializeElement* SaveHeaderElement(ISerializeElement* element)
	{
		int version = 1;
		element->AddSerializeMemberValue(_T("version"), SerializationValueType::Int32, &version);
		return element->AddSerializeMemberNewObject(_T("root"));
	}

	ISerializeElement* LoadHeaderElement(ISerializeElement* element)
	{
		ISerializeElement* version = element->FindSerializeElement(_T("version"));
		ISerializeElement* root = element->FindSerializeElement(_T("root"));
		if (version != nullptr && root != nullptr)
		{
			return root;
		}
		return element;
	}



	template<typename T> void Process(const KeyInfo& key, T && value)
	{
		switch (m_mode)
		{
		case ArchiveMode::Save:
			ProcessWrite(key, value);
			break;
		case ArchiveMode::Load:
			ProcessRead(key, value);
			break;
		default:
			assert(0);
			break;
		}
	}

	template<typename T>
	void ProcessWrite(const KeyInfo& key, T && value)
	{
		AddMemberValue(key, value);
	}

	template<typename T>
	void ProcessRead(const KeyInfo& key, T && value)
	{
		ISerializeElement* element = m_currentObject->FindSerializeElement(key.name);
		TryGetValue(element, &value, key.callBase);
	}

	// AddMemberValue()
	//		m_currentObject が示す Object 型へ、指定した名前と値のペアを追加する
	void AddMemberValue(const KeyInfo& key, bool value) { m_currentObject->AddSerializeMemberValue(key.name, SerializationValueType::Bool, &value); }
	void AddMemberValue(const KeyInfo& key, int8_t value);
	void AddMemberValue(const KeyInfo& key, int16_t value);
	void AddMemberValue(const KeyInfo& key, int32_t value) { m_currentObject->SetValueInt32(key.name, value); }
	void AddMemberValue(const KeyInfo& key, int64_t value);
	void AddMemberValue(const KeyInfo& key, uint8_t value);
	void AddMemberValue(const KeyInfo& key, uint16_t value);
	void AddMemberValue(const KeyInfo& key, uint32_t value);
	void AddMemberValue(const KeyInfo& key, uint64_t value);
	void AddMemberValue(const KeyInfo& key, float value) { m_currentObject->AddSerializeMemberValue(key.name, SerializationValueType::Float, &value); }
	void AddMemberValue(const KeyInfo& key, double value);
	void AddMemberValue(const KeyInfo& key, String& value) { m_currentObject->SetValueString(key.name, value); }
	template<typename T> void AddMemberValue(const KeyInfo& key, T& obj)	 // non‐intrusive Object
	{
		auto* old = m_currentObject;
		m_currentObject = m_currentObject->AddObject(key.name);
		DoSaveObjectType(obj, false);
		m_currentObject = old;
	}
	template<typename T> void AddMemberValue(const KeyInfo& key, RefPtr<T>& value)	 // non‐intrusive Object
	{
		auto* old = m_currentObject;
		m_currentObject = m_currentObject->AddObject(key.name);
		DoSaveObjectType(value, key.callBase);
		m_currentObject = old;
	}
	template<typename T> void AddMemberValue(const KeyInfo& key, List<T>& obj)	 // non‐intrusive Object
	{
		ISerializeElement* ary = m_currentObject->AddSerializeMemberNewArray(key.name);
		for (int i = 0; i < obj.GetCount(); i++)
		{
			AddItemValue(ary, obj[i]);
		}
	}
	template<typename T> void AddMemberValue(const KeyInfo& key, std::unordered_map<String, T>& obj)	 // non‐intrusive Object
	{
		ISerializeElement* e = m_currentObject->AddSerializeMemberNewObject(key.name);
		auto* old = m_currentObject;
		m_currentObject = e;
		DoSaveObjectType(obj, key.callBase);
		m_currentObject = old;
	}
	void AddMemberValue(const KeyInfo& key, ScVariant& value)	 // non‐intrusive Object
	{
		if (value.m_core == nullptr || value.m_core->m_type == ScVariantType::Unknown)
		{
			LN_NOTIMPLEMENTED();
		}
		else
		{
			switch (value.m_core->m_type)
			{
			case ScVariantType::Bool:
				AddMemberValue(key, value.m_core->m_bool);
				break;
			case ScVariantType::Int:
				AddMemberValue(key, value.m_core->m_int);
				break;
			case ScVariantType::Float:
				//AddMemberValue(key, value.m_core->m_float);
				break;
			case ScVariantType::String:
				//AddMemberValue(key, *value.m_core->m_string);
				break;
			case ScVariantType::List:
				AddMemberValue(key, *value.m_core->m_list);
				break;
			case ScVariantType::Map:
				AddMemberValue(key, *value.m_core->m_map);
				break;
			default:
				LN_UNREACHABLE();
				break;
			}
		}
	}

	// DoSaveObjectType()
	//		m_currentObject が示す Object 型へ、指定した名前と値のペアを追加する
	template<typename T> void DoSaveObjectType(T& value, bool baseCall)
	{
		value.Serialize(*this, 0);
	}
	template<typename T> void DoSaveObjectType(RefPtr<T>& value, bool baseCall)
	{
		CallSave(*value, baseCall);
	}
	template<typename T> void DoSaveObjectType(std::unordered_map<String, T>& value, bool baseCall)
	{
		for (auto& pair : value)
		{
			AddMemberValue({ pair.first, baseCall }, pair.second);
		}
	}

	//template<typename T> void AddMemberValueInternal(T& obj)
	//{
	//	obj.Serialize(*this, 0);
	//}
	//template<typename T> void AddMemberValueInternal(RefPtr<T>& value)
	//{
	//	CallSave(*value, key.callBase);
	//}
	//template<typename T> void AddMemberValueInternal(RefPtr<T>& value)
	//{
	//	CallSave(*value, key.callBase);
	//}


	//void ReadValue(const StringRef& name, bool& value);
	//void ReadValue(const StringRef& name, int8_t& value);
	//void ReadValue(const StringRef& name, int16_t& value);
	//void ReadValue(const StringRef& name, int32_t& value) { m_currentObject->TryGetValueInt32(name, &value); }	// TODO: Error
	//void ReadValue(const StringRef& name, int64_t value);
	//void ReadValue(const StringRef& name, uint8_t& value);
	//void ReadValue(const StringRef& name, uint16_t& value);
	//void ReadValue(const StringRef& name, uint32_t& value);
	//void ReadValue(const StringRef& name, uint64_t& value);
	//void ReadValue(const StringRef& name, float& value);
	//void ReadValue(const StringRef& name, double& value);
	//void ReadValue(const StringRef& name, String& value) { m_currentObject->TryGetValueString(name, &value); }	// TODO: Error
	//template<typename T> void ReadValue(const StringRef& name, T& obj)	 // non‐intrusive Object
	//{
	//	ISerializeElement* so;
	//	if (!m_currentObject->TryGetObject(name, &so)) return;

	//	auto* old = m_currentObject;
	//	m_currentObject = so;
	//	obj.Serialize(*this);
	//	m_currentObject = old;
	//}
	//template<typename T> void ReadValue(const StringRef& name, List<T>& value)	 // non‐intrusive Object
	//{
	//	ISerializeElement* arrayElement;
	//	if (!m_currentObject->TryGetArray(name, &arrayElement)) return;

	//	int count = arrayElement->GetSerializeElementCount();
	//	for (int i = 0; i < count; i++)
	//	{
	//		auto* element = arrayElement->GetSerializeElement(i);

	//		T raw;
	//		if (TryGetValue(element, &raw))
	//		{
	//			value.Add(raw);
	//		}
	//		else
	//		{
	//			LN_NOTIMPLEMENTED();
	//		}
	//	}

	//	//auto* old = m_currentObject;
	//	//m_currentObject = so;
	//	//obj.Serialize(*this);
	//	//m_currentObject = old;
	//}


#define LN_DEFINE_TryGetValue(type, cppType) \
	static bool TryGetValue(ISerializeElement* element, cppType* value, bool) { if (element->GetSerializationValueType() == SerializationValueType::type) { *value = element->GetSerializeValue##type(); return true; } return false; } \
	static void AddItemValue(ISerializeElement* arrayElement, cppType value) { arrayElement->AddSerializeItemValue(SerializationValueType::type, &value); }

	LN_DEFINE_TryGetValue(Bool, bool);
	LN_DEFINE_TryGetValue(Int8, int8_t);
	LN_DEFINE_TryGetValue(Int16, int16_t);
	LN_DEFINE_TryGetValue(Int32, int32_t);
	LN_DEFINE_TryGetValue(Int64, int64_t);
	LN_DEFINE_TryGetValue(UInt8, uint8_t);
	LN_DEFINE_TryGetValue(UInt16, uint16_t);
	LN_DEFINE_TryGetValue(UInt32, uint32_t);
	LN_DEFINE_TryGetValue(UInt64, uint64_t);
	LN_DEFINE_TryGetValue(Float, float);
	LN_DEFINE_TryGetValue(Double, double);
	LN_DEFINE_TryGetValue(String, String);

#undef LN_DEFINE_TryGetValue

	// for other struct type
	template<typename T>
	void AddItemValue(ISerializeElement* arrayElement, T& value)
	{
		ISerializeElement* objectElement = arrayElement->AddSerializeItemNewObject();

		auto* old = m_currentObject;
		m_currentObject = objectElement;
		value.Serialize(*this, 0);
		//CallSave(value);
		m_currentObject = old;
	}
	// for ln::Object type
	template<typename T>
	void AddItemValue(ISerializeElement* arrayElement, RefPtr<T>& value)
	{
		if (value == nullptr)
		{
			arrayElement->AddSerializeItemValue(SerializationValueType::Null, nullptr);
		}
		else
		{
			ISerializeElement* objectElement = arrayElement->AddSerializeItemNewObject();
			auto* old = m_currentObject;
			m_currentObject = objectElement;
			//value->Serialize(*this, T::lnsl_GetClassVersion());
			CallSave(*value, false);
			m_currentObject = old;
		}
	}
	template<typename T> void AddItemValue(ISerializeElement* arrayElement, List<T>& obj)	 // non‐intrusive Object
	{
		ISerializeElement* ary = arrayElement->AddSerializeItemNewArray();
		for (int i = 0; i < obj.GetCount(); i++)
		{
			AddItemValue(ary, obj[i]);
		}
	}
	void AddItemValue(ISerializeElement* arrayElement, ScVariant& value)
	{
		if (value.m_core == nullptr || value.m_core->m_type == ScVariantType::Unknown)
		{
			LN_NOTIMPLEMENTED();
		}
		else
		{
			switch (value.m_core->m_type)
			{
			case ScVariantType::Bool:
				AddItemValue(arrayElement, value.m_core->m_bool);
				break;
			case ScVariantType::Int:
				AddItemValue(arrayElement, value.m_core->m_int);
				break;
			case ScVariantType::Float:
				AddItemValue(arrayElement, value.m_core->m_float);
				break;
			case ScVariantType::String:
				AddItemValue(arrayElement, *value.m_core->m_string);
				break;
			case ScVariantType::List:
				AddItemValue(arrayElement, *value.m_core->m_list);
				break;
			case ScVariantType::Map:
				AddItemValue(arrayElement, *value.m_core->m_map);
				break;
			default:
				LN_UNREACHABLE();
				break;
			}
		}
	}
	template<typename T> void AddItemValue(ISerializeElement* arrayElement, std::unordered_map<String, T>& obj)	 // non‐intrusive Object
	{
		ISerializeElement* objectElement = arrayElement->AddSerializeItemNewObject();
		auto* old = m_currentObject;
		m_currentObject = objectElement;
		DoSaveObjectType(obj, false);
		m_currentObject = old;
	}


	template<typename T>
	bool TryGetValue(ISerializeElement* element, RefPtr<T>* value, bool callBase)
	{
		auto* old = m_currentObject;
		m_currentObject = element;
		CallLoad(value, callBase);
		m_currentObject = old;
		return true;
	}
	template<typename T>
	bool TryGetValue(ISerializeElement* arrayElement, List<T>* value, bool)
	{
		int count = arrayElement->GetSerializeElementCount();
		for (int i = 0; i < count; i++)
		{
			auto* element = arrayElement->GetSerializeElement(i);
			if (element->GetSerializationElementType() == SerializationElementType::Value &&
				element->GetSerializationValueType() == SerializationValueType::Null)
			{
				value->Add(T());	// null
			}
			else
			{
				T raw;
				if (TryGetValue(element, &raw, false))
				{
					value->Add(raw);
				}
				else
				{
					LN_NOTIMPLEMENTED();
				}
			}

		}
		return true;
	}
	template<typename T>
	bool TryGetValue(ISerializeElement* element, T* value, bool callBase)
	{
		auto* old = m_currentObject;
		m_currentObject = element;
		value->Serialize(*this, 0);
		m_currentObject = old;
		return true;
	}
	bool TryGetValue(ISerializeElement* element, ScVariant* value, bool callBase)
	{
		value->LoadInternal(element);
		return true;
	}


	//template<typename T>
	//void DoLoadObjectType(T* value, bool callBase)
	//{
	//	value->Serialize(*this, 0);
	//}
	//void DoLoadObjectType(ScVariant* value, bool callBase)
	//{
	//	
	//}



	template<typename T>
	void CallSave(T& obj, bool callBase)
	{
		if (m_refrectionSupported)
		{
			TypeInfo* typeInfo;

			if (callBase)
			{
				typeInfo = TypeInfo::GetTypeInfo<T>();
			}
			else
			{
				typeInfo = TypeInfo::GetTypeInfo(&obj);
			}

			int version = 0;
			if (typeInfo != TypeInfo::GetTypeInfo<Object>())
			{
				String typeName = typeInfo->GetName();
				int version = typeInfo->GetSerializeClassVersion();
				m_currentObject->AddSerializeMemberValue(ClassNameKey, SerializationValueType::String, &typeName);
				m_currentObject->AddSerializeMemberValue(ClassVersionKey, SerializationValueType::Int32, &version);
			}
			else
			{
				// "Object" の型情報しかとれないということは、T の TypeInfo が定義されていない。
				// 保存しても復元できないので、型情報は保存せずプレーンな情報にする。
			}

			if (callBase)
				obj.T::Serialize(*this, version);
			else
				obj.Serialize(*this, version);
		}
		else
		{
			obj.Serialize(*this, 0);
		}
	}

	template<typename T>
	void CallLoad(RefPtr<T>* value, bool callBase)
	{
		bool loaded = false;
		if (m_refrectionSupported)
		{
			ISerializeElement* classNameElement = m_currentObject->FindSerializeElement(ClassNameKey);
			ISerializeElement* classVersionElement = m_currentObject->FindSerializeElement(ClassVersionKey);
			if (classNameElement != nullptr && classVersionElement != nullptr)
			{
				String className;
				int classVersion;
				if (TryGetValue(classNameElement, &className, false) && TryGetValue(classVersionElement, &classVersion, false))
				{
					TypeInfo* typeInfo = TypeInfo::GetTypeInfo<T>();
					if (callBase)
					{
						(*value)->T::Serialize(*this, classVersion);
					}
					else
					{
						(*value) = RefPtr<T>::StaticCast(CreateObject(className, typeInfo));
						(*value)->Serialize(*this, classVersion);
					}
					loaded = true;
				}
			}
		}
		
		if (!loaded)
		{
			(*value) = NewObject<T>();
			(*value)->Serialize(*this, 0);
		}
	}
/*
	static bool TryGetValue(ISerializeElement* element, bool* value) { if (element->GetSerializationValueType() == SerializationValueType::Bool) { *value = element->GetSerializeValueBool(); return true; } return false; }

	virtual SerializationValueType GetSerializationValueType() const = 0;
	virtual bool GetSerializeValueBool() const = 0;
	virtual int8_t GetSerializeValue() const = 0;
	virtual int16_t GetSerializeValueInt16() const = 0;
	virtual int32_t GetSerializeValueInt32() const = 0;
	virtual int64_t GetSerializeValueInt64() const = 0;
	virtual uint8_t GetSerializeValueUInt8() const = 0;
	virtual uint16_t GetSerializeValueUInt16() const = 0;
	virtual uint32_t GetSerializeValueUInt32() const = 0;
	virtual uint64_t GetSerializeValueUInt64() const = 0;
	virtual float GetSerializeValueFloat() const = 0;
	virtual double GetSerializeValueDouble() const = 0;
	virtual String GetSerializeValueString() const = 0;*/

	ArchiveMode					m_mode;
	ISerializationeStore*		m_stream;
	ISerializeElement*			m_currentObject;			//　Serialize() 内で & された値の保存先、または復元元となる親 Object(Map)。Serialize() に入るとき、にセットされる。
	bool						m_refrectionSupported;

};


//#define LN_SERIALIZE(ar, version, classVersion) \
//	static const int lnsl_GetClassVersion() { return classVersion; } \
//	virtual void Serialize(tr::Archive& ar, int version)

} // namespace tr

#define LN_NVP(var)	ln::tr::MakeNVP(#var, var)

LN_NAMESPACE_END
