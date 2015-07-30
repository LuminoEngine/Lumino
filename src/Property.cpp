/*
	[2015/7/30] LN_PROPERTY_BEGIN と LN_PROPERTY_END はやっぱりやめる
		ベースクラスのプロパティにアクセスしづらくなる。
		例えば StackPanel::Properties::Size と書けない。

	[2015/7/29] LN_PROPERTY_BEGIN と LN_PROPERTY_END について
		これらはプロパティ定義を内部構造体に閉じ込めるもの。
		C# と違い、obj-> と書いてアクセスできたり、インテリセンスを汚したりと
		C++ では WPF のような定義はちょっと向かないと思ったのでこうしている。
*/
/*
	プロパティの種類は
	- 実体が C++ ネイティブのメンバ変数
	- 実体が外部(C#とか) のメンバ変数	→ 高速化を狙うときに使う。必須ではない
	- 実体が map に登録される Variant
	と、それぞれ
	- 普通のプロパティ
	- 添付プロパティ
*/
#include "Internal.h"
#include <Lumino/Variant.h>
#include <Lumino/Property.h>

namespace Lumino
{

//=============================================================================
// PropertyManager
//=============================================================================

PropertyManager::PropertyMap	PropertyManager::m_propertyMap;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/*AttachedProperty* */ void PropertyManager::RegisterAttachedProperty(TypeInfo* ownerClass, const String& propertyName, const Variant& defaultValue)
{
#if 0
	TypedNameKey key(ownerClass, propertyName);
	PropertyMap::iterator itr = m_propertyMap.find(key);
	if (itr == m_propertyMap.end())
	{
		std::shared_ptr<AttachedProperty> prop(LN_NEW AttachedProperty(propertyName, defaultValue));
		m_propertyMap[key] = prop;
		return prop.get();
	}
	return itr->second.get();
#endif;
}

} // namespace Lumino
