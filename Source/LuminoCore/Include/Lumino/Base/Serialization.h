
#pragma once
#include "String.h"

LN_NAMESPACE_BEGIN
template <class T> class RefPtr;

namespace tr {
class ReflectionObject;

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
	

	// Array
	virtual int GetSerializeElementCount() const = 0;
	virtual ISerializeElement* GetSerializeElement(int index) const = 0;
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
public:
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
		Process(nvp.name, nvp.value);
		return *this;
	}

protected:
	//virtual RefPtr<ReflectionObject> CreateObject()
	//{

	//}

private:
	static const TCHAR* ClassNameKey;
	static const TCHAR* ClassVersionKey;

	template<typename T> void Process(const TCHAR* name, T && value)
	{
		switch (m_mode)
		{
		case ArchiveMode::Save:
			ProcessWrite(name, value);
			break;
		case ArchiveMode::Load:
			ProcessRead(name, value);
			break;
		default:
			assert(0);
			break;
		}
	}

	template<typename T>
	void ProcessWrite(const TCHAR* name, T && value)
	{
		WriteValue(name, value);
	}

	template<typename T>
	void ProcessRead(const TCHAR* name, T && value)
	{
		ISerializeElement* element = m_currentObject->FindSerializeElement(name);
		TryGetValue(element, &value);
	}

	//void WriteValue(const TCHAR* name, SerializableObject* obj);
	void WriteValue(const StringRef& name, bool value);
	void WriteValue(const StringRef& name, int8_t value);
	void WriteValue(const StringRef& name, int16_t value);
	void WriteValue(const StringRef& name, int32_t value) { m_currentObject->SetValueInt32(name, value); }
	void WriteValue(const StringRef& name, int64_t value);
	void WriteValue(const StringRef& name, uint8_t value);
	void WriteValue(const StringRef& name, uint16_t value);
	void WriteValue(const StringRef& name, uint32_t value);
	void WriteValue(const StringRef& name, uint64_t value);
	void WriteValue(const StringRef& name, float value);
	void WriteValue(const StringRef& name, double value);
	void WriteValue(const StringRef& name, String& value) { m_currentObject->SetValueString(name, value); }
	template<typename T> void WriteValue(const StringRef& name, T& obj)	 // non]intrusive Object
	{
		auto* old = m_currentObject;
		m_currentObject = m_currentObject->AddObject(name);
		obj.lnsl_SerializeImpl(*this, T::lnsl_GetClassVersion());
		m_currentObject = old;
	}
	template<typename T> void WriteValue(const StringRef& name, List<T>& obj)	 // non]intrusive Object
	{
		ISerializeElement* ary = m_currentObject->AddSerializeMemberNewArray(name);
		for (int i = 0; i < obj.GetCount(); i++)
		{
			AddItemValue(ary, obj[i]);
		}
	}



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
	//template<typename T> void ReadValue(const StringRef& name, T& obj)	 // non]intrusive Object
	//{
	//	ISerializeElement* so;
	//	if (!m_currentObject->TryGetObject(name, &so)) return;

	//	auto* old = m_currentObject;
	//	m_currentObject = so;
	//	obj.lnsl_SerializeImpl(*this);
	//	m_currentObject = old;
	//}
	//template<typename T> void ReadValue(const StringRef& name, List<T>& value)	 // non]intrusive Object
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
	//	//obj.lnsl_SerializeImpl(*this);
	//	//m_currentObject = old;
	//}


#define LN_DEFINE_TryGetValue(type, cppType) \
	static bool TryGetValue(ISerializeElement* element, cppType* value) { if (element->GetSerializationValueType() == SerializationValueType::type) { *value = element->GetSerializeValue##type(); return true; } return false; } \
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
		value.lnsl_SerializeImpl(*this, T::lnsl_GetClassVersion());
		//CallSave(value);
		m_currentObject = old;
	}
	// for ln::Object type
	template<typename T>
	void AddItemValue(ISerializeElement* arrayElement, RefPtr<T>& value)
	{
		ISerializeElement* objectElement = arrayElement->AddSerializeItemNewObject();

		auto* old = m_currentObject;
		m_currentObject = objectElement;
		//value->lnsl_SerializeImpl(*this, T::lnsl_GetClassVersion());
		CallSave(*value);
		m_currentObject = old;
	}




	template<typename T>
	bool TryGetValue(ISerializeElement* element, RefPtr<T>* value)
	{
		auto* old = m_currentObject;
		m_currentObject = element;
		(*value) = NewObject<T>();
		(*value)->lnsl_SerializeImpl(*this, T::lnsl_GetClassVersion());
		m_currentObject = old;
		return true;
	}
	template<typename T>
	bool TryGetValue(ISerializeElement* arrayElement, List<T>* value)
	{
		int count = arrayElement->GetSerializeElementCount();
		for (int i = 0; i < count; i++)
		{
			auto* element = arrayElement->GetSerializeElement(i);

			T raw;
			if (TryGetValue(element, &raw))
			{
				value->Add(raw);
			}
			else
			{
				LN_NOTIMPLEMENTED();
			}
		}
		return true;
	}
	template<typename T>
	bool TryGetValue(ISerializeElement* element, T* value)
	{
		auto* old = m_currentObject;
		m_currentObject = element;
		value->lnsl_SerializeImpl(*this, T::lnsl_GetClassVersion());
		m_currentObject = old;
		return true;
	}


	template<typename T>
	void CallSave(T& obj)
	{
		if (m_refrectionSupported)
		{
			TypeInfo* typeInfo = TypeInfo::GetTypeInfo(&obj);
			String typeName = typeInfo->GetName();
			int version = T::lnsl_GetClassVersion();
			m_currentObject->AddSerializeMemberValue(ClassNameKey, SerializationValueType::String, &typeName);
			m_currentObject->AddSerializeMemberValue(ClassVersionKey, SerializationValueType::Int32, &version);
			obj.lnsl_SerializeImpl(*this, version);
		}
		else
		{
			obj.lnsl_SerializeImpl(*this, 0);
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
	ISerializeElement*			m_currentObject;
	bool						m_refrectionSupported;

};

#define LN_SERIALIZE(ar, version, classVersion) \
	static const int lnsl_GetClassVersion() { return classVersion; } \
	void lnsl_SerializeImpl(tr::Archive& ar, int version)

} // namespace tr
LN_NAMESPACE_END
