
#pragma once
#include <functional>
#include "../Engine/Object.hpp"

namespace ln {

/** */
LN_CLASS()
class Serializer
	: public Object
{
	LN_OBJECT;
public:
	
	/** write */
	LN_METHOD()
	void writeBool(const StringRef& name, bool value);

	/** write */
	LN_METHOD()
	void writeInt(const StringRef& name, int value);

	/** write */
	LN_METHOD()
	void writeFloat(const StringRef& name, float value);

	/** write */
	LN_METHOD()
	void writeString(const StringRef& name, const StringRef& value);

	/** write */
	LN_METHOD()
	void writeObject(const StringRef& name, Object* value);

	
	/** read */
	LN_METHOD()
	bool readBool(const StringRef& name);

	/** read */
	LN_METHOD()
	int readInt(const StringRef& name);

	/** read */
	LN_METHOD()
	float readFloat(const StringRef& name);

	/** read */
	LN_METHOD()
	String readString(const StringRef& name);

	/** read */
	LN_METHOD()
	Ref<Object> readObject(const StringRef& name);

	
	/** serialize */
	LN_METHOD()
	static String serialize(Object* value, const String& basePath);
	
	/** serialize */
	LN_METHOD()
	static Ref<Object> deserialize(const String& str, const String& basePath);

LN_CONSTRUCT_ACCESS:
	Serializer();
	virtual ~Serializer() = default;
	void init(/*ArchiveMode mode, const String& basePath*/);

private:
	void processSave();
	void processLoad();

	std::unique_ptr<Archive> m_archive;
};




#if 0
/** */
LN_CLASS()
class Serializer2
	: public Object
{
	LN_OBJECT;
public:
	
	/** write */
	//LN_METHOD()
	void writeBool(const StringRef& name, bool value);

	/** write */
	//LN_METHOD()
	void writeInt(const StringRef& name, int value);

	/** write */
	//LN_METHOD()
	void writeFloat(const StringRef& name, float value);

	/** write */
	//LN_METHOD()
	void writeString(const StringRef& name, const StringRef& value);

	/** write */
	//LN_METHOD()
	// beginObject, endObject のユーティリティ
	void writeObject(const StringRef& name, Object* value);

	
	/** read */
	//LN_METHOD()
	bool readBool(const StringRef& name);

	/** read */
	//LN_METHOD()
	int readInt(const StringRef& name);

	/** read */
	//LN_METHOD()
	float readFloat(const StringRef& name);

	/** read */
	//LN_METHOD()
	String readString(const StringRef& name);

	/** read */
	//LN_METHOD()
	// beginObject, endObject のユーティリティ
	Ref<Object> readObject(const StringRef& name);



	/** sec */
	void beginWriteObject(const StringRef& name, Object* value);
	void endWriteObject();
	void beginWriteList(const StringRef& name);
	void endWriteList();


	
	/** serialize */
	//LN_METHOD()
	static String serialize(Object* value, const String& basePath);
	
	/** serialize */
	//LN_METHOD()
	static Ref<Object> deserialize(const String& str, const String& basePath);

LN_CONSTRUCT_ACCESS:
	Serializer2();
	virtual ~Serializer2() = default;
	void init();

private:
	void processSave();
	void processLoad();
};
#endif

} // namespace ln
