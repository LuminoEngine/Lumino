
#pragma once
#include <functional>
#include "../Engine/Object.hpp"

namespace ln {
class AssetModel;

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

namespace detail {
class SerializerStore2;
}


#if 1
/** */
LN_CLASS()
class Serializer2
	: public Object
{
	LN_OBJECT;
public:

	bool isSaving() const { return m_mode == ArchiveMode::Save; }

	bool isLoading() const { return m_mode == ArchiveMode::Load; }


	void writeName(const StringRef& name);

	/** write */
	//LN_METHOD()
	void writeBool(bool value);

	void writeInt8(int8_t value);
	void writeInt16(int16_t value);
	void writeInt32(int32_t value);
	void writeInt64(int64_t value);
	void writeUInt8(uint8_t value);
	void writeUInt16(uint16_t value);
	void writeUInt32(uint32_t value);
	void writeUInt64(uint64_t value);
	void writeFloat(float value);
	void writeDouble(double value);

	/** write */
	//LN_METHOD()
	void writeInt(int value);


	/** write */
	//LN_METHOD()
	void writeString(const StringRef& value);



	/** write */
	//LN_METHOD()
	// beginObject, endObject のユーティリティ
	void writeObject(Object* value);

	//template<class T>
	//void writeList(const StringRef& name, const List<T>& value)
	//{
	//	beginWriteList(name);

	//	endWriteList();
	//}


	bool readName(const StringRef& name);

	//
	/** read */
	//LN_METHOD()
	bool readBool();

	int8_t readInt8();
	int16_t readInt16();
	int32_t readInt32();
	int64_t readInt64();
	uint8_t readUInt8();
	uint16_t readUInt16();
	uint32_t readUInt32();
	uint64_t readUInt64();
	float readFloat();
	double readDouble();

	/** read */
	//LN_METHOD()
	int readInt();

	/** read */
	//LN_METHOD()
	float readFloat(const StringRef& name);

	/** read */
	//LN_METHOD()
	String readString(const StringRef& name);

	/** read */
	//LN_METHOD()
	// beginObject, endObject のユーティリティ
	Ref<Object> readObject();



	/** sec */
	void beginWriteObject();
	void endWriteObject();
	void beginWriteList();
	void endWriteList();


	void beginReadObject();
	void endReadObject();
	bool beginReadList(int* outItemCount);
	void endReadList();

	
	/** serialize */
	//LN_METHOD()
	static String serialize(AssetModel* value, const String& basePath);
	
	/** serialize */
	//LN_METHOD()
	static Ref<AssetModel> deserialize(const String& str, const String& basePath);

LN_CONSTRUCT_ACCESS:
	Serializer2();
	virtual ~Serializer2() = default;
	void init();

private:
	void processSave();
	void processLoad();

	ArchiveMode m_mode;
	Ref<detail::SerializerStore2> m_store;
};
#endif

} // namespace ln
