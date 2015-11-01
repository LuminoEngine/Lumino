
#pragma once
#include <map>
#include "../CoreObject.h"
#include "../RoutedEvent.h"
#include "../Variant.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

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
	@brief	GUI 要素のプロパティを共有します。
*/
class Style
	: public CoreObject
{
	typedef GenericCoreList<Trigger*>	PropertyTriggerList;
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:

	/**
		@brief	Style を作成します。
		@return	作成された Style。このオブジェクトは AutoReleasePool で管理されています。
	*/
	static Style* Create();

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

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
