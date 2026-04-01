#include <LuminoCore/runtime/ObjectRegistry.hpp>
#include <LuminoCore/CoreInstance.hpp>

// テスト用の Object サブクラス
class TestObject : public ln::Object {
public:
    TestObject(int value = 0) : m_value(value) {}
    int value() const { return m_value; }
private:
    int m_value;
};

class Test_ObjectRegistry : public ::testing::Test {
protected:
    void SetUp() override {
        (void)ln::CoreInstance::initialize(ln::CoreInstance::Settings{});
    }
    void TearDown() override {
        ln::CoreInstance::terminate();
    }
    ln::ObjectRegistry* registry() {
        return ln::CoreInstance::instance()->objectRegistry();
    }
};

TEST_F(Test_ObjectRegistry, RegisterAndResolve) {
    auto obj = ln::Ref<TestObject>::adopt(new TestObject(42));
    auto* raw = obj.get();

    ln::LNHandle handle = registry()->registerObject(obj.get());
    EXPECT_NE(handle, ln::LN_NULL_HANDLE);

    auto* resolved = registry()->resolve(handle);
    EXPECT_EQ(resolved, raw);
    EXPECT_EQ(static_cast<TestObject*>(resolved)->value(), 42);

    EXPECT_TRUE(registry()->release(handle));
}

TEST_F(Test_ObjectRegistry, NullHandleResolvesNull) {
    EXPECT_EQ(registry()->resolve(ln::LN_NULL_HANDLE), nullptr);
}

TEST_F(Test_ObjectRegistry, ReleaseInvalidatesHandle) {
    auto obj = ln::Ref<TestObject>::adopt(new TestObject(1));
    ln::LNHandle handle = registry()->registerObject(obj.get());

    EXPECT_TRUE(registry()->release(handle));
    // 解放後は同じハンドルで resolve できない（世代番号が変わっている）
    EXPECT_EQ(registry()->resolve(handle), nullptr);
    // 二重 release は false
    EXPECT_FALSE(registry()->release(handle));
}

TEST_F(Test_ObjectRegistry, SlotReuseInvalidatesOldHandle) {
    auto obj1 = ln::Ref<TestObject>::adopt(new TestObject(1));
    ln::LNHandle handle1 = registry()->registerObject(obj1.get());
    EXPECT_TRUE(registry()->release(handle1));

    // 新しいオブジェクトがスロットを再利用
    auto obj2 = ln::Ref<TestObject>::adopt(new TestObject(2));
    ln::LNHandle handle2 = registry()->registerObject(obj2.get());

    // 新旧で異なるハンドル（同じインデックスでも世代番号が異なる）
    EXPECT_NE(handle1, handle2);
    // 旧ハンドルは無効
    EXPECT_EQ(registry()->resolve(handle1), nullptr);
    // 新ハンドルは有効
    auto* resolved = registry()->resolve<TestObject>(handle2);
    ASSERT_NE(resolved, nullptr);
    EXPECT_EQ(resolved->value(), 2);

    EXPECT_TRUE(registry()->release(handle2));
}

TEST_F(Test_ObjectRegistry, SizeTracking) {
    EXPECT_EQ(registry()->size(), 0u);

    auto obj1 = ln::Ref<TestObject>::adopt(new TestObject(1));
    auto obj2 = ln::Ref<TestObject>::adopt(new TestObject(2));

    ln::LNHandle h1 = registry()->registerObject(obj1.get());
    EXPECT_EQ(registry()->size(), 1u);

    ln::LNHandle h2 = registry()->registerObject(obj2.get());
    EXPECT_EQ(registry()->size(), 2u);

    registry()->release(h1);
    EXPECT_EQ(registry()->size(), 1u);

    registry()->release(h2);
    EXPECT_EQ(registry()->size(), 0u);
}
