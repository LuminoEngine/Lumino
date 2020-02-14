#pragma once
#include <LuminoEngine/Base/Event.hpp>
#include <LuminoEngine/Base/Promise.hpp>

namespace ln {
class ZVTestClass1;
class ZVTestEventArgs1;

/**
 * Test delegate 1.
 *
 * @param[in] a : param 1.
 */
LN_DELEGATE()
using ZVTestDelegate1 = Delegate<void(int a)>;

/**
 * Test delegate 2.
 *
 * @param[in] a : param 1.
 * @param[in] b : param 2.
 */
LN_DELEGATE()
using ZVTestDelegate2 = Delegate<int(int a, int b)>;

/**
 * Test delegate 3.
 *
 * @param[in] a : param 1.
 */
LN_DELEGATE()
using ZVTestDelegate3 = Delegate<void(Ref<ZVTestClass1> a)>;

/**
 * Test delegate 4.
 */
LN_DELEGATE()
using ZVTestDelegate4 = Delegate<void(void)>;

/**
 * Test promise.
 */
LN_PROMISE()
using ZVTestPromise1 = Promise<Ref<ZVTestClass1>>;

/**
 * Test promise.
 */
LN_PROMISE()
using ZVTestPromise2 = Promise<int>;

/**
 * Test class.
 */
LN_CLASS()
class ZVTestClass1
	: public Object
{
	LN_OBJECT;
public:

	/** setTestDelegate1 method. */
	LN_METHOD()
	void setTestDelegate1(ZVTestDelegate1* value);

	/** setTestDelegate2 method. */
	LN_METHOD()
	void setTestDelegate2(ZVTestDelegate2* value);

	/** setTestDelegate3 method. */
	LN_METHOD()
	void setTestDelegate3(ZVTestDelegate3* value);

	/** callTestDelegate1 method. */
	LN_METHOD()
	void callTestDelegate1(int a);

	/** callTestDelegate2 method. */
	LN_METHOD()
	int callTestDelegate2(int a, int b);

	/** callTestDelegate3 method. (create object in internal) */
	LN_METHOD()
	void callTestDelegate3();

	/**
	 * Promise test method. (static)
	 */
	LN_METHOD()
	static Ref<ZVTestPromise1> loadAsync(const String& filePath);

	/**
	 * Promise test method. (instance)
	 */
	LN_METHOD()
	Ref<ZVTestPromise2> executeAsync();

	/** Promise test method. */
	LN_METHOD(Property)
	const String& filePath() const { return m_filePath; }

	void setFilePath(const String& value) { m_filePath = value; }

	//--------------------
	// Event testing.
	
	/** connectOnClicked method. */
	LN_METHOD(Event)
	Ref<EventConnection> connectOnEvent1(Ref<ZVTestDelegate4> handler);

	/** raiseEvent1 method. */
	LN_METHOD()
	void raiseEvent1();
	
LN_CONSTRUCT_ACCESS:
    /** init method. */
    LN_METHOD()
	bool init();

private:
	Ref<ZVTestDelegate1> m_testDelegate1;
	Ref<ZVTestDelegate2> m_testDelegate2;
	Ref<ZVTestDelegate3> m_testDelegate3;
	String m_filePath;
	Event<ZVTestDelegate4> m_event1;
};

/**
 * Test class.
 */
LN_CLASS()
class ZVTestEventArgs1
	: public Object
{
	LN_OBJECT;
public:
	/** value method. */
	LN_METHOD()
	int value() const { return m_value; }

LN_CONSTRUCT_ACCESS:
    /** init method. */
    LN_METHOD()
	bool init();

    /** init method. */
    LN_METHOD(OverloadPostfix = "WithValue")
	bool init(int v);

private:
	int m_value;
};

} // namespace ln

