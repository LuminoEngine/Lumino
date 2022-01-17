
#include "Internal.hpp"
//#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>
#include "../../LuminoEngine/include/LuminoEngine/EngineContext.hpp"


namespace ln {
	
//==============================================================================
// 

//using ljsonserializer = nlohmann::adl_serializer;
//using ljson = nlohmann::basic_json<std::map, std::vector, String, bool, int64_t, uint64_t, double, std::allocator, >;
//using ljson = nlohmann::json;

static std::string str_to_ns(const String& str) { return str.toStdString(); }
static String ns_to_str(const std::string& str) { return String::fromStdString(str); }

namespace detail {
class SerializerStore2 : public RefObject
{
public:
	enum class ContainerType
	{
		Unknown,
		List,
		Object,
	};

	struct StackItem
	{
		ContainerType containerType = ContainerType::Unknown;
		YAML::Node node;
		std::string name;
		int nextIndex = 0;

		bool writingPrimitiveOnly = true;	// write 時、list などを簡易表現にしたいか

		// node の直接の子ノード
		YAML::Node readingNode;
	};

	std::vector<StackItem> stack;
	std::string nextName;

	std::string m_error;

	StackItem& current() { return stack.back(); }
	StackItem& parent() { return stack[stack.size() - 2]; }

	//------------------------------
	// Write

	void initWrite()
	{
		stack.push_back(StackItem{ ContainerType::Object, YAML::Node() });
	}

	void pushWrite(ContainerType containerType)
	{
		stack.push_back(StackItem{ containerType, YAML::Node(), nextName });
		nextName = std::string();
	}

	void popWrite()
	{
		auto& current = stack[stack.size() - 1];
		auto& parent = stack[stack.size() - 2];
		

		if (current.containerType == SerializerStore2::ContainerType::List && current.writingPrimitiveOnly) {
			current.node.SetStyle(YAML::EmitterStyle::Flow);
		}
		else {
			//current.node.SetStyle(YAML::EmitterStyle::Default);
		}

		if (parent.containerType == SerializerStore2::ContainerType::Object) {
			parent.node[current.name] = std::move(current.node);
		}
		else if (parent.containerType == SerializerStore2::ContainerType::List) {
			parent.node.push_back(std::move(current.node));
		}
		else {
			LN_UNREACHABLE();
		}
		stack.pop_back();

		this->current().writingPrimitiveOnly = false;
	}

	template<typename T>
	void writePrimitive(T value)
	{
		if (current().containerType == SerializerStore2::ContainerType::Object)
			current().node[nextName] = value;
		else if (current().containerType == SerializerStore2::ContainerType::List) {
			current().node.push_back(value);
			//current().node.SetStyle(YAML::EmitterStyle::Flow);
		}
	}

	void writeString(const std::string& v)
	{
		if (current().containerType == SerializerStore2::ContainerType::Object) {
			current().node[nextName] = v;
		}
		else {
			LN_UNREACHABLE();
		}
	}

	void writeNull()
	{
		if (current().containerType == SerializerStore2::ContainerType::Object) {
			current().node[nextName] = YAML::Node(YAML::NodeType::Null);
		}
		else {
			current().node.push_back(YAML::Node(YAML::NodeType::Null));
		}
	}

	std::string str()
	{
		YAML::Emitter emitter;
		emitter << stack.front().node;
		return emitter.c_str();
	}


	//------------------------------
	// Read

	bool initRead(const std::string& text)
	{
		try {
			auto doc = YAML::Load(text);
			stack.push_back(StackItem{ ContainerType::Object, doc });
			return true;
		}
		catch (YAML::ParserException& e) {
			LN_LOG_ERROR("YAML: {}:{}:{}", e.mark.line, e.mark.column, e.msg);
			return false;
		}
	}

	bool pushRead(ContainerType requireContainerType, int* outItemCount)
	{
		//auto& p = parent();
		//p.containerType


		auto node = current().readingNode;



		if (requireContainerType == ContainerType::Object) {
			if (node.Type() != YAML::NodeType::Map) {
				reportError("Serializer expects an Object, but wasn't actual an Object.");
				return false;
			}
			stack.push_back(SerializerStore2::StackItem{ SerializerStore2::ContainerType::Object, node });
			if (outItemCount) *outItemCount = node.size();
			return true;
		}
		else if (requireContainerType == ContainerType::List) {
			if (node.Type() != YAML::NodeType::Sequence) {
				reportError("Serializer expects an List, but wasn't actual an List.");
				return false;
			}
			stack.push_back(SerializerStore2::StackItem{ SerializerStore2::ContainerType::List, node });
			if (outItemCount) *outItemCount = node.size();

			if (node.size() > 0) {
				current().readingNode = node[0];
			}

			return true;
		}
		else {
			LN_UNREACHABLE();
			return false;
		}
	}

	void popRead()
	{
		stack.pop_back();
		postRead();
	}

	bool readName(const std::string& v)
	{
		if (LN_REQUIRE(current().containerType == ContainerType::Object && current().node.Type() == YAML::NodeType::Map)) return false;

		auto node = current().node[v];
		if (node.Type() != YAML::NodeType::Null && node.Type() != YAML::NodeType::Undefined) {
			current().readingNode = node;
			return true;
		}
		else {
			return false;
		}
	}

	bool readFirstProperty(std::string* name)
	{
		if (LN_REQUIRE(current().containerType == ContainerType::Object && current().node.Type() == YAML::NodeType::Map)) return false;

		auto itr = current().node.begin();
		if (itr != current().node.end()) {
			auto t = itr->first.Type();
			*name = itr->first.as<std::string>();
			current().readingNode = itr->second;
			return true;
		}
		else {
			return false;
		}
	}

	template<typename T>
	void readPrimitive(T* value)
	{
		auto& c = current();
		*value = c.readingNode.as<T>();
		postRead();

		//if (c.containerType == ContainerType::Object) {
		//}
		//else if (c.containerType == ContainerType::List) {
		//	*value = c.readingNode.as<T>();
		//	c.nextIndex++;
		//	c.readingNode = c.node[c.nextIndex];
		//}
		//else {
		//	LN_UNREACHABLE();
		//}
	}

	void postRead()
	{
		auto& c = current();
		if (c.containerType == ContainerType::Object) {
			c.readingNode = YAML::Node();
			c.name.clear();
		}
		else if (c.containerType == ContainerType::List) {
			c.nextIndex++;
			c.readingNode = c.node[c.nextIndex];
		}
		else {
			LN_UNREACHABLE();
		}
	}

	void reportError(const char* message)
	{
		m_error = message;
	}
};
} // namespace detail



#if 1
//==============================================================================
// Serializer2

/*
	Struct の serialize について
	----------
	Binding からも呼べるようにしたいが、struct は共通のベースクラスとか無いので、FlatAPI を個別に作っていかないとならない。
	Serializer の API としては writeStruct() とか公開できないので、struct 側に serialize を実装する必要がある。

	struct Vector3
	{
		LN_METHOD()
		void srialize(Serializer* sr)
		{
		   sr->beginObject();	// ほとんどは Array になるかな
		   sr->writePropertyName("x");
		   sr->writeFloat(x);
		   sr->endObject();
		}
	}
*/

LN_OBJECT_IMPLEMENT(Serializer2, Object) {}

Serializer2::Serializer2()
{
	std::string::value_type;
	m_store = makeRef<detail::SerializerStore2>();
	//nlohmann::json j;
	//j[u"pi"] = 3.141;
	//j[u"happy"] = true;
	//j[u"name"] = u"Niels";
	//j[u"nothing"] = nullptr;
	//j[u"answer"][u"everything"] = 42;  // 存在しないキーを指定するとobjectが構築される
	//j[u"list"] = { 1, 0, 2 };         // [1,0,2]
	//j[u"object"] = { {u"currency", u"USD"}, {u"value", 42.99} };  // {"currentcy": "USD", "value": 42.99}

	//auto& j2 = m_store->root["test"];
	//j2["pi"] = 3.141;
	//auto s = m_store->root.dump();
	//std::cout << s << std::endl;
}

void Serializer2::init()
{
	Object::init();
}

void Serializer2::writeName(const StringView& name)
{
	m_store->nextName = str_to_ns(name);
}

void Serializer2::writeBool(bool value)
{
	if (LN_REQUIRE(isSaving())) return;
	m_store->writePrimitive<bool>(value);
}

void Serializer2::writeInt8(int8_t value)
{
	if (LN_REQUIRE(isSaving())) return;
	m_store->writePrimitive(static_cast<int>(value));
}

void Serializer2::writeInt16(int16_t value)
{
	if (LN_REQUIRE(isSaving())) return;
	m_store->writePrimitive(value);
}

void Serializer2::writeInt32(int32_t value)
{
	if (LN_REQUIRE(isSaving())) return;
	m_store->writePrimitive(value);
}

void Serializer2::writeInt64(int64_t value)
{
	if (LN_REQUIRE(isSaving())) return;
	m_store->writePrimitive(value);
}

void Serializer2::writeUInt8(uint8_t value)
{
	if (LN_REQUIRE(isSaving())) return;
	m_store->writePrimitive(static_cast<int>(value));
}

void Serializer2::writeUInt16(uint16_t value)
{
	LN_NOTIMPLEMENTED();	// unsigned 系は未サポート。yaml-cpp の中で encode に失敗する
	//if (LN_REQUIRE(isSaving())) return;
	//m_store->writePrimitive(value);
}

void Serializer2::writeUInt32(uint32_t value)
{
	LN_NOTIMPLEMENTED();	// unsigned 系は未サポート。yaml-cpp の中で encode に失敗する
	//if (LN_REQUIRE(isSaving())) return;
	//m_store->writePrimitive(value);
}

void Serializer2::writeUInt64(uint64_t value)
{
	LN_NOTIMPLEMENTED();	// unsigned 系は未サポート。yaml-cpp の中で encode に失敗する
	//if (LN_REQUIRE(isSaving())) return;
	//m_store->writePrimitive(value);
}

void Serializer2::writeFloat(float value)
{
	if (LN_REQUIRE(isSaving())) return;
	m_store->writePrimitive(value);
}

void Serializer2::writeDouble(double value)
{
	if (LN_REQUIRE(isSaving())) return;
	m_store->writePrimitive(value);
}

void Serializer2::writeNull()
{
	if (LN_REQUIRE(isSaving())) return;
	m_store->writeNull();
}

void Serializer2::writeInt(int value)
{
	if (LN_REQUIRE(isSaving())) return;
	m_store->writePrimitive<int>(value);
}

void Serializer2::writeString(const StringView& value)
{
	if (LN_REQUIRE(isSaving())) return;
	m_store->writePrimitive(str_to_ns(value));
}

void Serializer2::writeObject(Object* value)
{
	if (LN_REQUIRE(isSaving())) return;
#if 1
	if (value) {
		beginWriteObject();
		auto typeName = TypeInfo::getTypeInfo(value)->name();
		m_store->nextName = "class." + str_to_ns(typeName);
		beginWriteObject();
		static_cast<Object*>(value)->serialize(*this);
		endWriteObject();
		endWriteObject();
	}
	else {
		m_store->writeNull();
	}
#else
	beginWriteObject();
	auto typeName = TypeInfo::getTypeInfo(value)->name();
	m_store->nextName = ".class";
	m_store->writeString(str_to_ns(typeName));
	static_cast<Object*>(value)->onSerialize2(this);
	endWriteObject();
#endif
}

void Serializer2::beginWriteObject()
{
	m_store->pushWrite(detail::SerializerStore2::ContainerType::Object);
}

void Serializer2::endWriteObject()
{
	m_store->popWrite();
}

void Serializer2::beginWriteList()
{
	m_store->pushWrite(detail::SerializerStore2::ContainerType::List);
}

void Serializer2::endWriteList()
{
	m_store->popWrite();
}

void Serializer2::beginReadObject()
{
	//m_store->stack.push_back(detail::SerializerStore2::StackItem{ detail::SerializerStore2::ContainerType::Object, *(m_store->current().readPos) });
	int itemCount = 0;
	m_store->pushRead(detail::SerializerStore2::ContainerType::Object, &itemCount);
}

void Serializer2::endReadObject()
{
	m_store->popRead();
}

bool Serializer2::readingValueIsObject() const
{
	return m_store->current().readingNode.IsMap();
}

bool Serializer2::readingValueIsNull() const
{
	return m_store->current().readingNode.IsNull();
}

bool Serializer2::beginReadList(int* outItemCount)
{
	return m_store->pushRead(detail::SerializerStore2::ContainerType::List, outItemCount);
}

void Serializer2::endReadList()
{
	m_store->popRead();
}

bool Serializer2::beginList(int* outItemCount)
{
	if (isSaving()) {
		beginWriteList();
		return true;
	}
	else {
		return beginReadList(outItemCount);
	}
}

void Serializer2::endList()
{
	if (isSaving()) {
		endWriteList();
	}
	else {
		endReadList();
	}
}

bool Serializer2::readName(const StringView& name)
{
	if (isSaving()) {
		LN_NOTIMPLEMENTED();
		return true;
	}
	else {
		return m_store->readName(str_to_ns(name));
	}
}

bool Serializer2::readBool()
{
	bool v = false;
	m_store->readPrimitive(&v);
	return v;
}

int8_t Serializer2::readInt8()
{
	int v = 0;
	m_store->readPrimitive(&v);
	return v;
}

int16_t Serializer2::readInt16()
{
	int16_t v = 0;
	m_store->readPrimitive(&v);
	return v;
}

int32_t Serializer2::readInt32()
{
	int32_t v = 0;
	m_store->readPrimitive(&v);
	return v;
}

int64_t Serializer2::readInt64()
{
	int64_t v = 0;
	m_store->readPrimitive(&v);
	return v;
}

uint8_t Serializer2::readUInt8()
{
	int v = 0;
	m_store->readPrimitive(&v);
	return v;
}

uint16_t Serializer2::readUInt16()
{
	uint16_t v = 0;
	m_store->readPrimitive(&v);
	return v;
}

uint32_t Serializer2::readUInt32()
{
	uint32_t v = 0;
	m_store->readPrimitive(&v);
	return v;
}

uint64_t Serializer2::readUInt64()
{
	uint64_t v = 0;
	m_store->readPrimitive(&v);
	return v;
}

float Serializer2::readFloat()
{
	float v = 0;
	m_store->readPrimitive(&v);
	return v;
}

double Serializer2::readDouble()
{
	double v = 0;
	m_store->readPrimitive(&v);
	return v;
}

int Serializer2::readInt()
{
	int v = 0;
	m_store->readPrimitive(&v);
	//return m_store->current().readPos->get<int>();
	return v;
}

float Serializer2::readFloat(const StringView& name)
{
	float value = 0.0;
	LN_NOTIMPLEMENTED();
	return value;
}

String Serializer2::readString()
{
	std::string v;
	m_store->readPrimitive(&v);
	return ns_to_str(v);
}

Ref<Object> Serializer2::readObject(Object* existing)
{
	return readObjectInteral(nullptr, existing);
}

Ref<Object> Serializer2::readObjectInteral(std::function<Ref<Object>()> knownTypeCreator, Object* existing)
{
	if (LN_REQUIRE(isLoading())) return nullptr;

#if 1
	beginReadObject();

	std::string typeName;
	Ref<Object> obj;
	if (existing) {
		obj = existing;

		// 型が既知なので不要だが、1度でも子ノードにアクセスしておかないと cpp-yaml が正しく孫ノードを呼んでくれなかった
		m_store->readFirstProperty(&typeName);
	}
	else {
		if (m_store->readFirstProperty(&typeName)) {

			if (typeName == "class.Object" && knownTypeCreator != nullptr) {
				// save 時に Lumino の 型システムに登録されていないものは Object 型として保存される。
				// ただ、Load 時に template として型が既知の場合はそちらを使用したいが、Object の場合は
				// 完全にインスタンスの生成を既知の型から行うようにしてみる。
				obj = knownTypeCreator();
			}

			if (!obj) {
				obj = TypeInfo::createInstance(String::fromStdString(typeName.substr(6)));
			}
		}

		// fallback
		if (!obj) {
			obj = makeObject<Object>();
		}
	}

	beginReadObject();
	obj->serialize(*this);
	endReadObject();

	endReadObject();
	return obj;
#else
	beginReadObject();

	Ref<Object> obj;

	if (m_store->readName(".class")) {
		std::string typeName;
		m_store->readPrimitive(&typeName);
		if (!typeName.empty()) {
			obj = TypeInfo::createInstance(String::fromStdString(typeName));
		}
	}

	// fallback
	if (!obj) {
		obj = makeObject<Object>();
	}

	obj->onSerialize2(this);
	endReadObject();
	return obj;
#endif
}

String Serializer2::serialize(AssetModel* value, const detail::AssetPath& basePath)
{
	//YAML::Node n;
	//n["a"] = 10;
	//n["b"] = "B";
	//YAML::Node n2;
	//n2["c"] = "C";
	//n["n2"] = n2;
	//n2["d"] = "DD";

	//YAML::Node n3 = n["n2"];
	//n3["d"] = "D3";
	//YAML::Emitter emitter;
	//emitter << n;
	//auto text = emitter.c_str();
	////auto a = YAML::LoadFile("D:/tmp/test.yml");
	////auto t = a.Type();
	////auto b = a["aaa"];

	if (LN_REQUIRE(value)) return String::Empty;
	auto sr = makeObject<Serializer2>();
	sr->m_mode = ArchiveMode::Save;
	sr->m_basePath = basePath;
	sr->m_store->initWrite();
	static_cast<Object*>(value)->serialize(*sr);
	LN_ENSURE(sr->m_store->stack.size() == 1);
	return ns_to_str(sr->m_store->str());
}

Ref<AssetModel> Serializer2::deserialize(const String& str, const detail::AssetPath& basePath)
{
	Ref<AssetModel> asset;

	try {
		ObjectInitializeContext::Default->autoAdd = false;

		auto sr = makeObject<Serializer2>();
		sr->m_mode = ArchiveMode::Load;
		sr->m_basePath = basePath;
		sr->m_store->initRead(str_to_ns(str));
		//sr->m_store->stack.push_back(detail::SerializerStore2::StackItem{ detail::SerializerStore2::ContainerType::Object, ljson::parse(str.toStdString()) });
		auto asset = makeObject<AssetModel>();
		static_cast<Object*>(asset)->serialize(*sr);

		ObjectInitializeContext::Default->autoAdd = true;
		return asset;
	}
	catch (...) {
		ObjectInitializeContext::Default->autoAdd = true;
		throw;
	}
}

void Serializer2::deserializeInstance(AssetModel* asset, const String& str, const detail::AssetPath& basePath)
{
	try {
		ObjectInitializeContext::Default->autoAdd = false;

		asset->m_externalObjectDeserialization = true;

		auto sr = makeObject<Serializer2>();
		sr->m_mode = ArchiveMode::Load;
		sr->m_basePath = basePath;
		sr->m_store->initRead(str_to_ns(str));
		//sr->m_store->stack.push_back(detail::SerializerStore2::StackItem{ detail::SerializerStore2::ContainerType::Object, ljson::parse(str.toStdString()) });
		static_cast<Object*>(asset)->serialize(*sr);

		ObjectInitializeContext::Default->autoAdd = true;
	}
	catch (...) {
		ObjectInitializeContext::Default->autoAdd = true;
		throw;
	}
}
#endif

} // namespace ln

