
#pragma once
#include <functional>
#include "../Reflection/Object.hpp"
#include "../Reflection/TypeInfo.hpp"

namespace ln {
class AssetModel;
class Serializer2;

namespace detail {

// void serialize(Archive& ar) をメンバ関数として持っているか
template<typename T>
class has_member_serialize2_function
{
private:
	template<typename U>
	static auto check(U&& v) -> decltype(v.serialize(*reinterpret_cast<Serializer2*>(0)), std::true_type());
	static auto check(...) -> decltype(std::false_type());

public:
	typedef decltype(check(std::declval<T>())) type;
	static bool const value = type::value;
};

// void serialize(Archive& ar) をメンバ関数として持っていないか
template<typename T>
class non_member_serialize2_function
{
private:
	template<typename U>
	static auto check(U&& v) -> decltype(v.serialize(*reinterpret_cast<Serializer2*>(0)), std::true_type());
	static auto check(...) -> decltype(std::false_type());

public:
	typedef decltype(check(std::declval<T>())) type;
	static bool const value = !type::value && !std::is_enum<T>::value;
};

} // namespace detail

template<typename T> void serialize(Serializer2& ar, List<T>& value);
template<typename T> void serialize(Serializer2& ar, Optional<T>& value);
void serialize(Serializer2& ar, Path& value);
void serialize(Serializer2& ar, detail::AssetPath& value);
void serialize(Serializer2& ar, Vector2& value);
void serialize(Serializer2& ar, Vector3& value);
void serialize(Serializer2& ar, Size& value);
void serialize(Serializer2& ar, Rect& value);

namespace detail {
class SerializerStore2;
}


#if 1
/** */
// Ref<List> はサポートしない。Ref<> は Object のみサポート。 Ref<Collection>　を使うこと。
LN_CLASS()
class Serializer2
	: public Object
{
	LN_OBJECT;
public:

	bool isSaving() const { return m_mode == ArchiveMode::Save; }

	bool isLoading() const { return m_mode == ArchiveMode::Load; }

	const detail::AssetPath& basePath() const { return m_basePath; }


	void writeName(const StringView& name);

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
	void writeNull();

	/** write */
	//LN_METHOD()
	void writeInt(int value);


	/** write */
	//LN_METHOD()
	void writeString(const StringView& value);



	/** write */
	//LN_METHOD()
	// beginObject, endObject のユーティリティ
	void writeObject(Object* value);

	//template<class T>
	//void writeList(const StringView& name, const List<T>& value)
	//{
	//	beginWriteList(name);

	//	endWriteList();
	//}


	bool readName(const StringView& name);

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
	float readFloat(const StringView& name);

	/** read */
	//LN_METHOD()
	String readString();

	/** read */
	//LN_METHOD()
	// beginObject, endObject のユーティリティ
	Ref<Object> readObject(Object* existing = nullptr);
	Ref<Object> readObjectInteral(std::function<Ref<Object>()> knownTypeCreator, Object* existing = nullptr);



	/** sec */
	void beginWriteObject();
	void endWriteObject();
	void beginWriteList();
	void endWriteList();


	void beginReadObject();
	void endReadObject();
	bool readingValueIsObject() const;
	bool readingValueIsNull() const;
	bool beginReadList(int* outItemCount);
	void endReadList();
	
	// beginWriteList と beginReadList のユーティリティ
	bool beginList(int* outItemCount);
	void endList();


	
	/** serialize */
	//LN_METHOD()
	static String serialize(AssetModel* value, const detail::AssetPath& basePath);
	
	/** serialize */
	//LN_METHOD()
	static Ref<AssetModel> deserialize(const String& str, const detail::AssetPath& basePath);

	static void deserializeInstance(AssetModel* asset, const String& str, const detail::AssetPath& basePath);


	// C++ utils

	template<typename T>
	Serializer2& operator&(T&& value)
	{
		process(std::forward<T>(value));
		return *this;
	}


	template<typename T>
	void process(T&& head)
	{
		switch (m_mode)
		{
		case ArchiveMode::Save:
			processSave(head);
			break;
		case ArchiveMode::Load:
			processLoad(head);
			break;
		default:
			LN_UNREACHABLE();
			break;
		}
	}

LN_CONSTRUCT_ACCESS:
	Serializer2();
	virtual ~Serializer2() = default;
	void init();

private:
	void processSave();
	void processLoad();

	template<typename T>
	void processSave(NameValuePair<T>& nvp)
	{
		writeName(nvp.name);
		writeValue(*nvp.value);
	}
	template<typename T>
	void processSave(T& value)
	{
		writeValue(value);
	}
	void writeValue(bool value) { writeBool(value); }
	void writeValue(int8_t value) { writeInt8(value); }
	void writeValue(int16_t value) { writeInt16(value); }
	void writeValue(int32_t value) { writeInt32(value); }
	void writeValue(int64_t value) { writeInt64(value); }
	void writeValue(uint8_t value) { writeUInt8(value); }
	void writeValue(uint16_t value) { writeUInt16(value); }
	void writeValue(uint32_t value) { writeUInt32(value); }
	void writeValue(uint64_t value) { writeUInt64(value); }
	void writeValue(float value) { writeFloat(value); }
	void writeValue(double value) { writeDouble(value); }
	void writeValue(String& value) { writeString(value); }
	template<typename T>
	void writeValue(Ref<T>& value) { writeObject(value); }
	template<typename T, typename std::enable_if<std::is_enum<T>::value, std::nullptr_t>::type = nullptr>
	void writeValue(T& value) { writeInt32((int)value); }
	template<typename T, typename std::enable_if<detail::has_member_serialize2_function<T>::value, std::nullptr_t>::type = nullptr>
	void writeValue(T& value) { value.serialize(*this); }
	template<typename T, typename std::enable_if<detail::non_member_serialize2_function<T>::value, std::nullptr_t>::type = nullptr>
	void writeValue(T& value) { ln::serialize(*this, value); }

	template<typename T>
	void processLoad(NameValuePair<T>& nvp)
	{
		if (readName(nvp.name)) {
			readValue(*nvp.value);
		}
	}
	template<typename T>
	void processLoad(T& value)
	{
		readValue(value);
	}
	void readValue(bool& outValue) { outValue = readBool(); }
	void readValue(int8_t& outValue) { outValue = readInt8(); }
	void readValue(int16_t& outValue) { outValue = readInt16(); }
	void readValue(int32_t& outValue) { outValue = readInt32(); }
	void readValue(int64_t& outValue) { outValue = readInt64(); }
	void readValue(uint8_t& outValue) { outValue = readUInt8(); }
	void readValue(uint16_t& outValue) { outValue = readUInt16(); }
	void readValue(uint32_t& outValue) { outValue = readUInt32(); }
	void readValue(uint64_t& outValue) { outValue = readUInt64(); }
	void readValue(float& outValue) { outValue = readFloat(); }
	void readValue(double& outValue) { outValue = readDouble(); }
	void readValue(String& outValue) { outValue = readString(); }
	template<typename T>
	void readValue(Ref<T>& outValue) { outValue = dynamic_pointer_cast<T>(readObjectInteral([]() -> Ref<Object> { return detail::makeObjectHelper<T>(); }, nullptr)); }
	template<typename T, typename std::enable_if<std::is_enum<T>::value, std::nullptr_t>::type = nullptr>
	void readValue(T& value) { value = (T)readInt32(); }
	template<typename T, typename std::enable_if<detail::has_member_serialize2_function<T>::value, std::nullptr_t>::type = nullptr>
	void readValue(T& outValue) { outValue.serialize(*this); }
	template<typename T, typename std::enable_if<detail::non_member_serialize2_function<T>::value, std::nullptr_t>::type = nullptr>
	void readValue(T& outValue) { ln::serialize(*this, outValue); }

	ArchiveMode m_mode;
	detail::AssetPath m_basePath;
	Ref<detail::SerializerStore2> m_store;
};
#endif

template<typename T>
void serialize(Serializer2& ar, List<T>& value)
{
	if (ar.isSaving()) {
		ar.beginWriteList();
		for (auto& v : value) {
			ar & v;
		}
		ar.endWriteList();
	}
	else {
		int size = 0;
		if (ar.beginReadList(&size)) {
			value.resize(size);
			for (auto& v : value) {
				ar & v;
			}
			ar.endReadList();
		}
	}
}

template<typename T>
void serialize(Serializer2& ar, Optional<T>& value)
{
	if (ar.isSaving()) {
		if (!value) {
			ar.writeNull();
		}
		else {
			T v = *value;
			ar & v;
		}
	}
	else {
		// ※そもそも key が存在しない場合は、値の serialize() 自体呼ばれない
		if (ar.readingValueIsNull()) {
			value = nullptr;
		}
		else {
			T v;
			ar & v;
			value = v;
		}
	}
}

inline void serialize(Serializer2& ar, Path& value)
{
	if (ar.isSaving()) {
		ar.writeString(value.str());
	}
	else {
		value = ar.readString();
	}
}

inline void serialize(Serializer2& ar, detail::AssetPath& value)
{
	if (ar.isSaving()) {
		ar.writeString(value.toString());
	}
	else {
		value = detail::AssetPath::parseAssetPath(ar.readString());
	}
}

inline void serialize(Serializer2& ar, Vector2& value)
{
	int size = 0;
	ar.beginList(&size);
	if (ar.isLoading()) {
		assert(size == 2);	// TODO: error handling
	}
	ar.process(value.x);
	ar.process(value.y);
	ar.endList();
}

inline void serialize(Serializer2& ar, Vector3& value)
{
	int size = 0;
	ar.beginList(&size);
	if (ar.isLoading()) {
		assert(size == 3);	// TODO: error handling
	}
	ar.process(value.x);
	ar.process(value.y);
	ar.process(value.z);
	ar.endList();
}

inline void serialize(Serializer2& ar, Size& value)
{
    int size = 0;
    if (ar.isSaving())
        ar.beginWriteList();
    else {
        ar.beginReadList(&size);
        assert(size == 2);	// TODO: error handling
    }

    ar.process(value.width);
    ar.process(value.height);

    if (ar.isSaving())
        ar.endWriteList();
    else
        ar.endReadList();
}

inline void serialize(Serializer2& ar, Rect& value)
{
    int size = 0;
    if (ar.isSaving())
        ar.beginWriteList();
    else {
        ar.beginReadList(&size);
        assert(size == 4);	// TODO: error handling
    }

    ar.process(value.x);
    ar.process(value.y);
    ar.process(value.width);
    ar.process(value.height);

    if (ar.isSaving())
        ar.endWriteList();
    else
        ar.endReadList();
}

} // namespace ln
