
#pragma once
#include "String.h"

LN_NAMESPACE_BEGIN
namespace tr {

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



/**
	@brief
*/
class ISerializeObjectElement
{
public:

	virtual void SetValueInt32(const StringRef& name, int32_t value) = 0;
	//virtual void SetValueDouble(const StringRef& name, int32_t value) = 0;
	virtual void SetValueString(const StringRef& name, const String& value) = 0;

	virtual ISerializeObjectElement* AddObject(const StringRef& name) = 0;


	virtual bool TryGetValueInt32(const StringRef& name, int32_t* outValue) = 0;
	virtual bool TryGetValueString(const StringRef& name, String* outValue) = 0;

	virtual bool TryGetObject(const StringRef& name, ISerializeObjectElement** outValue) = 0;
};

/**
	@brief
*/
class ISerializeArrayElement
{
public:

};

/**
	@brief
*/
class ISerializationeStore
{
public:
	virtual ISerializeObjectElement* GetRootObject() = 0;
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

	Archive(ISerializationeStore* stream, ArchiveMode mode)
		: m_mode(mode)
		, m_stream(stream)
	{
		m_currentObject = m_stream->GetRootObject();
	}

	//template<class T>
	//void Serialze(T* obj)
	//{
	//	obj->lnsl_SerializeImpl(*this);
	//}

	template<class TRef>
	Archive& operator & (const NameValuePair<TRef>& nvp)
	{
		Process(nvp.name, nvp.value);
		return *this;
	}

private:

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
		ReadValue(name, value);
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
	template<typename T> void WriteValue(const StringRef& name, T & obj)	 // non]intrusive Object
	{
		auto* old = m_currentObject;
		m_currentObject = m_currentObject->AddObject(name);
		obj.lnsl_SerializeImpl(*this);
		m_currentObject = old;
	}


	void ReadValue(const StringRef& name, bool& value);
	void ReadValue(const StringRef& name, int8_t& value);
	void ReadValue(const StringRef& name, int16_t& value);
	void ReadValue(const StringRef& name, int32_t& value) { m_currentObject->TryGetValueInt32(name, &value); }	// TODO: Error
	void ReadValue(const StringRef& name, int64_t value);
	void ReadValue(const StringRef& name, uint8_t& value);
	void ReadValue(const StringRef& name, uint16_t& value);
	void ReadValue(const StringRef& name, uint32_t& value);
	void ReadValue(const StringRef& name, uint64_t& value);
	void ReadValue(const StringRef& name, float& value);
	void ReadValue(const StringRef& name, double& value);
	void ReadValue(const StringRef& name, String& value) { m_currentObject->TryGetValueString(name, &value); }	// TODO: Error
	template<typename T> void ReadValue(const StringRef& name, T & obj)	 // non]intrusive Object
	{
		ISerializeObjectElement* so;
		if (!m_currentObject->TryGetObject(name, &so)) return;

		auto* old = m_currentObject;
		m_currentObject = so;
		obj.lnsl_SerializeImpl(*this);
		m_currentObject = old;
	}

	ArchiveMode					m_mode;
	ISerializationeStore*		m_stream;
	ISerializeObjectElement*	m_currentObject;
};

} // namespace tr
LN_NAMESPACE_END
