
#pragma once
#include <map>
#include "Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

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

	/// この UIElementFactory が生成するインスタンスの型名
	void SetTypeName(const String& typeFullName) { m_targetTypeFullName = typeFullName; }


	//void AddProperty(const String& propName, );
	//void AddTemplateBinding(const String& propName, );
	//void AddTemplateBinding(const String& propName, const Binding& binding)
	//{
	//}
	void AddTemplateBinding(const String& propName, const String& srcPropPath)
	{
		PropertyInfo info;
		info.Kind = PropertyKind_TemplateBinding;
		info.SourcePropPath = srcPropPath;
		m_propertyInfoList.Add(propName, info);
	}


	void AddChild(UIElementFactory* child);


	UIElement* CreateInstance(UIElement* rootLogicalParent);

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

	typedef SortedArray<String, PropertyInfo>	PropertyInfoList;

	GUIManager*						m_manager;	// CreateInstance() で必要。引数でもらってもいいかも？
	String							m_targetTypeFullName;
	PropertyInfoList				m_propertyInfoList;
	ArrayList<UIElementFactory*>	m_children;
};

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

	void SetVisualTreeRoot(UIElementFactory* factory) { m_visualTreeRoot = factory; }

	/// 指定された Control にこのテンプレートを適用します。
	void Apply(Control* control);

private:
	String						m_targetType;		///< 対象コントロール名 ("Button" 等)
	RefPtr<UIElementFactory>	m_visualTreeRoot;	///< テンプレートの VisualTree のルートノード
};

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
class ResourceDictionary
	: public CoreObject
{
public:
	ResourceDictionary();
	virtual ~ResourceDictionary();

	bool TryGetItem(const String& key, CoreObject** outObject);

	void AddItem(const String& key, CoreObject* obj);


	/// x:key が無く、TargetType でターゲットが指定されている ControlTemplate はこれで検索する (みつからなければ NULL)
	bool TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate);

	void AddControlTemplate(ControlTemplate* outTemplate);

private:
	typedef std::map<String, CoreObject*>	ItemMap;
	typedef std::pair<String, CoreObject*> ItemPair;
	ItemMap	m_items;

	typedef std::map<String, ControlTemplate*>	ControlTemplateMap;
	typedef std::pair<String, ControlTemplate*>	ControlTemplatePair;
	ControlTemplateMap	m_controlTemplateMap;
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
	bool TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate);

public:
	CombinedLocalResource*	m_parent;
	ResourceDictionary*		m_local;
};

} // namespace GUI
} // namespace Lumino
