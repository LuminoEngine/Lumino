#include "Common.hpp"

#if 0

class Test_Engine_Object : public ::testing::Test {};

class TestObjectA
    : public Object
{
    LN_OBJECT;
public:
    LN_PROPERTY_DECLARE(int, Prop1);

    void init();

    void setProp1(int value) { m_prop1 = value; }
    int prop1() const { return m_prop1; }

    int m_count = 0;

private:
    static void onProp1Changed(Object* obj);

    Property<int> m_prop1;
};

LN_OBJECT_IMPLEMENT(TestObjectA, Object) {}
LN_PROPERTY_IMPLEMENT(TestObjectA, Prop1, m_prop1, PropertyMetadata(TestObjectA::onProp1Changed));

void TestObjectA::init()
{
    Object::init();
}

void TestObjectA::onProp1Changed(Object* obj)
{
    static_cast<TestObjectA*>(obj)->m_count++;
}


class TestObjectB
    : public Object
{
    LN_OBJECT;
public:
};

LN_OBJECT_IMPLEMENT(TestObjectB, Object) {}

class TestObjectC
    : public Object
{
    LN_OBJECT;
public:
    LN_PROPERTY_DECLARE(int, V1);
    LN_PROPERTY_DECLARE(TestObjectA*, V2);
    //LN_PROPERTY_DECLARE(Point, V3);
    LN_PROPERTY_DECLARE(Ref<TestObjectA>, V4);

    Property<int> m_V1;
    Property<TestObjectA*> m_V2;
    //Property<Point> m_V3;
    Property<Ref<TestObjectA>> m_V4;
    Property<int> m_V5;   // メタデータを持たないプロパティ

    const PropertyInfo* m_lastChangedProp;

    TestObjectC()
        : m_V1(1)
        , m_V2(nullptr)
        //, m_V3()
        , m_V4()
        , m_V5(5)
    {}

    void setV1(int v) { m_V1 = v; }
    int getV1() const { return m_V1; }

    static void onV1Changed(Object* obj) { static_cast<TestObjectC*>(obj)->m_lastChangedProp = V1PropertyId; }
    static void onV2Changed(Object* obj) { static_cast<TestObjectC*>(obj)->m_lastChangedProp = V2PropertyId; }
    //static void onV3Changed(Object* obj) { static_cast<TestObjectC*>(obj)->m_lastChangedProp = V3Id; }
    static void onV4Changed(Object* obj) { static_cast<TestObjectC*>(obj)->m_lastChangedProp = V4PropertyId; }
};

LN_OBJECT_IMPLEMENT(TestObjectC, Object) {}
LN_PROPERTY_IMPLEMENT(TestObjectC, V1, m_V1, PropertyMetadata(onV1Changed));
LN_PROPERTY_IMPLEMENT(TestObjectC, V2, m_V2, PropertyMetadata(onV2Changed));
//LN_PROPERTY_IMPLEMENT(TestObjectC, V3, m_V3, PropertyMetadata(onV3Changed));
LN_PROPERTY_IMPLEMENT(TestObjectC, V4, m_V4, PropertyMetadata(onV4Changed));

//------------------------------------------------------------------------------
TEST_F(Test_Engine_Object, TypeInfo)
{
    auto obj1 = makeObject<TestObjectA>();
    auto obj2 = makeObject<TestObjectB>();

    //* [ ] can get by object pointer
    TypeInfo* type1 = TypeInfo::getTypeInfo(obj1);
    ASSERT_EQ(true, type1 != nullptr);
    ASSERT_EQ(_TT("TestObjectA"), type1->name());

    //* [ ] can get by template
    TypeInfo* type2 = TypeInfo::getTypeInfo<TestObjectB>();
    ASSERT_EQ(true, type2 != nullptr);
    ASSERT_EQ(_TT("TestObjectB"), type2->name());

    //* [ ] can get by object pointer (get as top level)
    Object* base1 = obj1;
    type1 = TypeInfo::getTypeInfo(base1);
    ASSERT_EQ(true, type1 != nullptr);
    ASSERT_EQ(_TT("TestObjectA"), type1->name());

    //* [ ] can get base type
    TypeInfo* type1Base = type1->baseType();
    ASSERT_EQ(_TT("Object"), type1Base->name());

    //* [ ] base type of Object is null
    ASSERT_EQ(nullptr, type1Base->baseType());
}

//------------------------------------------------------------------------------

class CreateFromTypeInfo_ClassA
	: public Object
{
	LN_OBJECT;
public:
	int value1;
};

LN_OBJECT_IMPLEMENT(CreateFromTypeInfo_ClassA, Object) {}

//------------------------------------------------------------------------------
TEST_F(Test_Engine_Object, Property)
{
    auto objA = makeObject<TestObjectA>();
    objA->setProp1(5);
    ASSERT_EQ(5, objA->prop1());

    //* [ ] PropertyChanged コールバックが呼ばれている
    ASSERT_EQ(1, objA->m_count);

    //* [ ] PropertyInfo が取れる
    ASSERT_EQ(true, TestObjectA::Prop1PropertyId != nullptr);


    auto objC = makeObject<TestObjectC>();

    //* [ ] int type
    ASSERT_EQ(1, objC->m_V1);
    objC->m_V1 = 2;
    ASSERT_EQ(2, objC->m_V1);

    //* [ ] pointer type
    ASSERT_EQ(nullptr, objC->m_V2);
    objC->m_V2 = objA;
    ASSERT_EQ(objA, objC->m_V2);

    //* [ ] struct type
    //Point pt1(1, 2);
    //objC->m_V3 = pt1;
    //ASSERT_EQ(1, objC->m_V3.get().x);
    //ASSERT_EQ(2, objC->m_V3.get().y);

    //* [ ] Ref<> type
    ASSERT_EQ(nullptr, objC->m_V4.get());
    objC->m_V4 = objA;
    ASSERT_EQ(objA, objC->m_V4);
}

//------------------------------------------------------------------------------
TEST_F(Test_Engine_Object, Notification)
{
    auto obj = makeObject<TestObjectC>();

    //* [ ] OwnerObject の確認
    {
        ASSERT_EQ(obj, obj->m_V1.ownerObject());
        ASSERT_EQ(obj, obj->m_V2.ownerObject());
        //ASSERT_EQ(obj, obj->m_V3.ownerObject());
        ASSERT_EQ(obj, obj->m_V4.ownerObject());
    }

    //* [ ] onPropertyChanged 通知
    //{
    //    // 初期値と同じだが source が Default → ユーザー値に変わるので通知が来る
    //    obj->setV1(1);
    //    ASSERT_EQ(TestObjectC::V1Id, obj->m_lastChangedProp);

    //    PropertyInfo::setPropertyValue(&obj, TestObjectC::V2Id, nullptr);
    //    ASSERT_EQ(TestObjectC::V2Id, obj->m_lastChangedProp);

    //    PropertyInfo::setTypedValue<int>(obj, TestObjectC::V1Id, 2);
    //    ASSERT_EQ(TestObjectC::V1Id, obj->m_lastChangedProp);

    //    // 値のクリア。現在値と初期値が同じだが、ユーザー値なので通知が来る
    //    obj->m_V2.clearValue();
    //    ASSERT_EQ(TestObjectC::V2Id, obj->m_lastChangedProp);
    //}

    //* [ ] デフォルト値に戻す
    //{
    //    obj->setV1(10);
    //    ASSERT_EQ(10, obj->getV1());

    //    auto ref = PropertyInfo::getProperty(&obj, TestObjectC::V1Id);	// TODO: id のとり方は隠蔽したい
    //    ref.clearValue();

    //    ASSERT_EQ(1, obj->getV1());
    //}
}

#if 0
//------------------------------------------------------------------------------
TEST_F(Test_Engine_Object, GetSetHelper)
{
    TestObjectC t1;

    // <Test> 値型
    {
        // set
        PropertyInfo::setPropertyValue(&t1, TestObjectC::V1Id, 100);
        ASSERT_EQ(100, t1.V1);

        // get
        Variant v1 = PropertyInfo::getPropertyValue(&t1, TestObjectC::V1Id);
        ASSERT_EQ(100, Variant::cast<int>(v1));

        // set (Direct)
        PropertyInfo::setPropertyValueDirect<int>(&t1, TestObjectC::V1Id, 200);
        ASSERT_EQ(200, t1.V1);

        // get (Direct)
        Variant v2 = PropertyInfo::getPropertyValueDirect<int>(&t1, TestObjectC::V1Id);
        ASSERT_EQ(200, Variant::cast<int>(v2));

        // setter/getter
        t1.SetV1(300);
        ASSERT_EQ(300, t1.GetV1());
    }

    // <Test> ポインタ型
    {
        RefTest1 rt1;
        RefTest1 rt2;

        // set
        PropertyInfo::setPropertyValue(&t1, TestObjectC::V2Id, &rt1);
        ASSERT_EQ(&rt1, t1.V2);

        // get
        Variant v1 = PropertyInfo::getPropertyValue(&t1, TestObjectC::V2Id);
        ASSERT_EQ(&rt1, Variant::cast<RefTest1*>(v1));

        // set (Direct)
        PropertyInfo::setPropertyValueDirect<RefTest1*>(&t1, TestObjectC::V2Id, &rt2);
        ASSERT_EQ(&rt2, t1.V2);

        // get (Direct)
        Variant v2 = PropertyInfo::getPropertyValueDirect<RefTest1*>(&t1, TestObjectC::V2Id);
        ASSERT_EQ(&rt2, Variant::cast<RefTest1*>(v2));

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
        PropertyInfo::setPropertyValue(&t1, TestObjectC::V3Id, pt1);
        ASSERT_EQ(1, t1.V3.get().x);
        ASSERT_EQ(2, t1.V3.get().y);

        // get
        Variant v = PropertyInfo::getPropertyValue(&t1, TestObjectC::V3Id);
        ASSERT_EQ(1, Variant::cast<Point>(v).x);
        ASSERT_EQ(2, Variant::cast<Point>(v).y);

        // set (Direct)
        Point pt2(10, 20);
        PropertyInfo::setPropertyValueDirect<Point>(&t1, TestObjectC::V3Id, pt2);
        ASSERT_EQ(10, t1.V3.get().x);
        ASSERT_EQ(20, t1.V3.get().y);

        // get (Direct)
        Variant v2 = PropertyInfo::getPropertyValueDirect<Point>(&t1, TestObjectC::V3Id);
        ASSERT_EQ(10, Variant::cast<Point>(v2).x);
        ASSERT_EQ(20, Variant::cast<Point>(v2).y);

        // setter/getter
        t1.SetV3(Point(3, 4));
        ASSERT_EQ(3, t1.GetV3().x);
        ASSERT_EQ(4, t1.GetV3().y);
    }

    // <Test> Ref
    {
        // set
        Ref<RefTest2> p1(LN_NEW RefTest2(), false);
        PropertyInfo::setPropertyValue(&t1, TestObjectC::V4Id, p1);
        ASSERT_EQ(p1, t1.V4);

        // get
        Variant v = PropertyInfo::getPropertyValue(&t1, TestObjectC::V4Id);
        ASSERT_EQ(p1.get(), Variant::cast<Ref<RefTest2>>(v));

        // set (Direct)
        Ref<RefTest2> p12(LN_NEW RefTest2(), false);
        PropertyInfo::setPropertyValueDirect<Ref<RefTest2>>(&t1, TestObjectC::V4Id, p12);
        ASSERT_EQ(p12, t1.V4);

        // get (Direct)
        Variant v2 = PropertyInfo::getPropertyValueDirect<Ref<RefTest2>>(&t1, TestObjectC::V4Id);
        ASSERT_EQ(p12, Variant::cast<Ref<RefTest2>>(v2));

        // setter/getter
        Ref<RefTest2> p2(LN_NEW RefTest2(), false);
        t1.SetV4(p2);
        ASSERT_EQ(p2.get(), t1.GetV4());

        // setter/getter (nullptr)
        t1.SetV4(nullptr);
        ASSERT_EQ(nullptr, t1.GetV4());
    }
}

//------------------------------------------------------------------------------
TEST_F(Test_Engine_Object, NonMetadataProperty)
{
    auto obj = makeObject<TestObjectC>();

    // get set
    ASSERT_EQ(5, obj->m_V5);
    obj->m_V5 = 10;
    ASSERT_EQ(10, obj->m_V5);

    // 通知は来ない
    ASSERT_EQ(nullptr, obj->m_lastChangedProp);

    // Clear
    obj->m_V5.clearValue();
    ASSERT_EQ(5, obj->m_V5);
}
#endif

//==============================================================================
class Test_Base_WeakRefPtr : public ::testing::Test
{
protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

class WeakRefTest1 : public Object
{
public:
    WeakRefTest1() { count++; }
    virtual ~WeakRefTest1() { count--; }
    int m = 100;
    static int count;
};
int WeakRefTest1::count = 0;

//------------------------------------------------------------------------------
TEST_F(Test_Base_WeakRefPtr, Basic)
{
    WeakRefPtr<WeakRefTest1> weak;
    {
        auto ptr = makeObject<WeakRefTest1>();
        weak = WeakRefPtr<WeakRefTest1>(ptr);
        ASSERT_EQ(true, weak.isAlive());
        ASSERT_EQ(100, weak.resolve()->m);
        ASSERT_EQ(1, WeakRefTest1::count);	// まだ生きている
    }
    ASSERT_EQ(0, WeakRefTest1::count);	// デストラクタが動いた
    ASSERT_EQ(false, weak.isAlive());
    ASSERT_EQ(true, weak.resolve() == nullptr);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_WeakRefPtr, Basic2)
{
    WeakRefPtr<WeakRefTest1> weak;
    {
        WeakRefTest1 t1;	// ローカルオブジェクト
        WeakRefTest1* ptr = &t1;
        weak = WeakRefPtr<WeakRefTest1>(ptr);
        ASSERT_EQ(true, weak.isAlive());
        ASSERT_EQ(100, weak.resolve()->m);
        ASSERT_EQ(1, WeakRefTest1::count);	// まだ生きている
    }
    ASSERT_EQ(0, WeakRefTest1::count);	// デストラクタが動いた
    ASSERT_EQ(false, weak.isAlive());
    ASSERT_EQ(true, weak.resolve() == nullptr);
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_WeakRefPtr, Basic3)
{
    WeakRefPtr<WeakRefTest1> weak;// (&t1);
    {
        WeakRefTest1 t1;	// ローカルオブジェクト
        weak = &t1;
    }
}


//==============================================================================
class Test_Engine_PropertySystem : public ::testing::Test {};

class PropTestObjectA : public Object
{
public:
	int m_int = 0;
	float m_float = 0;

	int getInt() const { return m_int; }
	void setInt(int v) { m_int = v; }
	float getFloat() const { return m_float; }
	void setFloat(float v) { m_float = v; }

	static void registerType(EngineContext* context);
};


//void PropTestObjectA::registerType(EngineContext* context)
//{
//	context->registerType<PropTestObjectA>({
//		makeRef<PropertyInfo>("Int", LN_MAKE_GET_SET_PROPERTY_ACCESSOR(PropTestObjectA, int, getInt, setInt)),
//		makeRef<PropertyInfo>("Float", LN_MAKE_GET_SET_PROPERTY_ACCESSOR(PropTestObjectA, float, getFloat, setFloat)),
//		});
//}

// - 型のプロパティ一覧を取得できること
// - プロパティ名指定で Accessor を取得できること
// - Accessor を使って値を get set できること
// - ベースクラスのプロパティや TypeInfo をとれること
// - 型名指定でオブジェクトを作成できること

//------------------------------------------------------------------------------
TEST_F(Test_Engine_PropertySystem, Basic)
{
	//PropTestObjectA::registerType(EngineContext::current());
	

}

#endif
