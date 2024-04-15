#include <LuminoEngine/Base/Promise.hpp>

class Test_Base_Promise : public ::testing::Test {};

static std::thread::id g_mainThreadId;

LN_PROMISE()
using TestPromise1 = Promise<Ref<Object>>;

static Ref<TestPromise1> createObjectAsync(bool value) {
    return TestPromise1::run([value](TestPromise1::Context* p) {
        // メインスレッドとは異なるスレッドで実行されている
        LN_CHECK(g_mainThreadId != std::this_thread::get_id());

        if (value) {
            p->resolve(makeObject_deprecated<Object>());
        }
        else {
            p->reject();
        }
    });
}

TEST_F(Test_Base_Promise, Basic) {
    RuntimeModule::initialize();
    g_mainThreadId = std::this_thread::get_id();

    Ref<Object> result;
    createObjectAsync(true)
        ->then([&](Ref<Object> value) {
            // メインスレッドで実行されている
            LN_CHECK(g_mainThreadId == std::this_thread::get_id());
            result = value;
        });

    
    while (!result) {
        EngineContext2::instance()->mainThreadTaskDispatcher()->executeTasks(1);
    }
    // 無限ループせずに終了すればOK.

    RuntimeModule::terminate();
}

TEST_F(Test_Base_Promise, Error) {
    RuntimeModule::initialize();
    g_mainThreadId = std::this_thread::get_id();

    std::atomic<bool> flag = false;
    createObjectAsync(false)
        ->then([&](auto value) {
            LN_UNREACHABLE();
        })
        ->catchWith([&]() {
            // メインスレッドで実行されている
            LN_CHECK(g_mainThreadId == std::this_thread::get_id());
            flag = true;
        });

    while (!flag) {
        EngineContext2::instance()->mainThreadTaskDispatcher()->executeTasks(1);
    }
    // 無限ループせずに終了すればOK.

    RuntimeModule::terminate();
}
