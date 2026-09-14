#include <LuminoBase/RefObject.hpp>
using namespace ln;

namespace {

// 生存しているインスタンス数。デストラクタが呼ばれたかどうかの判定に使う。
int g_liveCount = 0;

class Node : public RefObject {
public:
    Node() { ++g_liveCount; }
    ~Node() override { --g_liveCount; }

    Ref<Node> child;
    int value = 0;
};

} // namespace

class Test_RefObject : public ::testing::Test {
protected:
    void SetUp() override { g_liveCount = 0; }
};

// node = node->child のように、代入元が「今まさに解放される側」のメンバである場合でも安全であること。
// 先に reset() してしまうと、o 自身が解放済みメモリになる。
TEST_F(Test_RefObject, CopyAssignFromMemberOfReleasedObject) {
    {
        Ref<Node> node = Ref<Node>::adopt(new Node());
        node->value = 1;
        node->child = Ref<Node>::adopt(new Node());
        node->child->value = 2;
        ASSERT_EQ(2, g_liveCount);

        node = node->child;

        // 親だけが破棄され、子は node に引き継がれて生き残る。
        EXPECT_EQ(1, g_liveCount);
        EXPECT_EQ(2, node->value);
        EXPECT_EQ(1u, node->refCount());
    }
    EXPECT_EQ(0, g_liveCount);
}

// ムーブ代入も同じ順序の問題を持つ。
TEST_F(Test_RefObject, MoveAssignFromMemberOfReleasedObject) {
    {
        Ref<Node> node = Ref<Node>::adopt(new Node());
        node->value = 1;
        node->child = Ref<Node>::adopt(new Node());
        node->child->value = 2;
        ASSERT_EQ(2, g_liveCount);

        node = std::move(node->child);

        EXPECT_EQ(1, g_liveCount);
        EXPECT_EQ(2, node->value);
        EXPECT_EQ(1u, node->refCount());
    }
    EXPECT_EQ(0, g_liveCount);
}

TEST_F(Test_RefObject, SelfAssignKeepsObjectAlive) {
    {
        Ref<Node> node = Ref<Node>::adopt(new Node());
        const Ref<Node>& alias = node;
        node = alias;

        EXPECT_EQ(1, g_liveCount);
        EXPECT_EQ(1u, node->refCount());
    }
    EXPECT_EQ(0, g_liveCount);
}
