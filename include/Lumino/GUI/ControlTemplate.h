
#pragma once
#include <map>
#include "../CoreObject.h"
#include "Common.h"
#include "../RoutedEvent.h"
#include "../Variant.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

class Binding
{
public:
	Binding() : m_path() {}
	Binding(const String& srcPropPath) : m_path(srcPropPath) {}
	~Binding() {}

private:
	String	m_path;
};

/**
	@brief		
	@note	WPF の FrameworkElementFactory に相当する。
*/
class UIElementFactory	// TODO: UIElement 以外も作れるように。名前変更。
	: public CoreObject
{
public:
	UIElementFactory(GUIManager* manager);
	virtual ~UIElementFactory();

	void SetKeyName(const String& name) { m_keyName = name; }

	/// この UIElementFactory が生成するインスタンスの型名
	void SetTypeName(const String& typeFullName) { m_targetTypeFullName = typeFullName; }

	// TODO 名前が紛らわしい。ElementValue とかに。
	void SetPropertyValue(const Property* prop, const Variant& value) { m_propertyValueList.SetValue(prop, value); }
	Variant GetPropertyValue(const Property* prop) const { return m_propertyValueList.GetValue(prop); }



	//void AddProperty(const String& propName, );
	//void AddTemplateBinding(const String& propName, );
	//void AddTemplateBinding(const String& propName, const Binding& binding)
	//{
	//}
	void AddTemplateBinding(const Property* prop, const String& srcPropPath)
	{
		PropertyInfo info;
		info.Kind = PropertyKind_TemplateBinding;
		info.SourcePropPath = srcPropPath;
		m_propertyInfoList.Add(prop, info);
	}


	void AddChild(UIElementFactory* child);


	// 以下2つはもともと1つだったが、子要素をビルドする前に親要素に追加し、ローカルリソースを更新する必要があったため分けた。
	// それだけなら分ける必要も無い気がするが、親要素によって追加先が異なる。
	// ルートなら SetTemplateChild()、プロパティなら SetValue()、リストなら AddItem() 等、色々ある。
	//CoreObject* CreateInstance();
	//void BuildInstance(CoreObject* element, UIElement* rootLogicalParent);
	// Setter 経由の場合は NULL が渡される。
	CoreObject* CreateInstance(UIElement* rootLogicalParent);


private:
	enum PropertyKind
	{
		PropertyKind_Variant = 0,		///< 普通の値
		PropertyKind_Binding,			///< Binding
		PropertyKind_TemplateBinding,	///< TemplateBinding
	};

	struct PropertyInfo
	{
		PropertyKind	Kind;
		//Binding			BindingInfo;	///< PropertyKind_Binding のときはこれを使う
		String			SourcePropPath;		///< PropertyKind_TemplateBinding のときはこれを使う
	};

	typedef SortedArray<const Property*, PropertyInfo>	PropertyInfoList;
	typedef SortedArray<const Property*, Variant>	PropertyValueList;

	GUIManager*						m_manager;	// CreateInstance() で必要。引数でもらってもいいかも？
	String							m_keyName;
	String							m_targetTypeFullName;
	PropertyInfoList				m_propertyInfoList;
	PropertyValueList				m_propertyValueList;
	Array<UIElementFactory*>		m_children;
};

// TODO: 削除予定。UIElementFactory のプロパティとして登録するものは全て GenericCoreList<Variant> でなければならない。
typedef GenericCoreList<Variant>	UIElementFactorylist;

/**
	@brief		
	@note	ContentPresenter が必要。
*/
class ControlTemplate
	: public CoreObject
{
public:
	ControlTemplate();
	virtual ~ControlTemplate();

	void SetTargetType(const String& fullTypeName) { m_targetType = fullTypeName; }
	const String&  GetTargetType() const { return m_targetType; }



	//void SetPropertyValue(const Property* prop, const Variant& value)
	//{
	//	m_propertyValueList.SetValue(prop, value);
	//}
	//Variant GetPropertyValue(const Property* prop) const { return m_propertyValueList.GetValue(prop); }
	//void SetPropertyValue(const String& propertyName, const Variant& value) { m_propertyValueList.SetValue(propertyName, value); }
	//Variant GetPropertyValue(const String& propertyName) const { return m_propertyValueList.GetValue(propertyName); }

	void SetVisualTreeRoot(UIElementFactory* factory) { m_visualTreeRoot = factory; }

	/// 指定された Control にこのテンプレートを適用します。
	void Apply(Control* control);

private:
	//typedef SortedArray<const Property*, Variant>	PropertyValueList;

	String						m_targetType;		///< 対象コントロール名 ("Button" 等)
	//PropertyValueList			m_propertyValueList;
	RefPtr<UIElementFactory>	m_visualTreeRoot;	///< テンプレートの VisualTree のルートノード
};

#if 0
/**
	@brief
*/
class DataTemplate
	: public CoreObject
{
public:
	DataTemplate();
	virtual ~DataTemplate();

	void SetVisualTreeRoot(UIElementFactory* factory) { m_visualTreeRoot = factory; }

	/// 指定された Control にこのテンプレートを適用します。
	void Apply(Control* control);

private:
	RefPtr<UIElementFactory>	m_visualTreeRoot;	///< テンプレートの VisualTree のルートノード

	// WPF ではデータの型とかも持つ。とりあえず ControlTemplate とは分けておく。
};
#endif

/**
	@brief		
	@note	
*/
//class LayoutTemplate
//	: public CoreObject
//{
//public:
//	ControlTemplate();
//	virtual ~ControlTemplate();
//
//private:
//	String				m_targetType;		///< 対象コントロール名 ("Button" 等)
//	UIElementFactory*	m_visualTreeRoot;	///< テンプレートの VisualTree のルートノード
//};


/**
	@brief		
*/
class Setter
	: public CoreObject
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	Setter();
	Setter(const Property* prop, const Variant& value);
	virtual ~Setter();

public:
	void SetProperty(const Property* prop) { m_property = prop; }
	const Property* GetProperty() const { return m_property; }
	void SetValue(const Variant& value) { m_value = value; }
	const Variant& GetValue() const { return m_value; }

private:
	const Property*		m_property;
	Variant				m_value;
};

typedef GenericCoreList<Setter*>		SetterList;


/**
	@brief		
*/
class TriggerBase
	: public CoreObject
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	TriggerBase();
	virtual ~TriggerBase();

public:	// internal
	/// routedEvent : 例えばサブクラス Trigger は PropertyChangedEvent を受け取ったら target にプロパティをセットする
	/// tareget : 
	//virtual void Invoke(const RoutedEvent* prop, const RoutedEventArgs* e, CoreObject* target) = 0;

	virtual bool IsPropertyTrigger() const { return false; }
};

typedef GenericCoreList<TriggerBase*>		TriggerList;

/**
	@brief		
*/
class Trigger	// TODO: 名前 PropertyTrigger のほうがわかりやすい気がする
	: public TriggerBase
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	Trigger();
	Trigger(const Property* prop, const Variant& value);
	virtual ~Trigger();

public:
	void SetProperty(const Property* prop) { m_property = prop; }
	const Property* GetProperty() const { return m_property; }
	void SetValue(const Variant& value) { m_value = value; }
	const Variant& GetValue() const { return m_value; }

	// ユーティリティ
	void AddSetter(const Property* prop, const Variant& value)
	{
		RefPtr<Setter> setter = RefPtr<Setter>::Create(prop, value);
		Setter* p = setter.GetObjectPtr();
		m_setterList->Add(p);
	}

	// internal
	/// オーナーオブジェクトのプロパティ変更通知が発生した
	void TryInvoke(CoreObject* target, PropertyChangedEventArgs* e);
protected:
	virtual bool IsPropertyTrigger() const { return true; }

private:
	const Property*		m_property;
	Variant				m_value;
	RefPtr<SetterList>	m_setterList;
};

/**
	@brief		
*/
class Style
	: public CoreObject
{
	typedef GenericCoreList<Trigger*>	PropertyTriggerList;
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	Style();
	virtual ~Style();

public:
	void SetTargetType(TypeInfo* type) { m_targetType = type; }
	TypeInfo* GetTargetType() const { return m_targetType; }
	void SetBasedOn(Style* style) { m_baseStyle = style; }
	Style* GetBasedOn() const { return m_baseStyle; }

	SetterList* GetSetters() const { return m_setterList; }
	//TriggerList* GetTriggers() const { return m_triggerList; }

	// ユーティリティ
	void AddSetter(const Property* prop, const Variant& value) 
	{
		auto setter = RefPtr<Setter>::Create(prop, value);
		m_setterList->Add(setter);
	}

	Trigger* AddPropertyTrigger(const Property* prop, const Variant& value)
	{
		auto trigger = RefPtr<Trigger>::Create(prop, value);
		m_propertyTriggerList->Add(trigger);
		return trigger;
	}


	/// 指定した要素にこのスタイルを適用する
	void Apply(UIElement* element);

	void NortifyTargetObjectPropertyChanged(CoreObject* target, PropertyChangedEventArgs* e);

	//void InvoleTriggers(const RoutedEvent* routedEvent, const RoutedEventArgs* e, CoreObject* target);

private:

	TypeInfo*			m_targetType;
	RefPtr<Style>		m_baseStyle;
	RefPtr<SetterList>	m_setterList;
	RefPtr<PropertyTriggerList>	m_propertyTriggerList;
	//RefPtr<TriggerList>	m_triggerList;
};

/**
	@brief		
*/
class ResourceDictionary
	: public CoreObject
{
public:
	ResourceDictionary();
	virtual ~ResourceDictionary();

	bool TryGetItem(const String& key, CoreObject** outObject);

	void AddItem(const String& key, CoreObject* obj);


	/// x:key が無く、TargetType でターゲットが指定されている ControlTemplate はこれで検索する (みつからなければ NULL)
	//bool TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate);
	Style* FindStyle(TypeInfo* type);

	//void AddControlTemplate(ControlTemplate* outTemplate);
	void AddStyle(Style* style);

private:
	typedef std::map<String, CoreObject*>	ItemMap;
	typedef std::pair<String, CoreObject*> ItemPair;
	ItemMap	m_items;

	//typedef std::map<String, ControlTemplate*>	ControlTemplateMap;
	//typedef std::pair<String, ControlTemplate*>	ControlTemplatePair;
	//ControlTemplateMap	m_controlTemplateMap;

	//typedef std::map<TypeInfo*, Style*>	StyleMap;
	//StyleMap	m_styleMap;
	Array<Style*>	m_styleList;	///< この ResourceDictionary に登録されている Style (map にはしない。TargetType が変わる可能性があるので)
};
	
/// UIElement は親 UIElement に追加れた時、親の CombinedLocalResource と
/// 自分の Resource プロパティの値を元に CombinedLocalResource を作る。
/// そのあと VisualTree の作成を行うが、その時はこの CombinedLocalResource から各種データを読み取る。
/// 自分の Resource が NULL であれば親の CombinedLocalResource をそのまま参照する。
class CombinedLocalResource
	: public RefObject
{
public:
	CombinedLocalResource();
	virtual ~CombinedLocalResource();

	void Combine(CombinedLocalResource* parent, ResourceDictionary* local);

	CoreObject* GetItem(const String& key);
	Style* FindStyle(TypeInfo* type);
	//bool TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate);

public:
	CombinedLocalResource*	m_parent;
	ResourceDictionary*		m_local;
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
