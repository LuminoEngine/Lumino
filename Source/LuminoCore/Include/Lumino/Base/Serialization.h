
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

	void setInt(int value);
	void setString(const StringRef& value);
	void setList();
	void setMap();


private:
	void resetType(ScVariantType type);
	void releaseValue();
	//void serialize(Archive& ar, int version);

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
	ScVariantType getType() const;

	void setInt(int value);
	int getInt() const;

	void setString(const StringRef& value);
	const String& getString() const;

	void saveInternal(ISerializeElement* value);
	void loadInternal(ISerializeElement* value);

private:
	friend class Archive;
	ln::RefPtr<ScVariantCore>	m_core;
};

enum class ArchiveMode
{
	save,
	load,
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
NameValuePair<TRef> makeNVP(const TCHAR* name, TRef& valueRef)
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
NameValuePairBaseObject<TThis> makeNVPBaseObject(TThis* value)
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
	virtual SerializationElementType getSerializationElementType() const = 0;

	virtual void setValueInt32(const StringRef& name, int32_t value) = 0;
	//virtual void SetValueDouble(const StringRef& name, int32_t value) = 0;
	virtual void setValueString(const StringRef& name, const String& value) = 0;

	virtual ISerializeElement* addObject(const StringRef& name) = 0;


	virtual bool tryGetValueInt32(const StringRef& name, int32_t* outValue) = 0;
	virtual bool tryGetValueString(const StringRef& name, String* outValue) = 0;

	virtual bool tryGetObject(const StringRef& name, ISerializeElement** outValue) = 0;

	virtual bool tryGetArray(const StringRef& name, ISerializeElement** outValue) = 0;



	// Value
	virtual SerializationValueType getSerializationValueType() const = 0;
	virtual bool getSerializeValueBool() const = 0;
	virtual int8_t getSerializeValueInt8() const = 0;
	virtual int16_t getSerializeValueInt16() const = 0;
	virtual int32_t getSerializeValueInt32() const = 0;
	virtual int64_t getSerializeValueInt64() const = 0;
	virtual uint8_t getSerializeValueUInt8() const = 0;
	virtual uint16_t getSerializeValueUInt16() const = 0;
	virtual uint32_t getSerializeValueUInt32() const = 0;
	virtual uint64_t getSerializeValueUInt64() const = 0;
	virtual float getSerializeValueFloat() const = 0;
	virtual double getSerializeValueDouble() const = 0;
	virtual String getSerializeValueString() const = 0;
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
	virtual int getSerializeElementCount() const = 0;
	virtual ISerializeElement* getSerializeElement(int index) const = 0;
	virtual const String& getSerializeElementName(int index) const = 0;

	// Array
	virtual void addSerializeItemValue(SerializationValueType type, const void* value) = 0;
	virtual ISerializeElement* addSerializeItemNewArray() = 0;
	virtual ISerializeElement* addSerializeItemNewObject() = 0;

	// Object
	virtual ISerializeElement* findSerializeElement(const StringRef& name) const = 0;
	virtual void addSerializeMemberValue(const StringRef& name, SerializationValueType type, const void* value) = 0;
	virtual ISerializeElement* addSerializeMemberNewArray(const StringRef& name) = 0;
	virtual ISerializeElement* addSerializeMemberNewObject(const StringRef& name) = 0;


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
	virtual ISerializeElement* getRootObject() = 0;
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
		m_currentObject = m_stream->getRootObject();
	}

	template<class TRef>
	Archive& operator & (const NameValuePair<TRef>& nvp)
	{
		process(KeyInfo{ nvp.name, false }, nvp.value);
		return *this;
	}

	template<class TThis>
	Archive& operator & (const NameValuePairBaseObject<TThis>& nvp)
	{
		process(KeyInfo{ nvp.name, true }, RefPtr<TThis>(nvp.value));
		return *this;
	}

	//template<class T>
	//void serialize(const )
	//{

	//}

	ISerializeElement* getCurrentSerializeElement() const
	{
		return m_currentObject;
	}

	bool isSaving() const { return m_mode == ArchiveMode::save; }

	bool isLoading() const { return m_mode == ArchiveMode::load; }

	template<typename T>
	void save(T& value)
	{
		m_currentObject = saveHeaderElement(m_currentObject);
		doSaveObjectType(value, false);
	}
	void save(ScVariant& value)
	{
		if (LN_CHECK_ARG(value.getType() == ScVariantType::Map)) return;
		m_currentObject = saveHeaderElement(m_currentObject);
		doSaveObjectType(*value.m_core->m_map, false);
	}


	template<typename T>
	void load(T& value)
	{
		m_currentObject = loadHeaderElement(m_currentObject);
		tryGetValue(m_currentObject, &value, false);
	}
	void load(ScVariant& value)
	{
		m_currentObject = loadHeaderElement(m_currentObject);
		tryGetValue(m_currentObject, &value, false);
	}


protected:
	virtual RefPtr<ReflectionObject> createObject(const String& className, TypeInfo* requestedType);

private:

	ISerializeElement* saveHeaderElement(ISerializeElement* element)
	{
		int version = 1;
		element->addSerializeMemberValue(_T("version"), SerializationValueType::Int32, &version);
		return element->addSerializeMemberNewObject(_T("root"));
	}

	ISerializeElement* loadHeaderElement(ISerializeElement* element)
	{
		ISerializeElement* version = element->findSerializeElement(_T("version"));
		ISerializeElement* root = element->findSerializeElement(_T("root"));
		if (version != nullptr && root != nullptr)
		{
			return root;
		}
		return element;
	}



	template<typename T> void process(const KeyInfo& key, T && value)
	{
		switch (m_mode)
		{
		case ArchiveMode::save:
			processWrite(key, value);
			break;
		case ArchiveMode::load:
			processRead(key, value);
			break;
		default:
			assert(0);
			break;
		}
	}

	template<typename T>
	void processWrite(const KeyInfo& key, T && value)
	{
		addMemberValue(key, value);
	}

	template<typename T>
	void processRead(const KeyInfo& key, T && value)
	{
		ISerializeElement* element = m_currentObject->findSerializeElement(key.name);
		tryGetValue(element, &value, key.callBase);
	}

	// AddMemberValue()
	//		m_currentObject が示す Object 型へ、指定した名前と値のペアを追加する
	void addMemberValue(const KeyInfo& key, bool value) { m_currentObject->addSerializeMemberValue(key.name, SerializationValueType::Bool, &value); }
	void addMemberValue(const KeyInfo& key, int8_t value);
	void addMemberValue(const KeyInfo& key, int16_t value);
	void addMemberValue(const KeyInfo& key, int32_t value) { m_currentObject->setValueInt32(key.name, value); }
	void addMemberValue(const KeyInfo& key, int64_t value);
	void addMemberValue(const KeyInfo& key, uint8_t value);
	void addMemberValue(const KeyInfo& key, uint16_t value);
	void addMemberValue(const KeyInfo& key, uint32_t value);
	void addMemberValue(const KeyInfo& key, uint64_t value);
	void addMemberValue(const KeyInfo& key, float value) { m_currentObject->addSerializeMemberValue(key.name, SerializationValueType::Float, &value); }
	void addMemberValue(const KeyInfo& key, double value);
	void addMemberValue(const KeyInfo& key, String& value) { m_currentObject->setValueString(key.name, value); }
	template<typename T> void addMemberValue(const KeyInfo& key, T& obj)	 // non‐intrusive Object
	{
		auto* old = m_currentObject;
		m_currentObject = m_currentObject->addObject(key.name);
		doSaveObjectType(obj, false);
		m_currentObject = old;
	}
	template<typename T> void addMemberValue(const KeyInfo& key, RefPtr<T>& value)	 // non‐intrusive Object
	{
		auto* old = m_currentObject;
		m_currentObject = m_currentObject->addObject(key.name);
		doSaveObjectType(value, key.callBase);
		m_currentObject = old;
	}
	template<typename T> void addMemberValue(const KeyInfo& key, List<T>& obj)	 // non‐intrusive Object
	{
		ISerializeElement* ary = m_currentObject->addSerializeMemberNewArray(key.name);
		for (int i = 0; i < obj.getCount(); i++)
		{
			addItemValue(ary, obj[i]);
		}
	}
	template<typename T> void addMemberValue(const KeyInfo& key, std::unordered_map<String, T>& obj)	 // non‐intrusive Object
	{
		ISerializeElement* e = m_currentObject->addSerializeMemberNewObject(key.name);
		auto* old = m_currentObject;
		m_currentObject = e;
		doSaveObjectType(obj, key.callBase);
		m_currentObject = old;
	}
	void addMemberValue(const KeyInfo& key, ScVariant& value)	 // non‐intrusive Object
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
				addMemberValue(key, value.m_core->m_bool);
				break;
			case ScVariantType::Int:
				addMemberValue(key, value.m_core->m_int);
				break;
			case ScVariantType::Float:
				//AddMemberValue(key, value.m_core->m_float);
				break;
			case ScVariantType::String:
				//AddMemberValue(key, *value.m_core->m_string);
				break;
			case ScVariantType::List:
				addMemberValue(key, *value.m_core->m_list);
				break;
			case ScVariantType::Map:
				addMemberValue(key, *value.m_core->m_map);
				break;
			default:
				LN_UNREACHABLE();
				break;
			}
		}
	}

	// DoSaveObjectType()
	//		m_currentObject が示す Object 型へ、指定した名前と値のペアを追加する
	template<typename T> void doSaveObjectType(T& value, bool baseCall)
	{
		value.serialize(*this, 0);
	}
	template<typename T> void doSaveObjectType(RefPtr<T>& value, bool baseCall)
	{
		callSave(*value, baseCall);
	}
	template<typename T> void doSaveObjectType(std::unordered_map<String, T>& value, bool baseCall)
	{
		for (auto& pair : value)
		{
			addMemberValue({ pair.first, baseCall }, pair.second);
		}
	}

	//template<typename T> void AddMemberValueInternal(T& obj)
	//{
	//	obj.serialize(*this, 0);
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
	//	obj.serialize(*this);
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
	//	//obj.serialize(*this);
	//	//m_currentObject = old;
	//}


#define LN_DEFINE_TryGetValue(type, cppType) \
	static bool tryGetValue(ISerializeElement* element, cppType* value, bool) { if (element->getSerializationValueType() == SerializationValueType::type) { *value = element->getSerializeValue##type(); return true; } return false; } \
	static void addItemValue(ISerializeElement* arrayElement, cppType value) { arrayElement->addSerializeItemValue(SerializationValueType::type, &value); }

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
	void addItemValue(ISerializeElement* arrayElement, T& value)
	{
		ISerializeElement* objectElement = arrayElement->addSerializeItemNewObject();

		auto* old = m_currentObject;
		m_currentObject = objectElement;
		value.serialize(*this, 0);
		//CallSave(value);
		m_currentObject = old;
	}
	// for ln::Object type
	template<typename T>
	void addItemValue(ISerializeElement* arrayElement, RefPtr<T>& value)
	{
		if (value == nullptr)
		{
			arrayElement->addSerializeItemValue(SerializationValueType::Null, nullptr);
		}
		else
		{
			ISerializeElement* objectElement = arrayElement->addSerializeItemNewObject();
			auto* old = m_currentObject;
			m_currentObject = objectElement;
			//value->serialize(*this, T::lnsl_GetClassVersion());
			callSave(*value, false);
			m_currentObject = old;
		}
	}
	template<typename T> void addItemValue(ISerializeElement* arrayElement, List<T>& obj)	 // non‐intrusive Object
	{
		ISerializeElement* ary = arrayElement->addSerializeItemNewArray();
		for (int i = 0; i < obj.getCount(); i++)
		{
			addItemValue(ary, obj[i]);
		}
	}
	void addItemValue(ISerializeElement* arrayElement, ScVariant& value)
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
				addItemValue(arrayElement, value.m_core->m_bool);
				break;
			case ScVariantType::Int:
				addItemValue(arrayElement, value.m_core->m_int);
				break;
			case ScVariantType::Float:
				addItemValue(arrayElement, value.m_core->m_float);
				break;
			case ScVariantType::String:
				addItemValue(arrayElement, *value.m_core->m_string);
				break;
			case ScVariantType::List:
				addItemValue(arrayElement, *value.m_core->m_list);
				break;
			case ScVariantType::Map:
				addItemValue(arrayElement, *value.m_core->m_map);
				break;
			default:
				LN_UNREACHABLE();
				break;
			}
		}
	}
	template<typename T> void addItemValue(ISerializeElement* arrayElement, std::unordered_map<String, T>& obj)	 // non‐intrusive Object
	{
		ISerializeElement* objectElement = arrayElement->addSerializeItemNewObject();
		auto* old = m_currentObject;
		m_currentObject = objectElement;
		doSaveObjectType(obj, false);
		m_currentObject = old;
	}


	template<typename T>
	bool tryGetValue(ISerializeElement* element, RefPtr<T>* value, bool callBase)
	{
		auto* old = m_currentObject;
		m_currentObject = element;
		callLoad(value, callBase);
		m_currentObject = old;
		return true;
	}
	template<typename T>
	bool tryGetValue(ISerializeElement* arrayElement, List<T>* value, bool)
	{
		int count = arrayElement->getSerializeElementCount();
		for (int i = 0; i < count; i++)
		{
			auto* element = arrayElement->getSerializeElement(i);
			if (element->getSerializationElementType() == SerializationElementType::Value &&
				element->getSerializationValueType() == SerializationValueType::Null)
			{
				value->add(T());	// null
			}
			else
			{
				T raw;
				if (tryGetValue(element, &raw, false))
				{
					value->add(raw);
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
	bool tryGetValue(ISerializeElement* element, T* value, bool callBase)
	{
		auto* old = m_currentObject;
		m_currentObject = element;
		value->serialize(*this, 0);
		m_currentObject = old;
		return true;
	}
	bool tryGetValue(ISerializeElement* element, ScVariant* value, bool callBase)
	{
		value->loadInternal(element);
		return true;
	}


	//template<typename T>
	//void DoLoadObjectType(T* value, bool callBase)
	//{
	//	value->serialize(*this, 0);
	//}
	//void DoLoadObjectType(ScVariant* value, bool callBase)
	//{
	//	
	//}



	template<typename T>
	void callSave(T& obj, bool callBase)
	{
		if (m_refrectionSupported)
		{
			TypeInfo* typeInfo;

			if (callBase)
			{
				typeInfo = TypeInfo::getTypeInfo<T>();
			}
			else
			{
				typeInfo = TypeInfo::getTypeInfo(&obj);
			}

			int version = 0;
			if (typeInfo != TypeInfo::getTypeInfo<Object>())
			{
				String typeName = typeInfo->getName();
				int version = typeInfo->getSerializeClassVersion();
				m_currentObject->addSerializeMemberValue(ClassNameKey, SerializationValueType::String, &typeName);
				m_currentObject->addSerializeMemberValue(ClassVersionKey, SerializationValueType::Int32, &version);
			}
			else
			{
				// "Object" の型情報しかとれないということは、T の TypeInfo が定義されていない。
				// 保存しても復元できないので、型情報は保存せずプレーンな情報にする。
			}

			if (callBase)
				obj.T::serialize(*this, version);
			else
				obj.serialize(*this, version);
		}
		else
		{
			obj.serialize(*this, 0);
		}
	}

	template<typename T>
	void callLoad(RefPtr<T>* value, bool callBase)
	{
		bool loaded = false;
		if (m_refrectionSupported)
		{
			ISerializeElement* classNameElement = m_currentObject->findSerializeElement(ClassNameKey);
			ISerializeElement* classVersionElement = m_currentObject->findSerializeElement(ClassVersionKey);
			if (classNameElement != nullptr && classVersionElement != nullptr)
			{
				String className;
				int classVersion;
				if (tryGetValue(classNameElement, &className, false) && tryGetValue(classVersionElement, &classVersion, false))
				{
					TypeInfo* typeInfo = TypeInfo::getTypeInfo<T>();
					if (callBase)
					{
						(*value)->T::serialize(*this, classVersion);
					}
					else
					{
						(*value) = RefPtr<T>::staticCast(createObject(className, typeInfo));
						(*value)->serialize(*this, classVersion);
					}
					loaded = true;
				}
			}
		}
		
		if (!loaded)
		{
			(*value) = newObject<T>();
			(*value)->serialize(*this, 0);
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
	ISerializeElement*			m_currentObject;			//　serialize() 内で & された値の保存先、または復元元となる親 Object(Map)。serialize() に入るとき、にセットされる。
	bool						m_refrectionSupported;

};


//#define LN_SERIALIZE(ar, version, classVersion) \
//	static const int lnsl_GetClassVersion() { return classVersion; } \
//	virtual void serialize(tr::Archive& ar, int version)

} // namespace tr

#define LN_NVP(var)	ln::tr::makeNVP(#var, var)

LN_NAMESPACE_END
