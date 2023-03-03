#include "Internal.hpp"
#include <LuminoCore/Runtime/RuntimeContext.hpp>
#include <LuminoEngine/Runtime/detail/BindingValidation.hpp>

namespace ln {

//==============================================================================
// ZVTestClass1

LN_OBJECT_IMPLEMENT(ZVTestClass1, Object) {
}

Result<> ZVTestClass1::init() {
    return Object::init();
}

void ZVTestClass1::setTestDelegate1(ZVTestDelegate1* value) {
    m_testDelegate1 = value;
}

void ZVTestClass1::setTestDelegate2(ZVTestDelegate2* value) {
    m_testDelegate2 = value;
}

void ZVTestClass1::setTestDelegate3(ZVTestDelegate3* value) {
    m_testDelegate3 = value;
}

void ZVTestClass1::callTestDelegate1(int a) {
    m_testDelegate1->call(a);
}

int ZVTestClass1::callTestDelegate2(int a, int b) {
    return m_testDelegate2->call(a, b);
}

void ZVTestClass1::callTestDelegate3() {
    auto obj = makeObject_deprecated<ZVTestClass1>();
    m_testDelegate3->call(obj);
}

Ref<ZVTestPromise1> ZVTestClass1::loadAsync(const String& filePath) {
    return ZVTestPromise1::run([filePath](ZVTestPromise1::Context* p) {
        Thread::sleep(2000);
        if (filePath.isEmpty()) {
            p->reject();
        }
        else {
            auto obj = makeObject_deprecated<ZVTestClass1>();
            obj->setFilePath(filePath);
            p->resolve(obj);
        }
    });
}

Ref<ZVTestPromise2> ZVTestClass1::executeAsync() {
    return ZVTestPromise2::run([](ZVTestPromise2::Context* p) {
        Thread::sleep(2000);
        p->resolve(100);
    });
}

Ref<EventConnection> ZVTestClass1::connectOnEvent1(Ref<ZVTestEventHandler1> handler) {
    return m_event1.connect(handler);
}

void ZVTestClass1::raiseEvent1() {
    m_event1.raise();
}

Ref<EventConnection> ZVTestClass1::connectOnEvent2(Ref<ZVTestEventHandler2> handler) {
    return m_event2.connect(handler);
}

void ZVTestClass1::raiseEvent2() {
    auto e = makeObject_deprecated<ZVTestEventArgs1>(1024);
    m_event2.raise(e);
}

//==============================================================================
// ZVTestEventArgs1

LN_OBJECT_IMPLEMENT(ZVTestEventArgs1, Object) {
}

bool ZVTestEventArgs1::init(int v) {
    if (!Object::init()) return false;
    m_value = v;
    return true;
}

bool ZVTestEventArgs1::init() {
    if (!Object::init()) return false;
    m_value = 0;
    return true;
}

} // namespace ln
