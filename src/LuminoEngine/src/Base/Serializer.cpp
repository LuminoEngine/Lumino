
#include "Internal.hpp"
//#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>


namespace ln {
	
//==============================================================================
// Serializer
LN_OBJECT_IMPLEMENT(Serializer, Object) {}

Serializer::Serializer()
{
}

void Serializer::init(/*ArchiveMode mode, const String& basePath*/)
{
	Object::init();

	//switch (mode)
	//{
	//case ln::ArchiveMode::Save:
	//{
	//	break;
	//}
	//case ln::ArchiveMode::Load:
	//	break;
	//default:
	//	LN_UNREACHABLE();
	//	break;
	//}
}

void Serializer::writeBool(const StringRef& name, bool value)
{
	(*m_archive) & ln::makeNVP(name, value);
}

void Serializer::writeInt(const StringRef& name, int value)
{
	(*m_archive) & ln::makeNVP(name, value);
}

void Serializer::writeFloat(const StringRef& name, float value)
{
	(*m_archive) & ln::makeNVP(name, value);
}

void Serializer::writeString(const StringRef& name, const StringRef& value)
{
	String v = value;
	(*m_archive) & ln::makeNVP(name, v);
}

void Serializer::writeObject(const StringRef& name, Object* value)
{
	Ref<Object> v;
	(*m_archive) & ln::makeNVP(name, v);
}

bool Serializer::readBool(const StringRef& name)
{
	bool value = false;
	(*m_archive) & ln::makeNVP(name, value);
	return value;
}

int Serializer::readInt(const StringRef& name)
{
	int value = 0;
	(*m_archive) & ln::makeNVP(name, value);
	return value;
}

float Serializer::readFloat(const StringRef& name)
{
    float value = 0.0;
	(*m_archive) & ln::makeNVP(name, value);
	return value;
}

String Serializer::readString(const StringRef& name)
{
	String value;
	(*m_archive) & ln::makeNVP(name, value);
	return value;
}

Ref<Object> Serializer::readObject(const StringRef& name)
{
	Ref<Object> value;
	(*m_archive) & ln::makeNVP(name, value);
	return value;
}

String Serializer::serialize(Object* value, const String& basePath)
{
	auto asset = makeObject<AssetModel>(value);

	auto ptr = makeObject<Serializer>();
	auto ar = std::make_unique<JsonTextOutputArchive>();
	ar->setBasePath(basePath);
	ar->save(*asset);
	ptr->m_archive = std::move(ar);
	return ar->toString(JsonFormatting::Indented);
}

Ref<Object> Serializer::deserialize(const String& str, const String& basePath)
{
	auto asset = makeObject<AssetModel>();

	JsonTextInputArchive ar(str);
	ar.setBasePath(basePath);
	ar.load(*asset);

	return asset->target();
}

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
	};

	std::vector<StackItem> stack;
	std::string nextName;

	StackItem& current() { return stack.back(); }

	void initWrite()
	{
		stack.push_back(StackItem{ ContainerType::Object, YAML::Node() });
	}

	void pushWrite(ContainerType containerType)
	{
		stack.push_back(StackItem{ ContainerType::Object, YAML::Node(), nextName });
		nextName = std::string();
	}

	void popWrite()
	{
		auto& current = stack[stack.size() - 1];
		auto& parent = stack[stack.size() - 2];
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

	std::string str()
	{
		YAML::Emitter emitter;
		emitter << stack.front().node;
		return emitter.c_str();
	}


	void popRead()
	{
	}
};
} // namespace detail



#if 1
//==============================================================================
// Serializer2
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

void Serializer2::writeBool(const StringRef& name, bool value)
{
	if (LN_REQUIRE(isSaving())) return;
	LN_NOTIMPLEMENTED();
}

void Serializer2::writeInt(int value)
{
	if (LN_REQUIRE(isSaving())) return;
	//if (m_store->stack.back().containerType == detail::SerializerStore2::ContainerType::Object)
	//	m_store->stack.back().value[str_to_ns(m_store->nextName)] = value;
	//else if (m_store->stack.back().containerType == detail::SerializerStore2::ContainerType::List)
	//	m_store->stack.back().value.push_back(value);
}

void Serializer2::writeFloat(const StringRef& name, float value)
{
	if (LN_REQUIRE(isSaving())) return;
	LN_NOTIMPLEMENTED();
}

void Serializer2::writeString(const StringRef& name, const StringRef& value)
{
	if (LN_REQUIRE(isSaving())) return;
	String v = value;
	LN_NOTIMPLEMENTED();
}

void Serializer2::writeName(const StringRef& name)
{
	m_store->nextName = str_to_ns(name);
}

void Serializer2::writeObject(Object* value)
{
	if (LN_REQUIRE(isSaving())) return;
	beginWriteObject();
	auto typeName = TypeInfo::getTypeInfo(value)->name();
	m_store->nextName = "__type";
	m_store->writeString(str_to_ns(typeName));
	static_cast<Object*>(value)->onSerialize2(this);
	endWriteObject();
}

void Serializer2::beginWriteObject()
{
	m_store->pushWrite(detail::SerializerStore2::ContainerType::Object);
	//m_store->stack.push_back(detail::SerializerStore2::StackItem{ detail::SerializerStore2::ContainerType::Object, ljson(), m_store->nextName });
}

void Serializer2::endWriteObject()
{
	m_store->popWrite();
}

void Serializer2::beginWriteList()
{
	//m_store->stack.push_back(detail::SerializerStore2::StackItem{ detail::SerializerStore2::ContainerType::List, ljson(), m_store->nextName });
}

void Serializer2::endWriteList()
{
	m_store->popWrite();
}

void Serializer2::beginReadObject()
{
	//m_store->stack.push_back(detail::SerializerStore2::StackItem{ detail::SerializerStore2::ContainerType::Object, *(m_store->current().readPos) });
}

void Serializer2::endReadObject()
{
	m_store->popRead();
}

bool Serializer2::setName(const StringRef& name)
{
	if (isSaving()) {
		LN_NOTIMPLEMENTED();
		return true;
	}
	else {
		//auto& c = m_store->current();
		//c.readPos =c.value.find(str_to_ns(name));
		//return c.readPos != c.value.end();
		return false;
	}
}

bool Serializer2::readBool(const StringRef& name)
{
	bool value = false;
	LN_NOTIMPLEMENTED();
	return value;
}

int Serializer2::readInt()
{
	//return m_store->current().readPos->get<int>();
	return 0;
}

float Serializer2::readFloat(const StringRef& name)
{
	float value = 0.0;
	LN_NOTIMPLEMENTED();
	return value;
}

String Serializer2::readString(const StringRef& name)
{
	String value;
	LN_NOTIMPLEMENTED();
	return value;
}

Ref<Object> Serializer2::readObject()
{
	//if (LN_REQUIRE(isLoading())) return nullptr;
	//beginReadObject();
	//
	//Ref<Object> obj;
	//auto s = m_store->current().value.find("__type");
	//if (s != m_store->current().value.end()) {
	//	auto typeName = ns_to_str((*s).get<std::string>());
	//	if (!typeName.isEmpty()) {
	//		obj = TypeInfo::createInstance(typeName);
	//	}
	//}

	//// fallback
	//if (!obj) {
	//	obj = makeObject<Object>();
	//}

	//obj->onSerialize2(this);
	//endWriteObject();
	//return obj;
	return nullptr;
}

String Serializer2::serialize(AssetModel* value, const String& basePath)
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
	sr->m_store->initWrite();
	static_cast<Object*>(value)->onSerialize2(sr);
	LN_ENSURE(sr->m_store->stack.size() == 1);
	return ns_to_str(sr->m_store->str());
}

Ref<AssetModel> Serializer2::deserialize(const String& str, const String& basePath)
{
	auto sr = makeObject<Serializer2>();
	sr->m_mode = ArchiveMode::Load;
	//sr->m_store->stack.push_back(detail::SerializerStore2::StackItem{ detail::SerializerStore2::ContainerType::Object, ljson::parse(str.toStdString()) });
	auto asset = makeObject<AssetModel>();
	static_cast<Object*>(asset)->onSerialize2(sr);
	return asset;
}
#endif

} // namespace ln

