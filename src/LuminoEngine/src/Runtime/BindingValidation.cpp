
#include "Internal.hpp"
#include "BindingValidation.hpp"

namespace ln {
	
//==============================================================================
// ZVTestClass1

LN_OBJECT_IMPLEMENT(ZVTestClass1, Object)
{
    context->registerType<ZVTestClass1>({});
}

void ZVTestClass1::init()
{
	Object::init();
}

void ZVTestClass1::setTestDelegate1(ZVTestDelegate1* value)
{
	m_testDelegate1 = value;
}

void ZVTestClass1::setTestDelegate2(ZVTestDelegate2* value)
{
	m_testDelegate2 = value;
}

void ZVTestClass1::setTestDelegate3(ZVTestDelegate3* value)
{
	m_testDelegate3 = value;
}

void ZVTestClass1::callTestDelegate1(int a)
{
	m_testDelegate1->call(a);
}

int ZVTestClass1::callTestDelegate2(int a, int b)
{
	return m_testDelegate2->call(a, b);
}

void ZVTestClass1::callTestDelegate3()
{
	auto obj = makeObject<ZVTestClass1>();
	m_testDelegate3->call(obj);
}

//Ref<ZVTestPromise1> ZVTestClass1::loadAsync()
//{
//	return ZVTestPromise1::run([](ZVTestPromise1* p) {
//		p->resolve(makeObject<ZVTestClass1>());
//	});
//}

} // namespace ln

