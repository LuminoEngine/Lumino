
#include "Internal.hpp"
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

#if 0
//==============================================================================
// Serializer2
LN_OBJECT_IMPLEMENT(Serializer2, Object) {}

Serializer2::Serializer2()
{
}

void Serializer2::init()
{
	Object::init();
}

void Serializer2::writeBool(const StringRef& name, bool value)
{
	LN_NOTIMPLEMENTED();
}

void Serializer2::writeInt(const StringRef& name, int value)
{
	LN_NOTIMPLEMENTED();
}

void Serializer2::writeFloat(const StringRef& name, float value)
{
	LN_NOTIMPLEMENTED();
}

void Serializer2::writeString(const StringRef& name, const StringRef& value)
{
	String v = value;
	LN_NOTIMPLEMENTED();
}

void Serializer2::writeObject(const StringRef& name, Object* value)
{
	Ref<Object> v;
	LN_NOTIMPLEMENTED();
}

bool Serializer2::readBool(const StringRef& name)
{
	bool value = false;
	LN_NOTIMPLEMENTED();
	return value;
}

int Serializer2::readInt(const StringRef& name)
{
	int value = 0;
	LN_NOTIMPLEMENTED();
	return value;
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

Ref<Object> Serializer2::readObject(const StringRef& name)
{
	Ref<Object> value;
	LN_NOTIMPLEMENTED();
	return value;
}

String Serializer2::serialize(Object* value, const String& basePath)
{
	return String::Empty;
}

Ref<Object> Serializer2::deserialize(const String& str, const String& basePath)
{
	return nullptr;
}
#endif

} // namespace ln

