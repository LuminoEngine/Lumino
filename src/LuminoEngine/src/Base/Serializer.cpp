
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Asset/AssetObject.hpp>


namespace ln {

LN_OBJECT_IMPLEMENT(Serializer, Object) {
	context->registerType<Serializer>({});   // TODO: これ必須なのはちょっと忘れやすい・・
}

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


} // namespace ln

