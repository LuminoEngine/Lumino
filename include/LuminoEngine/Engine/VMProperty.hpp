﻿/*
	Use case
	----------
	Ruby:
		# Model-View の Binding
		@prop1 = IntProperty.new(100)
		@text_block.bind(UITextBlock.text_property_id, @prop1)	# idea.1
		@text_block.bind("text", @prop1)						# idea.2 こっちの方が結果的にすっきりするか…
		@prop1.value = 1
		p @prop1.value

		# View-View の Binding
		# チェックボタンでの要素の表示/非表示
		view_prop = @text_block.get_property("visible")
		@toggle_button.bind("checked", view_prop)
		# view_prop への直接 get,set は不可能. variant での get,set なら ok.

	ViewProperty
	----------
	UIElement などが持つ Property.
	主に Engine を実装する人が使う。
	データサイズは極力小さくしたい。
	外に公開する ViewProperty のインスタンスも遅延で作りたい。
	データ本体は ViewProperty の外側で持ちたい。スタイルなどが主。ViewProperty は getter, setter のラッパーとして機能するような感じ。

	ViewPropertyInfo を static で作る。
	ViewPropertyInfo には WPF 同様 static な getter, setter を登録するが、これらはマクロで普通の getter, setter を呼び出すようにする。

	実装するときは setWidth() 等の中で notifyPropertyChanged(ViewPropertyInfo*) を呼ぶようにする。

	インスタンスは ln::Object が持たざるを得ないかな。

	ViewPropertyInfo は TypeInfo に持たせてもいいが、必須ではない。static 定数でいい。

	ModelProperty
	----------
	データクラスが持つ Property.
	主にユーザーコードで使用する。
*/
#pragma once

#include "../Base/Variant.hpp"

namespace ln {
class TypeInfo;
class ViewPropertyAccessor;

class ObservablePropertyBase;

// ObservableProperty 自体は new しなくても使えるようにしたいが、
// そうすると他で Ref で参照を持つことはできなくなるので、弱参照の仕組みを使う。
class ObservablePropertyRef
{
public:
	ObservablePropertyRef()
		: m_prop()
	{}

	ObservablePropertyRef(ObservablePropertyBase* prop)
		: m_prop(prop)
	{}

	bool isAlive() const
	{
		return m_prop.isAlive();
	}

	Ref<ObservablePropertyBase> resolve() const
	{
		auto ptr = m_prop.resolve();
		if (ptr != nullptr) {
			return ptr;
		}
		else {
			return nullptr;
		}
	}

private:
	WeakRefPtr<ObservablePropertyBase> m_prop;
};

// これは値の管理は行わない。派生側で。
class ObservablePropertyBase : public Object
{
public:
	ObservablePropertyBase()
		: m_bindingSource()
		, m_syncing(false)
	{}

	~ObservablePropertyBase() {
		if (auto s = m_bindingSource.resolve()) {
			s->m_changed = nullptr;
		}
	}

	// bindingSource の変更を購読する
	void bind(ObservablePropertyBase* bindingSource)
	{
		if (LN_REQUIRE(!bindingSource->m_changed)) return;
		m_bindingSource = bindingSource;
		bindingSource->m_changed = ln::bind(this, &ObservablePropertyBase::handleSourceChanged);
	}

protected:
	void notifyChanged()
	{
		if (m_syncing) return;	// TODO: setValue で封印した方がいいかも
		m_syncing = true;
		if (m_changed) {
			m_changed(this);
		}
		syncToSource();
		m_syncing = false;
	}

	void syncToSource()
	{
		//if (auto s = m_bindingSource.resolve()) {
		//	if (!s->m_syncing) {
		//		s->setValue(value);
		//	}
		//}
	}

	void handleSourceChanged(const ObservablePropertyBase* value)
	{
	}

private:
	ObservablePropertyRef m_bindingSource;
	std::function<void(const ObservablePropertyBase*)> m_changed;
	bool m_syncing;
};

// TODO: 名前を普通の Property にしたいところ。
// Binding に公開するときは IntProperty などに派生する。
template<class TValue>
class ModelProperty : public ObservablePropertyBase
{
public:
	ModelProperty()
		: m_value()
	{}

	ModelProperty(const TValue& value)
		: m_value(makeVariant(value))
	{}

	TValue get() const
	{
		return m_value->get<TValue>();
	}

	void set(const TValue& value)
	{
		m_value = makeVariant(value);	// TODO: 毎か作るのやめて、既存 instance があれば Variant::assign でやりたい
		notifyChanged();
	}

	//virtual const Variant* getValue() const override
	//{
	//	return m_value;
	//}

	//virtual void setValue(const Variant* value) override
	//{
	//	m_value = makeVariant(*value);	// TODO: convert
	//	notifyChanged();
	//}

protected:

private:
	Ref<Variant> m_value;
};


class ViewProperty : public ObservablePropertyBase
{
public:
	ViewPropertyInfo* m_info = nullptr;

private:
};

class ViewPropertyInfo : public RefObject
{
public:
	ViewPropertyInfo(const char* name, const Ref<ViewPropertyAccessor>& accessor)
		: m_name(String::fromCString(name))
		, m_accessor(accessor)
		, m_registerd(false)
	{}

	const String& name() const { return m_name; }

	const Ref<ViewPropertyAccessor>& accessor() const { return m_accessor; }

private:
	String m_name;
	Ref<ViewPropertyAccessor> m_accessor;
	bool m_registerd;

	friend class TypeInfo;
};


class ViewPropertyAccessor : public RefObject
{
public:
	virtual void getValue(const Object* obj, Ref<Variant>* value) const = 0;
	virtual void setValue(Object* obj, const Ref<Variant>& value) = 0;
};

template<class TClassType, class TValue, class TGetFunction, class TSetFunction>
class ViewPropertyAccessorImpl : public ViewPropertyAccessor
{
public:
	ViewPropertyAccessorImpl(TGetFunction getFunction, TSetFunction setFunction)
		: m_getFunction(getFunction)
		, m_setFunction(setFunction)
	{ }

	void getValue(const Object* obj, Ref<Variant>* value) const override
	{
		LN_DCHECK(obj);
		LN_DCHECK(value);
		const auto classPtr = static_cast<const TClassType*>(obj);
		TValue t;
		m_getFunction(classPtr, &t);
		*value = makeVariant(t);	// DOTO: Variant Pool
	}

	void setValue(Object* obj, const Ref<Variant>& value) override
	{
		LN_DCHECK(obj);
		auto classPtr = static_cast<TClassType*>(obj);
		m_setFunction(classPtr, value->get<TValue>());
	}

private:
	TGetFunction m_getFunction;
	TSetFunction m_setFunction;
};

template<class TClassType, class TValue, class TGetFunction, class TSetFunction>
Ref<ViewPropertyAccessor> makeViewPropertyAccessor(TGetFunction getFunction, TSetFunction setFunction)
{
	return Ref<ViewPropertyAccessor>(LN_NEW ViewPropertyAccessorImpl<TClassType, TValue, TGetFunction, TSetFunction>(getFunction, setFunction), false);
}

#define LN_MAKE_VIEW_PROPERTY_ACCESSOR(className, typeName, getFunction, setFunction) \
	makeViewPropertyAccessor<className, typeName>( \
		[](const className* self, typeName* value) { *value = self->getFunction(); }, \
		[](className* self, const typeName& value) { self->setFunction(value); }) \

} // namespace ln
