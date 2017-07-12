#include <TestConfig.h>
#include <Lumino/Reflection/Notify.h>
#include <Lumino/Reflection/ReflectionEventArgs.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Reflection/Property.h>

#if 0

// テスト用
class Point
{
public:
	int x, y;

	Point()
		: x(0), y(0)
	{}

	Point(int x_, int y_)
		: x(x_), y(y_)
	{}

	bool operator != (const Point& rhs) const
	{
		return x != rhs.x || y != rhs.y;
	}
};

class IntegrationTest_Reflection_ReflectionObject : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

class RefTest1 : public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
};
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(RefTest1, tr::ReflectionObject);

class RefTest2 : public RefTest1
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
};
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(RefTest2, tr::ReflectionObject);

class EventTest1 : public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	LN_REFLECTION_EVENT(tr::ReflectionEventArgs, ClickedEvent);
public:
	tr::ReflectionEvent<tr::ReflectionEventArgs>	Clicked;
	tr::DelegateEvent<int>							Clicked2;

	void OnClicked() { tr::ReflectionEventArgs e; raiseReflectionEvent(Clicked, &e); }
	void OnClicked2(int a) { raiseDelegateEvent(Clicked2, a); }
};
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(EventTest1, tr::ReflectionObject);
LN_REFLECTION_EVENT_IMPLEMENT(EventTest1, tr::ReflectionEventArgs, ClickedEvent, "Clicked", Clicked);

//---------------------------------------------------------------------
TEST_F(IntegrationTest_Reflection_ReflectionObject, Basic)
{
	RefTest1 t1;
	RefTest2 t2;
	String name;

	auto type1 = tr::TypeInfo::getTypeInfo(&t1);
	name = type1->getName();
	ASSERT_TRUE(name.indexOf(_T("RefTest1")) >= 0);

	auto type2 = tr::TypeInfo::getTypeInfo(&t2);
	name = type2->getName();
	ASSERT_TRUE(name.indexOf(_T("RefTest2")) >= 0);

	tr::TypeInfo::getTypeInfo(&t1)->setBindingTypeInfo((void*)1);
	tr::TypeInfo::getTypeInfo(&t2)->setBindingTypeInfo((void*)2);

	ASSERT_EQ(1, (intptr_t)tr::TypeInfo::getBindingTypeInfo(&t1));
	ASSERT_EQ(2, (intptr_t)tr::TypeInfo::getBindingTypeInfo(&t2));

}

//---------------------------------------------------------------------
TEST_F(IntegrationTest_Reflection_ReflectionObject, Event)
{
	int count = 0;
	EventTest1 t1;
	t1.Clicked += [&count](tr::ReflectionEventArgs* e) { count++; };
	t1.OnClicked();
	t1.OnClicked();
	ASSERT_EQ(2, count);
}

//---------------------------------------------------------------------
TEST_F(IntegrationTest_Reflection_ReflectionObject, DelegateEvent)
{
	int count = 0;
	EventTest1 t1;
	t1.Clicked2 += [&count](int a) { count += a;};
	t1.OnClicked2(1);
	t1.OnClicked2(2);
	ASSERT_EQ(3, count);
}

//---------------------------------------------------------------------
static int g_StackDestructCount = 0;
TEST_F(IntegrationTest_Reflection_ReflectionObject, StackDestruct)
{
	// <Test> 参照カウントが残っている状態でデストラクタが呼ばれたら FatalError になる。
	class MyObj : public  RefObject
	{
	};
	g_StackDestructCount = 0;
	Assertion::setNotifyFataiErrorHandler([](const char* file, int line, const char* message) { g_StackDestructCount++; return true; });
	{
		MyObj o1;
		MyObj o2;
		o2.addRef();
	}
	Assertion::setNotifyFataiErrorHandler(nullptr);

	ASSERT_EQ(1, g_StackDestructCount);
}

//==============================================================================
class Test_Reflection_Property : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

class PropertyTest1
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	LN_TR_PROPERTY(int, V1);
	tr::Property<int> V1;
	LN_TR_PROPERTY(RefTest1*, V2);
	tr::Property<RefTest1*> V2;
	LN_TR_PROPERTY(Point, V3);
	tr::Property<Point> V3;
	LN_TR_PROPERTY(RefPtr<RefTest2>, V4);
	tr::Property<RefPtr<RefTest2>> V4;
	tr::Property<int> V5;	// メタデータを持たないプロパティ
	LN_TR_PROPERTY(int, V6);
	tr::Property<int> V6;

public:
	PropertyTest1()
		: V1(1)
		, V2(nullptr)
		, V3()
		, V4()
		, V5(5)
		, m_lastChangedProp(nullptr)
		, m_v6ChangedCount(0)
	{
		initializeProperties();
	}

	void SetV1(int v) { V1 = v; }
	int GetV1() const { return V1; }

	void SetV2(RefTest1* v) { V2 = v; }
	RefTest1* GetV2() const { return V2; }

	void SetV3(const Point& v) { V3 = v; }
	const Point& GetV3() const { return V3; }

	void SetV4(RefTest2* v) { V4.set(v); }
	//RefTest2* GetV4() const { return V4; }	// TODO: できればコレがやりたいのだが・・・
	RefTest2* GetV4() const { return V4.get(); }
	RefPtr<RefTest2> GetV4_2() const { return V4; }

	virtual void onPropertyChanged(tr::PropertyChangedEventArgs* e) override
	{
		m_lastChangedProp = e->changedProperty;
	}

public:
	const tr::PropertyInfo*	m_lastChangedProp;
	int m_v6ChangedCount;

private:
	static void V6Changed(tr::ReflectionObject* obj)
	{
		static_cast<PropertyTest1*>(obj)->m_v6ChangedCount++;
	}
};
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PropertyTest1, tr::ReflectionObject);
LN_TR_PROPERTY_IMPLEMENT(PropertyTest1, int, V1, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(PropertyTest1, RefTest1*, V2, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(PropertyTest1, Point, V3, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(PropertyTest1, RefPtr<RefTest2>, V4, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(PropertyTest1, int, V6, tr::PropertyMetadata(PropertyTest1::V6Changed));

//------------------------------------------------------------------------------
TEST_F(Test_Reflection_Property, GetSet)
{
	PropertyTest1 obj;
	RefTest1 t2;
	auto t3 = RefPtr<RefTest2>::makeRef();

	// int type
	ASSERT_EQ(1, obj.V1);
	obj.V1 = 2;
	ASSERT_EQ(2, obj.V1);

	// pointer type
	ASSERT_EQ(nullptr, obj.V2);
	obj.V2 = &t2;
	ASSERT_EQ(&t2, obj.V2);

	// struct type
	Point pt1(1, 2);
	obj.V3 = pt1;
	ASSERT_EQ(1, obj.V3.get().x);
	ASSERT_EQ(2, obj.V3.get().y);

	// RefPtr<> type
	ASSERT_EQ(nullptr, obj.V4.get());
	obj.V4 = t3;
	ASSERT_EQ(t3, obj.V4);
}

//------------------------------------------------------------------------------
TEST_F(Test_Reflection_Property, Notification)
{
	PropertyTest1 obj;
	// <Test> OwnerObject の確認
	{
		ASSERT_EQ(&obj, obj.V1.getOwnerObject());
		ASSERT_EQ(&obj, obj.V2.getOwnerObject());
		ASSERT_EQ(&obj, obj.V3.getOwnerObject());
		ASSERT_EQ(&obj, obj.V4.getOwnerObject());
	}

	// <Test> onPropertyChanged 通知
	{
		// 初期値と同じだが source が Default → ユーザー値に変わるので通知が来る
		obj.SetV1(1);
		ASSERT_EQ(PropertyTest1::V1Id, obj.m_lastChangedProp);

		tr::PropertyInfo::setPropertyValue(&obj, PropertyTest1::V2Id, nullptr);
		ASSERT_EQ(PropertyTest1::V2Id, obj.m_lastChangedProp);

		tr::PropertyInfo::setPropertyValueDirect<int>(&obj, PropertyTest1::V1Id, 2);
		ASSERT_EQ(PropertyTest1::V1Id, obj.m_lastChangedProp);

		// 値のクリア。現在値と初期値が同じだが、ユーザー値なので通知が来る
		obj.V2.clearValue();
		ASSERT_EQ(PropertyTest1::V2Id, obj.m_lastChangedProp);
	}

	// <Test> デフォルト値に戻す
	{
		obj.SetV1(10);
		ASSERT_EQ(10, obj.GetV1());

		auto ref = tr::PropertyInfo::getProperty(&obj, PropertyTest1::V1Id);	// TODO: id のとり方は隠蔽したい
		ref.clearValue();

		ASSERT_EQ(1, obj.GetV1());
	}

	// <Test> static PropertyChanged 通知
	{
		obj.V6 = 100;
		ASSERT_EQ(1, obj.m_v6ChangedCount);
	}
}

//------------------------------------------------------------------------------
TEST_F(IntegrationTest_Reflection_ReflectionObject, GetSetHelper)
{
	PropertyTest1 t1;

	// <Test> 値型
	{
		// set
		tr::PropertyInfo::setPropertyValue(&t1, PropertyTest1::V1Id, 100);
		ASSERT_EQ(100, t1.V1);

		// get
		tr::Variant v1 = tr::PropertyInfo::getPropertyValue(&t1, PropertyTest1::V1Id);
		ASSERT_EQ(100, tr::Variant::cast<int>(v1));

		// set (Direct)
		tr::PropertyInfo::setPropertyValueDirect<int>(&t1, PropertyTest1::V1Id, 200);
		ASSERT_EQ(200, t1.V1);

		// get (Direct)
		tr::Variant v2 = tr::PropertyInfo::getPropertyValueDirect<int>(&t1, PropertyTest1::V1Id);
		ASSERT_EQ(200, tr::Variant::cast<int>(v2));

		// setter/getter
		t1.SetV1(300);
		ASSERT_EQ(300, t1.GetV1());
	}

	// <Test> ポインタ型
	{
		RefTest1 rt1;
		RefTest1 rt2;

		// set
		tr::PropertyInfo::setPropertyValue(&t1, PropertyTest1::V2Id, &rt1);
		ASSERT_EQ(&rt1, t1.V2);

		// get
		tr::Variant v1 = tr::PropertyInfo::getPropertyValue(&t1, PropertyTest1::V2Id);
		ASSERT_EQ(&rt1, tr::Variant::cast<RefTest1*>(v1));

		// set (Direct)
		tr::PropertyInfo::setPropertyValueDirect<RefTest1*>(&t1, PropertyTest1::V2Id, &rt2);
		ASSERT_EQ(&rt2, t1.V2);

		// get (Direct)
		tr::Variant v2 = tr::PropertyInfo::getPropertyValueDirect<RefTest1*>(&t1, PropertyTest1::V2Id);
		ASSERT_EQ(&rt2, tr::Variant::cast<RefTest1*>(v2));

		// setter/getter
		RefTest1 rt3;
		t1.SetV2(&rt3);
		ASSERT_EQ(&rt3, t1.GetV2());

		// setter/getter (nullptr)
		t1.SetV2(nullptr);
		ASSERT_EQ(nullptr, t1.GetV2());
	}

	// <Test> 構造体
	{
		// set
		Point pt1(1, 2);
		tr::PropertyInfo::setPropertyValue(&t1, PropertyTest1::V3Id, pt1);
		ASSERT_EQ(1, t1.V3.get().x);
		ASSERT_EQ(2, t1.V3.get().y);

		// get
		tr::Variant v = tr::PropertyInfo::getPropertyValue(&t1, PropertyTest1::V3Id);
		ASSERT_EQ(1, tr::Variant::cast<Point>(v).x);
		ASSERT_EQ(2, tr::Variant::cast<Point>(v).y);

		// set (Direct)
		Point pt2(10, 20);
		tr::PropertyInfo::setPropertyValueDirect<Point>(&t1, PropertyTest1::V3Id, pt2);
		ASSERT_EQ(10, t1.V3.get().x);
		ASSERT_EQ(20, t1.V3.get().y);

		// get (Direct)
		tr::Variant v2 = tr::PropertyInfo::getPropertyValueDirect<Point>(&t1, PropertyTest1::V3Id);
		ASSERT_EQ(10, tr::Variant::cast<Point>(v2).x);
		ASSERT_EQ(20, tr::Variant::cast<Point>(v2).y);

		// setter/getter
		t1.SetV3(Point(3, 4));
		ASSERT_EQ(3, t1.GetV3().x);
		ASSERT_EQ(4, t1.GetV3().y);
	}

	// <Test> RefPtr
	{
		// set
		RefPtr<RefTest2> p1(LN_NEW RefTest2(), false);
		tr::PropertyInfo::setPropertyValue(&t1, PropertyTest1::V4Id, p1);
		ASSERT_EQ(p1, t1.V4);

		// get
		tr::Variant v = tr::PropertyInfo::getPropertyValue(&t1, PropertyTest1::V4Id);
		ASSERT_EQ(p1.get(), tr::Variant::cast<RefPtr<RefTest2>>(v));

		// set (Direct)
		RefPtr<RefTest2> p12(LN_NEW RefTest2(), false);
		tr::PropertyInfo::setPropertyValueDirect<RefPtr<RefTest2>>(&t1, PropertyTest1::V4Id, p12);
		ASSERT_EQ(p12, t1.V4);

		// get (Direct)
		tr::Variant v2 = tr::PropertyInfo::getPropertyValueDirect<RefPtr<RefTest2>>(&t1, PropertyTest1::V4Id);
		ASSERT_EQ(p12, tr::Variant::cast<RefPtr<RefTest2>>(v2));

		// setter/getter
		RefPtr<RefTest2> p2(LN_NEW RefTest2(), false);
		t1.SetV4(p2);
		ASSERT_EQ(p2.get(), t1.GetV4());

		// setter/getter (nullptr)
		t1.SetV4(nullptr);
		ASSERT_EQ(nullptr, t1.GetV4());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Reflection_Property, NonMetadataProperty)
{
	PropertyTest1 t1;

	// get set
	ASSERT_EQ(5, t1.V5);
	t1.V5 = 10;
	ASSERT_EQ(10, t1.V5);

	// 通知は来ない
	ASSERT_EQ(nullptr, t1.m_lastChangedProp);

	// Clear
	t1.V5.clearValue();
	ASSERT_EQ(5, t1.V5);
}
#endif

//==============================================================================================================================================================
#if 0

namespace tr2 {
class PropertyInfo;
class Object;

namespace detail {

enum class PropertyMapEntryType
{
	End,
	Register,
};

struct PropertyMapEntry
{
	PropertyMapEntryType type;
	const PropertyInfo* (*getPropertyInfo)();
};

struct PropertyMap
{
	//const PropertyMap* base;
	const PropertyMap* (*getBaseMap)();
	const PropertyMapEntry* entries;
};

class TypeInfoManager
{
public:
	

private:
};

} // namespace detail


 //
class PropertyMetadata
{
public:
	typedef void(*ChangedCallback)(Object* obj);

	PropertyMetadata()
		: m_changedCallback(nullptr)
	{}

	PropertyMetadata(ChangedCallback changedCallback)
		: m_changedCallback(changedCallback)
	{}

private:
	ChangedCallback	m_changedCallback;
};

class TypeInfo
{
public:

private:
};



//
class PropertyInfo
{

};

//
template<typename TValue>
class TypedPropertyInfo
	: public PropertyInfo
{
public:
	typedef void(*SetterFunc)(Object* obj, const TValue& value);
	typedef const TValue&(*GetterFunc)(Object* obj);

	TypedPropertyInfo(SetterFunc setter, GetterFunc getter, PropertyMetadata metadata)
		: m_setter(setter)
		, m_getter(getter)
		, m_metadata(metadata)
	{}

private:
	SetterFunc			m_setter;
	GetterFunc			m_getter;
	PropertyMetadata	m_metadata;
};

// 
class Object
{
public:

protected:
	void initialize();
	static const tr2::detail::PropertyMap* lnrs_getThisPropertyMap() { return nullptr; }
	virtual const detail::PropertyMap* lnrs_getPropertyMap() const { return nullptr; }

private:
};

class PropertyBase
{
protected:

};

//
template<typename TValue>
class Property
	: public PropertyBase
{
public:
	void set(const TValue& value) { m_value = value; }
	const TValue& get() const { return m_value; }

private:
	TValue	m_value;
};

} // namespace tr2

//-----------------------------------------------------------------------------


class TR2ObjTest1 : tr2::Object
{
public:

	//// MACRO begin
protected:
	static const tr2::detail::PropertyMap* lnrs_getThisPropertyMap();
	virtual const tr2::detail::PropertyMap* lnrs_getPropertyMap() const override;
	//// MACRO end

private:
	tr::Property<int> m_value1;

	static void Value1Changed(Object* obj)
	{
		printf("changed\n");
	}
};


//// MACRO begin
const tr2::detail::PropertyMap* TR2ObjTest1::lnrs_getPropertyMap() const
{
	return lnrs_getThisPropertyMap();
}
const tr2::detail::PropertyMap* TR2ObjTest1::lnrs_getThisPropertyMap()
{
	static const tr2::detail::PropertyMapEntry localEntries[] =
	{
//// MACRO end

		{ tr2::detail::PropertyMapEntryType::Register,[]() -> const tr2::PropertyInfo* { static tr2::TypedPropertyInfo<int> info([](tr2::Object* obj, const int& value) { static_cast<TR2ObjTest1*>(obj)->m_value1.set(value); }, [](tr2::Object* obj) -> const int& { return static_cast<TR2ObjTest1*>(obj)->m_value1.get(); }, tr2::PropertyMetadata()); return &info; } },

//// MACRO begin
		{ tr2::detail::PropertyMapEntryType::End },
	};
	static const tr2::detail::PropertyMap localMap = { &tr2::Object::lnrs_getThisPropertyMap, &localEntries[0] };
	return &localMap;
}
//// MACRO end

// WPF...
//		PropInfo: 型、変数名、公開名、初期値
//		MEtadata: get, set, changed, validate, corace



class Test_Reflection_ReflectionObject2 : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Reflection_ReflectionObject2, Basic)
{
	TR2ObjTest1 obj1;
	obj1.initialize();
}


#endif

