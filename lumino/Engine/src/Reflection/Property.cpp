/*
	[2019/10/24] Property の必要性について
	----------
	UI モジュールを retaind で作っていることから要求が出てきている。

	まずゲームという毎フレーム画面が更新されるようなものは、常に Model が持っているフィールドを読み取らざるを得ないから、
	変更通知なんて不要なのでは？という考えがあるが、それは immediate で描画する場合。
	自分で drawText() とかする場合はこれでよい。

	なら retaind やめれば？なのであるが、エンドユーザー向けのUIのような「見た目」が重要な UI システムは imm だと厳しい。
	- imm ではアニメーションの実装が困難。
	- TextBox や TextBlock など、マルチラインなテキストレイアウトの仕組みを持つ表示は imm だと非常に難しいかパフォーマンスが大きく落ちる。

	で、retaind で行くなら毎フレーム setText() とかやるの？という話になるが、
	Lumino としては "別にやってもいい". それで特にパフォーマンスが落ちることはない。
	ただ、書くのが面倒。
	単純な TextBlock とかならまだしも、ListItem の中の Text を変更したりを考えるとすごく面倒。

	プロパティバインディングはパフォーマンス的にちょっとオーバーヘッド乗るけど、解決策としてはベターかなと。
*/
#include "Internal.hpp"
#include <LuminoEngine/Reflection/Property.hpp>
#include <LuminoEngine/Reflection/VMProperty.hpp>
#include "../../LuminoEngine/include/LuminoEngine/EngineContext.hpp"

namespace ln {

//==============================================================================
// TypeInfo

void TypeInfo::registerProperty(PropertyInfo* prop)
{
    if (LN_REQUIRE(!prop->m_registerd)) return;
    m_properties.add(prop);
    prop->m_registerd = true;
}

void TypeInfo::registerViewProperty(ViewPropertyInfo* prop)
{
	if (LN_REQUIRE(!prop->m_registerd)) return;
	m_viewProperties.add(prop);
	prop->m_registerd = true;
}

ViewPropertyInfo* TypeInfo::findViewProperty(const StringRef& name) const
{
	auto info = m_viewProperties.findIf([&](auto& x) { return x->name() == name; });
	if (info)
		return *info;
	else if (m_baseType)
		return m_baseType->findViewProperty(name);
	else
		return nullptr;
}

Ref<Object> TypeInfo::createInstance() const
{
	return m_factory(this);
}

Ref<Object> TypeInfo::createInstance(const String& typeName)
{
	if (TypeInfo* info = EngineContext::current()->findTypeInfo(typeName)) {
		return info->createInstance();
	}
	else {
        // TODO: Objcet.hpp の serialize の TODO にもあるが、切り替えできるようにしたい
        LN_LOG_WARNING << " Not found type.　(" << typeName << ")";
		return nullptr;
	}
}

TypeInfo* TypeInfo::getTypeInfo(const Object* obj)
{
	return obj->_lnref_getThisTypeInfo();
}

void TypeInfo::initializeObjectProperties(Object* obj)
{
    // TODO: ベースクラス
    TypeInfo* info = getTypeInfo(obj);
    if (info)
    {
        for (auto& propInfo : info->m_properties)
        {
			if (propInfo->m_getPropertyCallback)
			{
				if (PropertyBase* prop = propInfo->m_getPropertyCallback(obj)) {
					prop->m_owner = obj;
					prop->m_propertyInfo = propInfo;
				}
			}
        }
    }
}

//==============================================================================
// PropertyInfo

PropertyRef PropertyInfo::getPropertyRef(Object* obj, PropertyInfo* propertyInfo)
{
	return PropertyRef(obj, propertyInfo->m_accessor);
}

void PropertyInfo::notifyPropertyChanged(Object* ownerObject, PropertyBase* target, const PropertyInfo* prop, PropertySetSource source)
{
    if (prop->m_staticPropertyChangedCallback) {
        prop->m_staticPropertyChangedCallback(ownerObject);
    }
}

//==============================================================================
// PropertyPath

PropertyRef PropertyPath::findProperty(Object* root, const PropertyPath* path)
{
	class LocalVisitor : public ReflectionObjectVisitor
	{
	public:
		const PropertyPath* path;
		PropertyRef ref;
		virtual bool visitProperty(Object* obj, PropertyInfo* prop)
		{
			if (path->m_propertyName == prop->name()) {
				ref = PropertyRef(obj, prop->accessor());
				return true;
			}
			return false;
		}
	};

	LocalVisitor visitor;
	visitor.path = path;
	root->traverseRefrection(&visitor);
	return visitor.ref;
}

//==============================================================================
// EngineContext

bool ReflectionObjectVisitor::visitProperty(Object* obj, PropertyInfo* prop)
{
	return true;
}

} // namespace ln

