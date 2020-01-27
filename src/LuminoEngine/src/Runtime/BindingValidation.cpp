
#include "Internal.hpp"
#include "BindingValidation.hpp"

namespace ln {
	
//==============================================================================
// ZVTestClass1

LN_OBJECT_IMPLEMENT(ZVTestClass1, Object) {}

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

Ref<ZVTestPromise1> ZVTestClass1::loadAsync(const String& filePath)
{
	std::cout << "ZVTestClass1::loadAsync " << filePath << std::endl;
	return ZVTestPromise1::run([filePath](ZVTestPromise1* p) {
		std::cout << "ZVTestClass1::loadAsync lambda 1 " << p->m_rejected << std::endl;
		Thread::sleep(2000);
		std::cout << "ZVTestClass1::loadAsync lambda 2 " << p->m_rejected << std::endl;
		if (filePath.isEmpty()) {
			std::cout << "if reject " << p->m_rejected << std::endl;
			p->reject();
		}
		else {
			std::cout << "if resolve " << p->m_rejected << std::endl;
			auto obj = makeObject<ZVTestClass1>();
			obj->setFilePath(filePath);
			std::cout << "if resolve " << p->m_rejected << std::endl;
			p->resolve(obj);
		}
	});
}

Ref<ZVTestPromise2> ZVTestClass1::executeAsync()
{
	return ZVTestPromise2::run([](ZVTestPromise2* p) {
		Thread::sleep(2000);
		p->resolve(100);
	});
}

} // namespace ln

