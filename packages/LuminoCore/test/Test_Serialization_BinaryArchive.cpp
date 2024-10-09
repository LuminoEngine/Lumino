#include "Common.hpp"
#include <LuminoCore/IO/MemoryStream.hpp>
#include <LuminoCore/Serialization/Archive.hpp>
#include <LuminoCore/Serialization/BinaryArchiveStore.hpp>

#if 0 // prototype

class Test_Serialization_BinaryArchive : public ::testing::Test {};

TEST_F(Test_Serialization_BinaryArchive, Basic1) {
    class TestClass {
    public:
        int x = 0;
        int y = 0;
        void serialize(Archive& ar) {
            ar& LN_NVP(x);
            ar& LN_NVP(y);
        }
    };
    MemoryStream buf;
    TestClass obj1;
    obj1.x = 1;
    obj1.y = 2;
    auto result1 = BinarySerializer::serialize(obj1, &buf);

    buf.seek(0, SeekOrigin::Begin);
    TestClass obj2;
    auto result2 = BinarySerializer::deserialize(&buf, &obj1);
    //ASSERT_EQ(5, obj2->x);
    //ASSERT_EQ(123, obj2->y);
}

#endif
