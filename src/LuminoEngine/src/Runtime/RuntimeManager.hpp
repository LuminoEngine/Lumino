#pragma once
#include <stack>
#include <LuminoEngine/Runtime/Common.hpp>
#include <LuminoEngine/Runtime/Runtime.hpp>

namespace ln {
namespace detail {

struct ObjectRuntimeData
{
	int index = -1;	// inded of RuntimeManager::m_objectEntryList (LnHandle)
	int64_t managedObjectId = -1;
	bool fromCreate = false;
};

struct ObjectEntry
{
	Object*	object = nullptr;
	int index = -1;	// inded of RuntimeManager::m_objectEntryList (LnHandle)

	// 1~ である場合、fromCreate は true でなければならない。
	// 0 になった場合、Handle を無効化し、object->release する。
	// fromCreate が false の場合、この値は常に 0 である。（明示的 Release 禁止）
	// Get (fromCreate=fasle && refCount==0) 後、明示的 Retain すると、fromCreate=true, refCount=1 となり object->retain し、Create と同じ扱いになる。
	int refCount = 0;
};

class RuntimeManager
	: public RefObject
{
public:
	struct Settings
	{
	};

	RuntimeManager();
	virtual ~RuntimeManager();
	void init(const Settings& settings);
	void dispose();

	// create の時は想定通りの動作。externalRefCount=1 の状態で作られる。
	// get の場合も同様に作られる。基本方針は COM と同じく、Get したものは Release で解放するべきだが、
	// ObjectEntry は Native 側の Release を受けて強制的に解放されるため、FlatC API を直接使っているときは Get の Release は不要。
	LnHandle makeObjectWrap(Object* obj, bool fromCreate);

	void retainObjectExplicitly(LnHandle handle);
	void releaseObjectExplicitly(LnHandle handle);
	void onDestructObject(Object* obj);

	ObjectEntry* getObjectEntry(LnHandle handle);
    Object* getObjectFromHandle(LnHandle handle);

	void setManagedObjectId(LnHandle handle, int64_t id);
	int64_t getManagedObjectId(LnHandle handle);
	int64_t getManagedTypeInfoId(LnHandle handle);
	
	static void setReferenceCountTracker(LnReferenceCountTrackerCallback callback);
	void setReferenceTrackEnabled(LnHandle handle);
	void onRetainedObject(Object* obj);
	void onReleasedObject(Object* obj);

    static void setRuntimeFinalizedCallback(LnRuntimeFinalizedCallback callback);

	LnResult processException(Exception* e);

    RuntimeStringBuffer* requestCommonStringBuffer() { return &m_commonStringBuffer; }

private:
	List<ObjectEntry> m_objectEntryList;
	std::stack<int> m_objectIndexStack;
	bool m_systemAliving;
	std::mutex m_mutex;
    RuntimeStringBuffer m_commonStringBuffer;
	static LnReferenceCountTrackerCallback m_referenceCountTracker;
    static LnRuntimeFinalizedCallback m_runtimeFinalizedCallback;
};

} // namespace detail
} // namespace ln


