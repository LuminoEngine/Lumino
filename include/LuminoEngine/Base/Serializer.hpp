
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

} // namespace ln
